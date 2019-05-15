#ifndef __XHASH_HASH_H__
#define __XHASH_HASH_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase/x_buffer.h"

namespace xcore
{
    class xhash
    {
    public:
        struct xmd5
        {
            const s32 SIZE = 16;

            struct hash
            {
                xbyte   m_data[SIZE];
                xbuffer buffer() const { return xbuffer(SIZE, m_data); }
            };

            void reset();
            void hash(xcbuffer const& data);
            void end(hash&);

            void compute(xcbuffer const& data, hash&);
            hash compute(xcbuffer const& data);

            u64 m_ctxt[12];
        };
        static xmd5 md5;

        struct xsha1
        {
            const s32 SIZE = 20;

            struct hash
            {
                xbyte   m_data[SIZE];
                xbuffer buffer() const { return xbuffer(SIZE, m_data); }
            };

            sha1();
            void reset();
            void hash(xcbuffer const& data);
            void end(hash&);

            void compute(xcbuffer const& data, hash&);
            hash compute(xcbuffer const& data);

            u64 m_ctxt[12];
        };
        static xsha1 sha1;

        struct xskein256
        {
            const s32 SIZE = 32;

            struct hash
            {
                xbyte   m_data[SIZE];
                xbuffer buffer() const { return xbuffer(SIZE, m_data); }
            };

            xskein256();
            void reset();
            void hash(xcbuffer const& data);
            void end(hash&);

            void compute(xcbuffer const& data, hash&);
            hash compute(xcbuffer const& data);

            u64 m_ctxt[11];
        };
        static xskein256 skein256;

        struct xskein512
        {
            const s32 SIZE = 64;

            struct hash
            {
                xbyte   m_data[SIZE];
                xbuffer buffer() const { return xbuffer(SIZE, m_data); }
            };

            xskein512();
            void reset();
            void hash(xcbuffer const& data);
            void end(hash&);

            void compute(xcbuffer const& data, hash&);
            hash compute(xcbuffer const& data);

            u64 m_ctxt[19];
        };
        static xskein512 skein512;

        struct xskein1024
        {
            const s32 SIZE = 128;

            struct hash
            {
                xbyte   m_data[SIZE];
                xbuffer buffer() const { return xbuffer(SIZE, m_data); }
            };

            xskein1024();
            void reset();
            void hash(xcbuffer const& data);
            void end(hash&);

            void compute(xcbuffer const& data, hash&);
            hash compute(xcbuffer const& data);

            u64 m_ctxt[35];
        };
        static xskein1024 skein1024;

        struct xmurmur32
        {
            const s32 SIZE = 4;

            struct hash
            {
                xbyte   m_data[SIZE];
                xbuffer buffer() const { return xbuffer(SIZE, m_data); }
            };

            murmur32(u32 seed = 0);
            void reset();
            void hash(xcbuffer const& data);
            void end(hash&);

            void compute(xcbuffer const& data, hash&);
            hash compute(xcbuffer const& data);

            u32 m_seed;
            u32 m_hash;
        };
        static xmurmur32 murmur32;

        struct xmurmur64
        {
            const s32 SIZE = 8;

            struct hash
            {
                xbyte   m_data[SIZE];
                xbuffer buffer() const { return xbuffer(SIZE, m_data); }
            };

            murmur64(u32 seed = 0);
            void reset();
            void hash(xcbuffer const& data);
            void end(hash&);

            void compute(xcbuffer const& data, hash&);
            hash compute(xcbuffer const& data);

            u64 m_seed;
        };
        static xmurmur64 murmur64;
    };

    static void UseCase()
    {
        using namespace xhash;
        const char* mystring = "this is a string that i want to hash";
        xsha1.hash  myhash   = sha1.compute(xcbuffer::from_ascii_string(mystring));
    }
} // namespace xcore

#endif
