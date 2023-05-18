#ifndef __CHASH_HASH_H__
#define __CHASH_HASH_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "cbase/c_buffer.h"

namespace ncore
{
    namespace nhash
    {
        template <s32 N> struct digest_t
        {
            static const s32 SIZE = N;
            inline s32       size() const { return SIZE; }
            u8               m_data[SIZE];
            inline buffer_t  buffer() const { return buffer_t(SIZE, (u8*)m_data); }
            inline cbuffer_t cbuffer() const { return cbuffer_t(SIZE, (u8 const*)m_data); }
        };

        template <s32 N> inline bool equal(const digest_t<N>& a, const digest_t<N>& b)
        {
            cbuffer_t ba = a.cbuffer();
            cbuffer_t bb = b.cbuffer();
            return ba == bb;
        }
        template <s32 N> inline s32 compare(const digest_t<N>& a, const digest_t<N>& b)
        {
            cbuffer_t ba = a.cbuffer();
            cbuffer_t bb = b.cbuffer();
            return ba.compare(bb);
        }

        typedef digest_t<16>  md5;
        typedef digest_t<20>  sha1;
        typedef digest_t<32>  skein256;
        typedef digest_t<64>  skein512;
        typedef digest_t<128> skein1024;
        typedef digest_t<4>   murmur32;
        typedef digest_t<8>   murmur64;
        typedef digest_t<8>   xxhash64;
    }; // namespace nhash

    struct md5_t
    {
        md5_t();

        s32  size() const { return sizeof(nhash::md5); }
        void reset();
        void hash(cbuffer_t const& data);
        void end(nhash::md5&);

        void       compute(cbuffer_t const& data, nhash::md5&);
        nhash::md5 compute(cbuffer_t const& data);

        u64 m_ctxt[12];
    };
    static md5_t md5;

    struct sha1_t
    {
        sha1_t();

        s32  size() const { return sizeof(nhash::sha1); }
        void reset();
        void hash(cbuffer_t const& data);
        void end(nhash::sha1&);

        void        compute(cbuffer_t const& data, nhash::sha1&);
        nhash::sha1 compute(cbuffer_t const& data);

        u64 m_ctxt[12];
    };
    static sha1_t sha1;

    struct skein256_t
    {
        skein256_t();

        s32  size() const { return sizeof(nhash::skein256); }
        void reset();
        void hash(cbuffer_t const& data);
        void end(nhash::skein256&);

        void            compute(cbuffer_t const& data, nhash::skein256&);
        nhash::skein256 compute(cbuffer_t const& data);

        bool m_initialized;
        u64  m_ctxt[11];
    };
    static skein256_t skein256;

    struct skein512_t
    {
        skein512_t();

        s32  size() const { return sizeof(nhash::skein512); }
        void reset();
        void hash(cbuffer_t const& data);
        void end(nhash::skein512&);

        void            compute(cbuffer_t const& data, nhash::skein512&);
        nhash::skein512 compute(cbuffer_t const& data);

        bool m_initialized;
        u64  m_ctxt[19];
    };
    static skein512_t skein512;

    struct skein1024_t
    {
        skein1024_t();

        s32  size() const { return sizeof(nhash::skein1024); }
        void reset();
        void hash(cbuffer_t const& data);
        void end(nhash::skein1024&);

        void             compute(cbuffer_t const& data, nhash::skein1024&);
        nhash::skein1024 compute(cbuffer_t const& data);

        bool m_initialized;
        u64  m_ctxt[35];
    };
    static skein1024_t skein1024;

    struct murmur32_t
    {
        murmur32_t(u32 seed = 0);

        s32  size() const { return sizeof(nhash::murmur32); }
        void reset();
        void hash(cbuffer_t const& data);
        void end(nhash::murmur32&);

        void            compute(cbuffer_t const& data, nhash::murmur32&);
        nhash::murmur32 compute(cbuffer_t const& data);

        u32 m_seed;
        u32 m_hash;
    };
    static murmur32_t murmur32;

    struct murmur64_t
    {
        murmur64_t(u64 seed = 0);

        s32  size() const { return sizeof(nhash::murmur64); }
        void reset();
        void hash(cbuffer_t const& data);
        void end(nhash::murmur64&);

        void            compute(cbuffer_t const& data, nhash::murmur64&);
        nhash::murmur64 compute(cbuffer_t const& data);

        u64 m_seed;
        u64 m_hash;
    };
    static murmur64_t murmur64;

    struct xxhash64_t
    {
        xxhash64_t(u64 seed = 0);

        s32  size() const { return sizeof(nhash::xxhash64); }
        void reset();
        void hash(cbuffer_t const& data);
        void end(nhash::xxhash64&);

        void            compute(cbuffer_t const& data, nhash::xxhash64&);
        nhash::xxhash64 compute(cbuffer_t const& data);

        u64 m_seed;
        u64 m_ctxt[11];
    };
    static xxhash64_t xxhash64;

} // namespace ncore

#endif
