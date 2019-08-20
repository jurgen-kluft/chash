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
        class hash
        {
        public:
            struct md5
            {
                const s32 SIZE = 16;
                xbyte     m_data[SIZE];
                xbuffer   buffer() const { return xbuffer(SIZE, m_data); }
            };
            struct sha1
            {
                const s32 SIZE = 20;
                xbyte     m_data[SIZE];
                xbuffer   buffer() const { return xbuffer(SIZE, m_data); }
            };
            struct xskein256
            {
                const s32 SIZE = 32;
                xbyte     m_data[SIZE];
                xbuffer   buffer() const { return xbuffer(SIZE, m_data); }
            };
            struct xskein512
            {
                const s32 SIZE = 64;
                xbyte     m_data[SIZE];
                xbuffer   buffer() const { return xbuffer(SIZE, m_data); }
            };
            struct xskein1024
            {
                const s32 SIZE = 128;
                xbyte     m_data[SIZE];
                xbuffer   buffer() const { return xbuffer(SIZE, m_data); }
            };
            struct xmurmur32
            {
                const s32 SIZE = 4;
                xbyte     m_data[SIZE];
                xbuffer   buffer() const { return xbuffer(SIZE, m_data); }
            };
            struct xmurmur64
            {
                const s32 SIZE = 8;
                xbyte     m_data[SIZE];
                xbuffer   buffer() const { return xbuffer(SIZE, m_data); }
            };
        };

        struct xmd5
        {
            void reset();
            void hash(xcbuffer const& data);
            void end(hash::md5&);

            void      compute(xcbuffer const& data, hash::md5&);
            hash::md5 compute(xcbuffer const& data);

            u64 m_ctxt[12];
        };
        static xmd5 md5;

        struct xsha1
        {
            xsha1();
            void reset();
            void hash(xcbuffer const& data);
            void end(hash::sha1&);

            void       compute(xcbuffer const& data, hash::sha1&);
            hash::sha1 compute(xcbuffer const& data);

            u64 m_ctxt[12];
        };
        static xsha1 sha1;

        struct xskein256
        {
            xskein256();

            void reset();
            void hash(xcbuffer const& data);
            void end(hash::skein256&);

            void           compute(xcbuffer const& data, hash::skein256&);
            hash::skein256 compute(xcbuffer const& data);

            bool m_initialized;
            u64  m_ctxt[11];
        };
        static xskein256 skein256;

        struct xskein512
        {
            xskein512();
            void reset();
            void hash(xcbuffer const& data);
            void end(hash::skein512&);

            void           compute(xcbuffer const& data, hash::skein512&);
            hash::skein512 compute(xcbuffer const& data);

            bool m_initialized;
            u64  m_ctxt[19];
        };
        static xskein512 skein512;

        struct xskein1024
        {
            xskein1024();
            void reset();
            void hash(xcbuffer const& data);
            void end(hash::skein1024&);

            void            compute(xcbuffer const& data, hash::skein1024&);
            hash::skein1024 compute(xcbuffer const& data);

            bool m_initialized;
            u64  m_ctxt[35];
        };
        static xskein1024 skein1024;

        struct xmurmur32
        {
            murmur32(u32 seed = 0);
            void reset();
            void hash(xcbuffer const& data);
            void end(hash::murmur32&);

            void           compute(xcbuffer const& data, hash::murmur32&);
            hash::murmur32 compute(xcbuffer const& data);

            u32 m_seed;
            u32 m_hash;
        };
        static xmurmur32 murmur32;

        struct xmurmur64
        {
            murmur64(u32 seed = 0);
            void reset();
            void hash(xcbuffer const& data);
            void end(hash::murmur64&);

            void           compute(xcbuffer const& data, hash::murmur64&);
            hash::murmur64 compute(xcbuffer const& data);

            u64 m_seed;
        };
        static xmurmur64 murmur64;
    };

    static void UseCase()
    {
        using namespace xhash;
        const char* mystring = "this is a string that i want to hash";
        hash::sha1  myhash   = sha1.compute(xcbuffer::from_ascii_string(mystring));
    }
} // namespace xcore

#endif
