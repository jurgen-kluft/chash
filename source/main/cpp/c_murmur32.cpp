#include "ccore/c_target.h"
#include "cbase/c_endian.h"

#include "chash/private/c_internal_hash.h"

namespace ncore
{
    ncore::u32 gGetMurmurHash32(const u8* _data, u32 size, u32 seed)
    {
        // 'm' and 'r' are mixing constants generated offline.
        // They're not really 'magic', they just happen to work well.

        const ncore::u32 m      = 0x5bd1e995;
        const ncore::s32 r      = 24;
        ncore::u32       length = size;

        // Initialize the hash to a 'random' value

        ncore::u32 h = seed ^ length;

        // Mix 4 bytes at a time into the hash

        const ncore::u8* data = _data;
        while (length >= 4)
        {
            ncore::u32 k;

            k = ncore::u32(data[0]);
            k |= ncore::u32(data[1]) << 8;
            k |= ncore::u32(data[2]) << 16;
            k |= ncore::u32(data[3]) << 24;

            k *= m;
            k ^= k >> r;
            k *= m;

            h *= m;
            h ^= k;

            data += 4;
            length -= 4;
        }

        switch (length)
        {
            case 3: h ^= ncore::u32(data[2]) << 16;
            case 2: h ^= ncore::u32(data[1]) << 8;
            case 1: h ^= ncore::u32(data[0]); h *= m;
        };

        h ^= h >> 13;
        h *= m;
        h ^= h >> 15;

        return h;
    }

    void murmur32_t::reset(u64 seed)
    {
        m_seed = (u32)seed;
        m_hash = m_seed;
    }

    void murmur32_t::hash(const u8* _buffer, u8 const* _end) { m_hash = gGetMurmurHash32(_buffer, (u32)(_end - _buffer), m_hash); }

    void murmur32_t::end(u8* _hash)
    {
        u32       p   = nendian_ne::swap(m_hash);
        u8 const* src = (u8 const*)&p;
        _hash[0]      = src[0];
        _hash[1]      = src[1];
        _hash[2]      = src[2];
        _hash[3]      = src[3];
    }

} // namespace ncore
