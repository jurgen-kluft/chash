#include "ccore/c_target.h"
#include "ccore/c_allocator.h"

#include "chash/c_hash.h"
#include "chash/private/c_internal_hash.h"

namespace ncore
{
    static inline void ctxt_clear(hash_ctxt_t ctxt, u32 size, u16 type)
    {
        u8* data = (u8*)ctxt;
        for (u32 i = 0; i < size; ++i)
            data[i] = 0;

        hash_header_t* hdr = (hash_header_t*)ctxt;
        hdr->type = type;
    }

    hash_ctxt_t create_hash_ctxt(alloc_t* allocator, EHash type)
    {
        hash_ctxt_t ctxt;
        switch (type.value)
        {
            case EHash::MD5: ctxt = allocator->allocate(sizeof(md5_t)); ctxt_clear(ctxt, sizeof(md5_t), EHash::MD5); break;
            case EHash::SHA1: ctxt = allocator->allocate(sizeof(sha1_t)); ctxt_clear(ctxt, sizeof(sha1_t), EHash::SHA1); break;
            case EHash::Skein256: ctxt = allocator->allocate(sizeof(skein256_t)); ctxt_clear(ctxt, sizeof(skein256_t), EHash::Skein256); break;
            case EHash::Skein512: ctxt = allocator->allocate(sizeof(skein512_t)); ctxt_clear(ctxt, sizeof(skein512_t), EHash::Skein512); break;
            case EHash::Skein1024: ctxt = allocator->allocate(sizeof(skein1024_t)); ctxt_clear(ctxt, sizeof(skein1024_t), EHash::Skein1024); break;
            case EHash::Murmur32: ctxt = allocator->allocate(sizeof(murmur32_t)); ctxt_clear(ctxt, sizeof(murmur32_t), EHash::Murmur32); break;
            case EHash::Murmur64: ctxt = allocator->allocate(sizeof(murmur64_t)); ctxt_clear(ctxt, sizeof(murmur64_t), EHash::Murmur64); break;
            case EHash::XXHash64: ctxt = allocator->allocate(sizeof(xxhash64_t)); ctxt_clear(ctxt, sizeof(xxhash64_t), EHash::XXHash64); break;
            default: return nullptr;
        }
        return ctxt;
    }

    void destroy_hash_ctxt(alloc_t* allocator, hash_ctxt_t ctxt)
    {
        hash_header_t* hash = (hash_header_t*)ctxt;
        switch (hash->type)
        {
            case EHash::MD5: allocator->destruct<md5_t>((md5_t*)ctxt);
            case EHash::SHA1: allocator->destruct<sha1_t>((sha1_t*)ctxt);
            case EHash::Skein256: allocator->destruct<skein256_t>((skein256_t*)ctxt);
            case EHash::Skein512: allocator->destruct<skein512_t>((skein512_t*)ctxt);
            case EHash::Skein1024: allocator->destruct<skein1024_t>((skein1024_t*)ctxt);
            case EHash::Murmur32: allocator->destruct<murmur32_t>((murmur32_t*)ctxt);
            case EHash::Murmur64: allocator->destruct<murmur64_t>((murmur64_t*)ctxt);
            case EHash::XXHash64: allocator->destruct<xxhash64_t>((xxhash64_t*)ctxt);
        }
    }

    s32 hash_size(EHash type) { return (s32)(type.value & EHash::SizeMask); }

    void hash_begin(hash_ctxt_t ctxt)
    {
        hash_header_t* hash = (hash_header_t*)ctxt;
        switch (hash->type)
        {
            case EHash::MD5: ((md5_t*)ctxt)->reset(); break;
        }
    }

    void hash_update(hash_ctxt_t ctxt, const u8* data, s32 size)
    {
        hash_header_t* hash = (hash_header_t*)ctxt;
        switch (hash->type)
        {
            case EHash::MD5: ((md5_t*)ctxt)->hash(data, size); break;
        }
    }

    void hash_end(hash_ctxt_t ctxt, u8* out_hash, s32 size)
    {
        hash_header_t* hash = (hash_header_t*)ctxt;
        switch (hash->type)
        {
            case EHash::MD5: ((md5_t*)ctxt)->end(out_hash); break;
        }
    }

} // namespace ncore
