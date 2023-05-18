#include "ccore/c_target.h"
#include "cbase/c_va_list.h"
#include "cbase/c_integer.h"
#include "cbase/c_memory.h"
#include "cbase/c_endian.h"

#include "chash/c_hash.h"

namespace ncore
{
#ifdef D_LITTLE_ENDIAN
#define MURMUR64_ENDIAN_SWAP(r) (r)
#else
#define MURMUR64_ENDIAN_SWAP(r) (((r >> 24) & 0x000000FF) | ((r >> 8) & 0x0000FF00) | ((r << 8) & 0x00FF0000) | ((r << 24) & 0xFF000000))
#endif

    ncore::u64 gGetMurmurHash64(cbuffer_t const& buffer, u64 seed)
    {
        const ncore::u32 m = 0x5bd1e995;
        const ncore::s32 r = 24;

        u32 len = (u32)buffer.size();

        ncore::u32 h1 = ncore::u32(seed) ^ len;
        ncore::u32 h2 = ncore::u32(seed >> 32);

        const ncore::u32* data = (const ncore::u32*)buffer.m_const;

        while (len >= 8)
        {
            ncore::u32 k1 = *data++;
            k1            = MURMUR64_ENDIAN_SWAP(k1);
            k1 *= m;
            k1 ^= k1 >> r;
            k1 *= m;
            h1 *= m;
            h1 ^= k1;
            len -= 4;

            ncore::u32 k2 = *data++;
            k2            = MURMUR64_ENDIAN_SWAP(k2);
            k2 *= m;
            k2 ^= k2 >> r;
            k2 *= m;
            h2 *= m;
            h2 ^= k2;
            len -= 4;
        }

        if (len >= 4)
        {
            ncore::u32 k1 = *data++;
            k1            = MURMUR64_ENDIAN_SWAP(k1);
            k1 *= m;
            k1 ^= k1 >> r;
            k1 *= m;
            h1 *= m;
            h1 ^= k1;
            len -= 4;
        }

        switch (len)
        {
            case 3: h2 ^= ((ncore::u8*)data)[2] << 16;
            case 2: h2 ^= ((ncore::u8*)data)[1] << 8;
            case 1: h2 ^= ((ncore::u8*)data)[0]; h2 *= m;
        };

        h1 ^= h2 >> 18;
        h1 *= m;
        h2 ^= h1 >> 22;
        h2 *= m;
        h1 ^= h2 >> 17;
        h1 *= m;
        h2 ^= h1 >> 19;
        h2 *= m;

        ncore::u64 h = h1;

        h = (h << 32) | h2;

        return h;
    }

    murmur64_t::murmur64_t(u64 seed)
        : m_seed(seed)
        , m_hash(seed)
    {
    }

    void murmur64_t::reset() { m_hash = m_seed; }

    void murmur64_t::hash(cbuffer_t const& _buffer) { m_hash = gGetMurmurHash64(_buffer, m_hash); }

    void murmur64_t::end(nhash::murmur64& _hash)
    {
        u64            p   = nendian_ne::swap(m_hash);
        u8 const*   src = (u8 const*)&p;
        binary_writer_t writer(_hash.buffer());
        for (int i = 0; i < 8; i++)
            writer.write(*src++);
    }

    void murmur64_t::compute(cbuffer_t const& data, nhash::murmur64& out_hash)
    {
        reset();
        hash(data);
        end(out_hash);
    }

    nhash::murmur64 murmur64_t::compute(cbuffer_t const& data)
    {
        nhash::murmur64 hash;
        compute(data, hash);
        return hash;
    }

}
