#include "xbase/x_target.h"
#include "xbase/x_endian.h"
#include "xbase/x_memory.h"
#include "xbase/x_buffer.h"

#include "xhash/x_hash.h"

namespace xcore
{
    xcore::u32 gGetMurmurHash32(xcbuffer const& buffer, u32 seed)
    {
        // 'm' and 'r' are mixing constants generated offline.
        // They're not really 'magic', they just happen to work well.

        const xcore::u32 m      = 0x5bd1e995;
        const xcore::s32 r      = 24;
        xcore::u32       length = (u32)buffer.size();

        // Initialize the hash to a 'random' value

        xcore::u32 h = seed ^ length;

        // Mix 4 bytes at a time into the hash

        const xcore::u8* data = (const xcore::u8*)buffer.m_const;
        while (length >= 4)
        {
            xcore::u32 k;

            k = xcore::u32(data[0]);
            k |= xcore::u32(data[1]) << 8;
            k |= xcore::u32(data[2]) << 16;
            k |= xcore::u32(data[3]) << 24;

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
            case 3: h ^= xcore::u32(data[2]) << 16;
            case 2: h ^= xcore::u32(data[1]) << 8;
            case 1: h ^= xcore::u32(data[0]); h *= m;
        };

        h ^= h >> 13;
        h *= m;
        h ^= h >> 15;

        return h;
    }

    xhash::murmur32::murmur32(u32 seed = 0)
        : m_seed(seed)
        , m_hash(seed)
    {
    }

    s32 xhash::xmurmur32::size() const { return 32; }

    void xhash::xmurmur32::reset() { m_hash = m_seed; }

    void xhash::xmurmur32::hash(xcbuffer const& _buffer) { m_hash = gGetMurmurHash32(_buffer, m_hash); }

    void xhash::xmurmur32::end(xbuffer& _hash)
    {
        u32            p   = xhtonl(m_hash);
        xbyte const*   src = (xbyte const*)&p;
        xbinary_writer writer(bytes);
        writer.write(*src++);
        writer.write(*src++);
        writer.write(*src++);
        writer.write(*src++);
    }
}
