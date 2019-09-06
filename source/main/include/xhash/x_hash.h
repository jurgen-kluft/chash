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
                static const u32 SIZE = 16;
				s32       size() const { return SIZE; }
                xbyte     m_data[SIZE];
                xbuffer   buffer() const { return xbuffer(SIZE, (xbyte*)m_data); }
				xcbuffer  cbuffer() const { return xcbuffer(SIZE, (xbyte const*)m_data); }
            };
            struct sha1
            {
                static const s32 SIZE = 20;
				s32       size() const { return SIZE; }
                xbyte     m_data[SIZE];
                xbuffer   buffer() const { return xbuffer(SIZE, (xbyte*)m_data); }
				xcbuffer  cbuffer() const { return xcbuffer(SIZE, (xbyte const*)m_data); }
            };
            struct skein256
            {
                static const s32 SIZE = 32;
				s32       size() const { return SIZE; }
                xbyte     m_data[SIZE];
                xbuffer   buffer() const { return xbuffer(SIZE, (xbyte*)m_data); }
				xcbuffer  cbuffer() const { return xcbuffer(SIZE, (xbyte const*)m_data); }
            };
            struct skein512
            {
                static const s32 SIZE = 64;
				s32       size() const { return SIZE; }
                xbyte     m_data[SIZE];
                xbuffer   buffer() const { return xbuffer(SIZE, (xbyte*)m_data); }
				xcbuffer  cbuffer() const { return xcbuffer(SIZE, (xbyte const*)m_data); }
            };
            struct skein1024
            {
                static const s32 SIZE = 128;
				s32       size() const { return SIZE; }
                xbyte     m_data[SIZE];
                xbuffer   buffer() const { return xbuffer(SIZE, (xbyte*)m_data); }
				xcbuffer  cbuffer() const { return xcbuffer(SIZE, (xbyte const*)m_data); }
            };
            struct murmur32
            {
                static const s32 SIZE = 4;
				s32       size() const { return SIZE; }
                u32       m_data;
                xbuffer   buffer() const { return xbuffer(SIZE, (xbyte*)&m_data); }
				xcbuffer  cbuffer() const { return xcbuffer(SIZE, (xbyte const*)&m_data); }
            };
            struct murmur64
            {
                static const s32 SIZE = 8;
				s32       size() const { return SIZE; }
                u64       m_data;
                xbuffer   buffer() const { return xbuffer(SIZE, (xbyte*)&m_data); }
				xcbuffer  cbuffer() const { return xcbuffer(SIZE, (xbyte const*)&m_data); }
            };
			struct xxhash64
			{
                static const s32 SIZE = 8;
				s32       size() const { return SIZE; }
                u64       m_data;
                xbuffer   buffer() const { return xbuffer(SIZE, (xbyte*)&m_data); }
				xcbuffer  cbuffer() const { return xcbuffer(SIZE, (xbyte const*)&m_data); }
			};

			template<class T>
			inline bool equal(const T& a, const T& b)
			{
				xcbuffer ba = a.cbuffer();
				xcbuffer bb = b.cbuffer();
				return ba == bb;
			}
			template<class T>
			inline bool not_equal(const T& a, const T& b)
			{
				xcbuffer ba = a.cbuffer();
				xcbuffer bb = b.cbuffer();
				return ba != bb;
			}
			template<class T>
			inline s32 compare(const T& a, const T& b)
			{
				xcbuffer ba = a.cbuffer();
				xcbuffer bb = b.cbuffer();
				return ba.compare(bb);
			}
		};

        struct xmd5
        {
			xmd5();

			s32 size() const { return sizeof(hash::md5); }
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

			s32 size() const { return sizeof(hash::sha1); }
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

			s32 size() const { return sizeof(hash::skein256); }
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

			s32 size() const { return sizeof(hash::skein512); }
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

			s32 size() const { return sizeof(hash::skein1024); }
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
			xmurmur32(u32 seed = 0);

			s32 size() const { return sizeof(hash::murmur32); }
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
            xmurmur64(u64 seed = 0);

			s32 size() const { return sizeof(hash::murmur64); }
			void reset();
            void hash(xcbuffer const& data);
            void end(hash::murmur64&);

            void           compute(xcbuffer const& data, hash::murmur64&);
            hash::murmur64 compute(xcbuffer const& data);

            u64 m_seed;
            u64 m_hash;
        };
        static xmurmur64 murmur64;

        struct xxhash64
        {
            xxhash64(u64 seed = 0);

			s32 size() const { return sizeof(hash::xxhash64); }
			void reset();
            void hash(xcbuffer const& data);
            void end(hash::xxhash64&);

            void           compute(xcbuffer const& data, hash::xxhash64&);
            hash::xxhash64	compute(xcbuffer const& data);

            u64 m_seed;
			u64 m_ctxt[11];
        };
        static xxhash64 xxhash;

	};

} // namespace xcore

#endif
