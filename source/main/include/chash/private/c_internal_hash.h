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
        typedef digest_t<32>  sha256;
        typedef digest_t<32>  skein256;
        typedef digest_t<64>  skein512;
        typedef digest_t<128> skein1024;
        typedef digest_t<4>   murmur32;
        typedef digest_t<8>   murmur64;
        typedef digest_t<8>   xxhash64;
        typedef digest_t<8>   wyhash64;
        typedef digest_t<16>  spookyhashv2;
    }; // namespace nhash

    namespace nhash
    {
        struct hash_header_t
        {
            u32 type;
            u32 dummy;
        };

        struct md5_t
        {
            hash_header_t hdr;
            u64           m_ctxt[12];

            s32  size() const { return sizeof(nhash::md5); }
            void reset(u64 seed = 0x67452301efcdab89);
            void hash(u8 const* data, u8 const* end);
            void end(u8* hash);
        };

        struct sha1_t
        {
            hash_header_t hdr;
            u64           m_ctxt[12];

            s32  size() const { return sizeof(nhash::sha1); }
            void reset(u64 seed = 0);
            void hash(u8 const* data, u8 const* end);
            void end(u8* hash);
        };

        struct sha256_t
        {
            hash_header_t hdr;
            u64           m_ctxt[13];

            s32  size() const { return sizeof(nhash::sha256); }
            void reset(u64 seed = 0);
            void hash(u8 const* data, u8 const* end);
            void end(u8* hash);
        };

        struct skein256_t
        {
            hash_header_t hdr;
            u64           m_initialized;
            u64           m_ctxt[11];

            s32  size() const { return sizeof(nhash::skein256); }
            void reset(u64 seed = 0);
            void hash(u8 const* data, u8 const* end);
            void end(u8* hash);
        };

        struct skein512_t
        {
            hash_header_t hdr;
            u64           m_initialized;
            u64           m_ctxt[19];

            s32  size() const { return sizeof(nhash::skein512); }
            void reset(u64 seed = 0);
            void hash(u8 const* data, u8 const* end);
            void end(u8* hash);
        };

        struct skein1024_t
        {
            hash_header_t hdr;
            u64           m_initialized;
            u64           m_ctxt[35];

            s32  size() const { return sizeof(nhash::skein1024); }
            void reset(u64 seed = 0);
            void hash(u8 const* data, u8 const* end);
            void end(u8* hash);
        };

        struct murmur32_t
        {
            hash_header_t hdr;
            u32           m_seed;
            u32           m_hash;
            u64           m_length;
            u8            m_tail[4];
            s32           m_tail_size;

            s32  size() const { return sizeof(nhash::murmur32); }
            void reset(u64 seed = 0);
            void hash(u8 const* data, u8 const* end);
            void end(u8* hash);
        };

        struct murmur64_t
        {
            hash_header_t hdr;
            u32           m_seed;
            u64           m_hash;
            u64           m_hash1;
            u64           m_hash2;
            u64           m_length;
            u8            m_tail[16];
            s32           m_tail_size;

            s32  size() const { return sizeof(nhash::murmur64); }
            void reset(u64 seed = 0);
            void hash(u8 const* data, u8 const* end);
            void end(u8* hash);
        };

        struct xxhash64_t
        {
            hash_header_t hdr;
            u64           m_seed;
            u64           m_ctxt[11];

            s32  size() const { return sizeof(nhash::xxhash64); }
            void reset(u64 seed = 0);
            void hash(u8 const* data, u8 const* end);
            void end(u8* hash);
        };

        struct wyhash64_t
        {
            hash_header_t hdr;
            u64           m_ctxt[10];
            u64           m_seed;

            s32  size() const { return sizeof(nhash::wyhash64); }
            void reset(u64 seed = 0);
            void hash(u8 const* data, u8 const* end);
            void end(u8* hash);
        };

        struct spookyhashv2_t
        {
            hash_header_t hdr;
            u64           m_seed;
            u64           m_ctxt[38];

            s32  size() const { return sizeof(nhash::spookyhashv2); }
            void reset(u64 seed = 0, u64 seed2 = 0);
            void hash(u8 const* data, u8 const* end);
            void end(u8* hash);

            static void hash128(const void* message, s64 length, u64* hash1, u64* hash2);
            static u64  hash64(const void* message, s64 length, u64 seed);
            static u32  hash32(const void* message, s64 length, u32 seed);
        };
    } // namespace nhash
} // namespace ncore

#endif
