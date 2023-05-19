#ifndef __CHASH_HASH_H__
#define __CHASH_HASH_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    class alloc_t;

    struct EHash
    {
        enum
        {
            MD5       = (1 << 12) | 16,
            SHA1      = (2 << 12) | 20,
            Skein256  = (3 << 12) | 32,
            Skein512  = (4 << 12) | 64,
            Skein1024 = (5 << 12) | 128,
            Murmur32  = (6 << 12) | 4,
            Murmur64  = (7 << 12) | 8,
            XXHash64  = (8 << 12) | 8,
            SizeMask  = 0x0FFF,
        };
        u16 value;
    };
    typedef void* hash_ctxt_t;
    hash_ctxt_t   create_hash_ctxt(alloc_t* allocator, EHash type);
    void          destroy_hash_ctxt(alloc_t* allocator, hash_ctxt_t ctxt);
    s32           hash_size(EHash type);

    void hash_begin(hash_ctxt_t ctxt);
    void hash_update(hash_ctxt_t ctxt, const u8* data, s32 size);
    void hash_end(hash_ctxt_t ctxt, u8* hash, s32 size);

} // namespace ncore

#endif
