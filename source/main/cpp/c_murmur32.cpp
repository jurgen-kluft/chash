#include "cbase/c_target.h"
#include "cbase/c_va_list.h"
#include "cbase/c_integer.h"
#include "cbase/c_memory.h"
#include "cbase/c_endian.h"

#include "chash/c_hash.h"

namespace ncore
{
    ncore::u32 gGetMurmurHash32(cbuffer_t const& buffer, u32 seed)
    {
        // 'm' and 'r' are mixing constants generated offline.
        // They're not really 'magic', they just happen to work well.

        const ncore::u32 m      = 0x5bd1e995;
        const ncore::s32 r      = 24;
        ncore::u32       length = (u32)buffer.size();

        // Initialize the hash to a 'random' value

        ncore::u32 h = seed ^ length;

        // Mix 4 bytes at a time into the hash

        const ncore::u8* data = (const ncore::u8*)buffer.m_const;
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

    murmur32_t::murmur32_t(u32 seed)
        : m_seed(seed)
        , m_hash(seed)
    {
    }

    void murmur32_t::reset() { m_hash = m_seed; }

    void murmur32_t::hash(cbuffer_t const& _buffer) { m_hash = gGetMurmurHash32(_buffer, m_hash); }

    void murmur32_t::end(ndigest::murmur32& _hash)
    {
        u32             p   = x_NetworkEndian::swap(m_hash);
        u8 const*    src = (u8 const*)&p;
        binary_writer_t writer(_hash.buffer());
        writer.write(*src++);
        writer.write(*src++);
        writer.write(*src++);
        writer.write(*src++);
    }

    void murmur32_t::compute(cbuffer_t const& data, ndigest::murmur32& out_hash)
    {
        reset();
        hash(data);
        end(out_hash);
    }

    ndigest::murmur32 murmur32_t::compute(cbuffer_t const& data)
    {
        ndigest::murmur32 hash;
        compute(data, hash);
        return hash;
    }

} // namespace ncore
