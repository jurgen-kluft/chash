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

    namespace ehashtype
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

        typedef u32 value_t;
        enum
        {
            MD5          = (1 << IndexShift) | (16 << SizeShift) | (sizeof(nhash_private::md5_t) << CtxSizeShift),
            SHA1         = (2 << IndexShift) | (20 << SizeShift) | (sizeof(nhash_private::sha1_t) << CtxSizeShift),
            Skein256     = (3 << IndexShift) | (32 << SizeShift) | (sizeof(nhash_private::skein256_t) << CtxSizeShift),
            Skein512     = (4 << IndexShift) | (64 << SizeShift) | (sizeof(nhash_private::skein512_t) << CtxSizeShift),
            Skein1024    = (5 << IndexShift) | (128 << SizeShift) | (sizeof(nhash_private::skein1024_t) << CtxSizeShift),
            Murmur32     = (6 << IndexShift) | (4 << SizeShift) | (sizeof(nhash_private::murmur32_t) << CtxSizeShift),
            Murmur64     = (7 << IndexShift) | (8 << SizeShift) | (sizeof(nhash_private::murmur64_t) << CtxSizeShift),
            XXHash64     = (8 << IndexShift) | (8 << SizeShift) | (sizeof(nhash_private::xxhash64_t) << CtxSizeShift),
            SpookyHashV2 = (9 << IndexShift) | (16 << SizeShift) | (sizeof(nhash_private::spookyhashv2_t) << CtxSizeShift),
        };

        static inline s32 size(value_t type) { return (s32)((type & SizeMask) >> SizeShift); }
    }; // namespace ehashtype

    typedef void* hash_instance_t;

    hash_instance_t create_hash(alloc_t* allocator, ehashtype::value_t type);
    void            destroy_hash(alloc_t* allocator, hash_instance_t h);
    s32             hash_size(hash_instance_t ctxt);
    void            hash_begin(hash_instance_t ctxt);
    void            hash_update(hash_instance_t ctxt, const u8* begin, const u8* end);
    void            hash_end(hash_instance_t ctxt, u8* hash, s32 size);

} // namespace ncore

#endif
