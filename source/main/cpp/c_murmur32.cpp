#include "ccore/c_target.h"
#include "ccore/c_endian.h"

#include "chash/private/c_internal_hash.h"

namespace ncore
{
    static u32 murmur32_read_le(const u8* data)
    {
        return u32(data[0]) | (u32(data[1]) << 8) | (u32(data[2]) << 16) | (u32(data[3]) << 24);
    }

    static u32 murmur32_rotl(u32 value, s32 bits) { return (value << bits) | (value >> (32 - bits)); }

    static u32 murmur32_fmix(u32 value)
    {
        value ^= value >> 16;
        value *= 0x85ebca6b;
        value ^= value >> 13;
        value *= 0xc2b2ae35;
        value ^= value >> 16;
        return value;
    }

    static u32 murmur32_mix_block(u32 hash, u32 block)
    {
        block *= 0xcc9e2d51;
        block = murmur32_rotl(block, 15);
        block *= 0x1b873593;
        hash ^= block;
        hash = murmur32_rotl(hash, 13);
        return hash * 5 + 0xe6546b64;
    }

    ncore::u32 gGetMurmurHash32(const u8* _data, u32 size, u32 seed)
    {
        nhash::murmur32_t hash;
        hash.reset(seed);
        hash.hash(_data, _data + size);
        u8 digest[sizeof(u32)];
        hash.end(digest);
        return murmur32_read_le(digest);
    }

    namespace nhash
    {
        void murmur32_t::reset(u64 seed)
        {
            m_seed = (u32)seed;
            m_hash = m_seed;
            m_length = 0;
            m_tail_size = 0;
        }

        void murmur32_t::hash(const u8* data, u8 const* end)
        {
            m_length += (u64)(end - data);

            if (m_tail_size > 0)
            {
                while (data < end && m_tail_size < 4)
                    m_tail[m_tail_size++] = *data++;
                if (m_tail_size == 4)
                {
                    m_hash = murmur32_mix_block(m_hash, murmur32_read_le(m_tail));
                    m_tail_size = 0;
                }
            }

            while (data + 4 <= end)
            {
                m_hash = murmur32_mix_block(m_hash, murmur32_read_le(data));
                data += 4;
            }

            while (data < end)
                m_tail[m_tail_size++] = *data++;
        }

        void murmur32_t::end(u8* _hash)
        {
            u32 hash = m_hash;
            u32 tail = 0;
            for (s32 i = 0; i < m_tail_size; ++i)
                tail |= u32(m_tail[i]) << (8 * i);
            if (m_tail_size > 0)
            {
                tail *= 0xcc9e2d51;
                tail = murmur32_rotl(tail, 15);
                tail *= 0x1b873593;
                hash ^= tail;
            }
            hash ^= (u32)m_length;
            hash = murmur32_fmix(hash);

            u8 const* src = (u8 const*)&hash;
            _hash[0]      = src[0];
            _hash[1]      = src[1];
            _hash[2]      = src[2];
            _hash[3]      = src[3];
        }
    } // namespace nhash
} // namespace ncore
