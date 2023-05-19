#ifndef __CHASH_INTERNAL_HASH_H__
#define __CHASH_INTERNAL_HASH_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "ccore/c_allocator.h"

namespace ncore
{
    namespace nhash
    {
        template <s32 N> struct digest_t
        {
            static const s32 SIZE = N;
            inline s32       size() const { return SIZE; }
            u8               m_data[SIZE];
        };

        typedef digest_t<16>  md5;
        typedef digest_t<20>  sha1;
        typedef digest_t<32>  skein256;
        typedef digest_t<64>  skein512;
        typedef digest_t<128> skein1024;
        typedef digest_t<4>   murmur32;
        typedef digest_t<8>   murmur64;
        typedef digest_t<8>   xxhash64;
    }; // namespace nhash

    struct hash_header_t
    {
        u64 type;
    };

    struct md5_t
    {
        hash_header_t hdr;
        md5_t();

        s32  size() const { return sizeof(nhash::md5); }
        void reset();
        void hash(u8 const* data, u32 size);
        void end(u8* hash);

        u64           m_ctxt[12];
    };

    struct sha1_t
    {
        hash_header_t hdr;
        sha1_t();

        s32  size() const { return sizeof(nhash::sha1); }
        void reset();
        void hash(u8 const* data, u32 size);
        void end(u8* hash);

        u64 m_ctxt[12];
    };

    struct skein256_t
    {
        hash_header_t hdr;
        skein256_t();

        s32  size() const { return sizeof(nhash::skein256); }
        void reset();
        void hash(u8 const* data, u32 size);
        void end(u8* hash);

        bool m_initialized;
        u64  m_ctxt[11];
    };

    struct skein512_t
    {
        hash_header_t hdr;
        skein512_t();

        s32  size() const { return sizeof(nhash::skein512); }
        void reset();
        void hash(u8 const* data, u32 size);
        void end(u8* hash);

        bool m_initialized;
        u64  m_ctxt[19];
    };

    struct skein1024_t
    {
        hash_header_t hdr;
        skein1024_t();

        s32  size() const { return sizeof(nhash::skein1024); }
        void reset();
        void hash(u8 const* data, u32 size);
        void end(u8* hash);

        bool m_initialized;
        u64  m_ctxt[35];
    };

    struct murmur32_t
    {
        hash_header_t hdr;
        murmur32_t(u32 seed = 0);

        s32  size() const { return sizeof(nhash::murmur32); }
        void reset();
        void hash(u8 const* data, u32 size);
        void end(u8* hash);

        u32 m_seed;
        u32 m_hash;
    };

    struct murmur64_t
    {
        hash_header_t hdr;
        murmur64_t(u64 seed = 0);

        s32  size() const { return sizeof(nhash::murmur64); }
        void reset();
        void hash(u8 const* data, u32 size);
        void end(u8* hash);

        u64 m_seed;
        u64 m_hash;
    };

    struct xxhash64_t
    {
        hash_header_t hdr;
        xxhash64_t(u64 seed = 0);

        s32  size() const { return sizeof(nhash::xxhash64); }
        void reset();
        void hash(u8 const* data, u32 size);
        void end(u8* hash);

        u64 m_seed;
        u64 m_ctxt[11];
    };

} // namespace ncore

#endif
