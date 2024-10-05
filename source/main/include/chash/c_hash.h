#ifndef __CHASH_HASH_H__
#define __CHASH_HASH_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    class alloc_t;

    namespace __nprivate__
    {
        enum
        {
            IndexMask    = 0xFFF,
            IndexShift   = 20,
            SizeMask     = 0xFFFFF,
            SizeShift    = 0,
        };
    }

    struct ehashtype
    {
        enum
        {
            MD5          = (1 << __nprivate__::IndexShift) | (16 << __nprivate__::SizeShift),
            SHA1         = (2 << __nprivate__::IndexShift) | (20 << __nprivate__::SizeShift),
            Skein256     = (3 << __nprivate__::IndexShift) | (32 << __nprivate__::SizeShift),
            Skein512     = (4 << __nprivate__::IndexShift) | (64 << __nprivate__::SizeShift),
            Skein1024    = (5 << __nprivate__::IndexShift) | (128 << __nprivate__::SizeShift),
            Murmur32     = (6 << __nprivate__::IndexShift) | (4 << __nprivate__::SizeShift),
            Murmur64     = (7 << __nprivate__::IndexShift) | (8 << __nprivate__::SizeShift),
            XXHash64     = (8 << __nprivate__::IndexShift) | (8 << __nprivate__::SizeShift),
            SpookyHashV2 = (9 << __nprivate__::IndexShift) | (16 << __nprivate__::SizeShift),
        };
        u32 value;
    };

    typedef void* hash_instance_t;

    hash_instance_t create_hash(alloc_t* allocator, ehashtype type);
    void            destroy_hash(alloc_t* allocator, hash_instance_t h);
    s32             hash_size(hash_instance_t ctxt);
    void            hash_begin(hash_instance_t ctxt);
    void            hash_update(hash_instance_t ctxt, const u8* begin, const u8* end);
    void            hash_end(hash_instance_t ctxt, u8* hash, s32 size);

} // namespace ncore

#endif
