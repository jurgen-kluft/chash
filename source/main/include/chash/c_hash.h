#ifndef __CHASH_HASH_H__
#define __CHASH_HASH_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "chash/private/c_internal_hash.h"

namespace ncore
{
    class alloc_t;

    namespace nhash
    {
        enum
        {
            IndexMask    = 0x0000007F,
            IndexShift   = 0,
            SizeMask     = 0x0000FF80,
            SizeShift    = 7,
            CtxSizeMask  = 0xFFFF0000,
            CtxSizeShift = 16,
        };

        typedef u32 type_t;
        enum
        {
            MD5          = (1 << IndexShift) | (16 << SizeShift) | (sizeof(md5_t) << CtxSizeShift),
            Skein256     = (3 << IndexShift) | (32 << SizeShift) | (sizeof(skein256_t) << CtxSizeShift),
            Skein512     = (4 << IndexShift) | (64 << SizeShift) | (sizeof(skein512_t) << CtxSizeShift),
            Skein1024    = (5 << IndexShift) | (128 << SizeShift) | (sizeof(skein1024_t) << CtxSizeShift),
            Murmur32     = (6 << IndexShift) | (4 << SizeShift) | (sizeof(murmur32_t) << CtxSizeShift),
            Murmur64     = (7 << IndexShift) | (8 << SizeShift) | (sizeof(murmur64_t) << CtxSizeShift),
            XXHash64     = (8 << IndexShift) | (8 << SizeShift) | (sizeof(xxhash64_t) << CtxSizeShift),
            SpookyHashV2 = (9 << IndexShift) | (16 << SizeShift) | (sizeof(spookyhashv2_t) << CtxSizeShift),
            SHA1         = (10 << IndexShift) | (20 << SizeShift) | (sizeof(sha1_t) << CtxSizeShift),
            SHA256       = (11 << IndexShift) | (32 << SizeShift) | (sizeof(sha256_t) << CtxSizeShift),
        };

        static inline s32 digest_size(type_t type) { return (s32)((type & SizeMask) >> SizeShift); }
        static inline s32 context_size(type_t type) { return (s32)((type & CtxSizeMask) >> CtxSizeShift); }
    }; // namespace nhash

    typedef void* hash_instance_t;

    // Make sure that 'memory' points to a valid and large enough block of memory
    // to hold the hash instance of the specified type.
    // The size of the memory block required can be determined using the nhash::context_size(type) function.
    // Also the memory block should be aligned to 8 bytes.

    hash_instance_t create_hash(void* memory, nhash::type_t type);
    void            destroy_hash(void* memory, hash_instance_t h);
    s32             hash_size(hash_instance_t ctxt); // Returns the size of the hash output for the given hash instance.
    void            hash_begin(hash_instance_t ctxt);
    void            hash_update(hash_instance_t ctxt, const u8* begin, const u8* end);
    void            hash_end(hash_instance_t ctxt, u8* hash, s32 size);

} // namespace ncore

#endif
