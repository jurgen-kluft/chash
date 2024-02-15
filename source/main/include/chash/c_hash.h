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

    typedef void* hashtype_t;

    hashtype_t create_hash(alloc_t* allocator, EHash type);
    void       destroy_hash(alloc_t* allocator, hashtype_t ctxt);
    s32        hash_size(hashtype_t ctxt);
    void       hash_begin(hashtype_t ctxt);
    void       hash_update(hashtype_t ctxt, const u8* begin, const u8* end);
    void       hash_end(hashtype_t ctxt, u8* hash, s32 size);

} // namespace ncore

#endif
