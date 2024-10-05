#include "ccore/c_target.h"
#include "ccore/c_allocator.h"

#include "chash/c_hash.h"
#include "chash/private/c_internal_hash.h"

namespace ncore
{
    static inline void ctxt_clear(hash_instance_t ctxt, u32 size, u16 type)
    {
        u8* data = (u8*)ctxt;
        for (u32 i = 0; i < size; ++i)
            data[i] = 0;

        hash_header_t* hdr = (hash_header_t*)ctxt;
        hdr->type          = type;
    }

    // clang-format off
    static u32 s_hash_type_size[] = {
        sizeof(md5_t),
        sizeof(sha1_t),
        sizeof(skein256_t),
        sizeof(skein512_t),
        sizeof(skein1024_t),
        sizeof(murmur32_t),
        sizeof(murmur64_t),
        sizeof(xxhash64_t),
        sizeof(spookyhashv2_t)
    };
    // clang-format on

    hash_instance_t create_hash_ctxt(alloc_t* allocator, ehashtype type)
    {
        u32 const size = s_hash_type_size[(type.value & __nprivate__::IndexMask) >> __nprivate__::IndexShift];
        hash_instance_t    ctxt = allocator->allocate(size);
        ctxt_clear(ctxt, size, type.value);
        return ctxt;
    }

    void destroy_hash_ctxt(alloc_t* allocator, hash_instance_t ctxt) { allocator->deallocate(ctxt); }

    s32 hash_size(ehashtype type) { return (s32)((type.value & __nprivate__::SizeMask) >> __nprivate__::SizeShift); }
    s32 hash_size(hash_instance_t ctxt)
    {
        hash_header_t* h = (hash_header_t*)ctxt;
        return (h->type & __nprivate__::SizeMask) >> __nprivate__::SizeShift;
    }

    void hash_begin(hash_instance_t ctxt)
    {
        hash_header_t* hash = (hash_header_t*)ctxt;
        switch (hash->type)
        {
            case ehashtype::MD5: ((md5_t*)ctxt)->reset(); break;
            case ehashtype::SHA1: ((sha1_t*)ctxt)->reset(); break;
            case ehashtype::Skein256: ((skein256_t*)ctxt)->reset(); break;
            case ehashtype::Skein512: ((skein512_t*)ctxt)->reset(); break;
            case ehashtype::Skein1024: ((skein1024_t*)ctxt)->reset(); break;
            case ehashtype::Murmur32: ((murmur32_t*)ctxt)->reset(); break;
            case ehashtype::Murmur64: ((murmur64_t*)ctxt)->reset(); break;
            case ehashtype::XXHash64: ((xxhash64_t*)ctxt)->reset(); break;
            case ehashtype::SpookyHashV2: ((spookyhashv2_t*)ctxt)->reset(); break;
        }
    }

    void hash_update(hash_instance_t ctxt, const u8* begin, const u8* end)
    {
        hash_header_t* hash = (hash_header_t*)ctxt;
        switch (hash->type)
        {
            case ehashtype::MD5: ((md5_t*)ctxt)->hash(begin, end); break;
            case ehashtype::SHA1: ((sha1_t*)ctxt)->hash(begin, end); break;
            case ehashtype::Skein256: ((skein256_t*)ctxt)->hash(begin, end); break;
            case ehashtype::Skein512: ((skein512_t*)ctxt)->hash(begin, end); break;
            case ehashtype::Skein1024: ((skein1024_t*)ctxt)->hash(begin, end); break;
            case ehashtype::Murmur32: ((murmur32_t*)ctxt)->hash(begin, end); break;
            case ehashtype::Murmur64: ((murmur64_t*)ctxt)->hash(begin, end); break;
            case ehashtype::XXHash64: ((xxhash64_t*)ctxt)->hash(begin, end); break;
            case ehashtype::SpookyHashV2: ((spookyhashv2_t*)ctxt)->hash(begin, end); break;
        }
    }

    void hash_end(hash_instance_t ctxt, u8* out_hash, s32 size)
    {
        hash_header_t* hash = (hash_header_t*)ctxt;
        switch (hash->type)
        {
            case ehashtype::MD5: ((md5_t*)ctxt)->end(out_hash); break;
            case ehashtype::SHA1: ((sha1_t*)ctxt)->end(out_hash); break;
            case ehashtype::Skein256: ((skein256_t*)ctxt)->end(out_hash); break;
            case ehashtype::Skein512: ((skein512_t*)ctxt)->end(out_hash); break;
            case ehashtype::Skein1024: ((skein1024_t*)ctxt)->end(out_hash); break;
            case ehashtype::Murmur32: ((murmur32_t*)ctxt)->end(out_hash); break;
            case ehashtype::Murmur64: ((murmur64_t*)ctxt)->end(out_hash); break;
            case ehashtype::XXHash64: ((xxhash64_t*)ctxt)->end(out_hash); break;
            case ehashtype::SpookyHashV2: ((spookyhashv2_t*)ctxt)->end(out_hash); break;
        }
    }

} // namespace ncore
