#ifndef __CHASH_HASH_H__
#define __CHASH_HASH_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    class alloc_t;

    struct hash_type_t
    {
        enum
        {
            IndexMask    = 0xFFF,
            IndexShift   = 20,
            SizeMask     = 0xFFFFF,
            SizeShift    = 0,
            MD5          = (1 << IndexShift) | (16 << SizeShift),
            SHA1         = (2 << IndexShift) | (20 << SizeShift),
            Skein256     = (3 << IndexShift) | (32 << SizeShift),
            Skein512     = (4 << IndexShift) | (64 << SizeShift),
            Skein1024    = (5 << IndexShift) | (128 << SizeShift),
            Murmur32     = (6 << IndexShift) | (4 << SizeShift),
            Murmur64     = (7 << IndexShift) | (8 << SizeShift),
            XXHash64     = (8 << IndexShift) | (8 << SizeShift),
            SpookyHashV2 = (9 << IndexShift) | (16 << SizeShift),
        };
        u32 value;
    };

    typedef void* hash_instance_t;

    hash_instance_t create_hash(alloc_t* allocator, hash_type_t type);
    void            destroy_hash(alloc_t* allocator, hash_instance_t h);
    s32             hash_size(hash_instance_t ctxt);
    void            hash_begin(hash_instance_t ctxt);
    void            hash_update(hash_instance_t ctxt, const u8* begin, const u8* end);
    void            hash_end(hash_instance_t ctxt, u8* hash, s32 size);

} // namespace ncore

#endif
