#include "ccore/c_target.h"
#include "ccore/c_endian.h"

#include "chash/private/c_internal_hash.h"

namespace ncore
{
    static u64 murmur64_read_le(const u8* data)
    {
        u64 value = 0;
        for (s32 i = 0; i < 8; ++i)
            value |= u64(data[i]) << (8 * i);
        return value;
    }

    static u64 murmur64_rotl(u64 value, s32 bits) { return (value << bits) | (value >> (64 - bits)); }

    static u64 murmur64_fmix(u64 value)
    {
        value ^= value >> 33;
        value *= 0xff51afd7ed558ccdULL;
        value ^= value >> 33;
        value *= 0xc4ceb9fe1a85ec53ULL;
        value ^= value >> 33;
        return value;
    }

    static void murmur64_mix_block(u64& hash1, u64& hash2, u64 block1, u64 block2)
    {
        block1 *= 0x87c37b91114253d5ULL;
        block1 = murmur64_rotl(block1, 31);
        block1 *= 0x4cf5ad432745937fULL;
        hash1 ^= block1;
        hash1 = murmur64_rotl(hash1, 27);
        hash1 += hash2;
        hash1 = hash1 * 5 + 0x52dce729;

        block2 *= 0x4cf5ad432745937fULL;
        block2 = murmur64_rotl(block2, 33);
        block2 *= 0x87c37b91114253d5ULL;
        hash2 ^= block2;
        hash2 = murmur64_rotl(hash2, 31);
        hash2 += hash1;
        hash2 = hash2 * 5 + 0x38495ab5;
    }

    ncore::u64 gGetMurmurHash64(const u8* buffer, u32 size, u64 seed)
    {
        nhash::murmur64_t hash;
        hash.reset(seed);
        hash.hash(buffer, buffer + size);
        u8 digest[sizeof(u64)];
        hash.end(digest);
        return murmur64_read_le(digest);
    }

    namespace nhash
    {
        void murmur64_t::reset(u64 seed)
        {
            m_seed = u32(seed) ^ u32(seed >> 32);
            m_hash = 0;
            m_hash1 = m_seed;
            m_hash2 = m_seed;
            m_length = 0;
            m_tail_size = 0;
        }

        void murmur64_t::hash(const u8* data, const u8* end)
        {
            m_length += (u64)(end - data);

            if (m_tail_size > 0)
            {
                while (data < end && m_tail_size < 16)
                    m_tail[m_tail_size++] = *data++;
                if (m_tail_size == 16)
                {
                    murmur64_mix_block(m_hash1, m_hash2, murmur64_read_le(m_tail), murmur64_read_le(m_tail + 8));
                    m_tail_size = 0;
                }
            }

            while (data + 16 <= end)
            {
                murmur64_mix_block(m_hash1, m_hash2, murmur64_read_le(data), murmur64_read_le(data + 8));
                data += 16;
            }

            while (data < end)
                m_tail[m_tail_size++] = *data++;
        }

        void murmur64_t::end(u8* _hash)
        {
            u64 hash1 = m_hash1;
            u64 hash2 = m_hash2;
            u64 block1 = 0;
            u64 block2 = 0;
            for (s32 i = 0; i < m_tail_size && i < 8; ++i)
                block1 |= u64(m_tail[i]) << (8 * i);
            for (s32 i = 8; i < m_tail_size; ++i)
                block2 |= u64(m_tail[i]) << (8 * (i - 8));
            if (m_tail_size > 8)
            {
                block2 *= 0x4cf5ad432745937fULL;
                block2 = murmur64_rotl(block2, 33);
                block2 *= 0x87c37b91114253d5ULL;
                hash2 ^= block2;
            }
            if (m_tail_size > 0)
            {
                block1 *= 0x87c37b91114253d5ULL;
                block1 = murmur64_rotl(block1, 31);
                block1 *= 0x4cf5ad432745937fULL;
                hash1 ^= block1;
            }
            hash1 ^= m_length;
            hash2 ^= m_length;
            hash1 += hash2;
            hash2 += hash1;
            hash1 = murmur64_fmix(hash1);
            hash2 = murmur64_fmix(hash2);
            hash1 += hash2;

            u8 const* src = (u8 const*)&hash1;
            for (int i = 0; i < 8; i++)
                _hash[i] = *src++;
        }
    } // namespace nhash
} // namespace ncore
