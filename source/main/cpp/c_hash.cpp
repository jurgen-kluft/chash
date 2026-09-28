#include "ccore/c_target.h"
#include "ccore/c_allocator.h"

#include "chash/c_hash.h"
#include "chash/private/c_internal_hash.h"

namespace ncore
{
    using namespace nhash;

    static inline void ctxt_clear(hash_instance_t ctxt, u32 size, u32 type)
    {
        u8* data = (u8*)ctxt;
        for (u32 i = 0; i < size; ++i)
            data[i] = 0;

        hash_header_t* hdr = (hash_header_t*)ctxt;
        hdr->type          = type;
    }

    hash_instance_t create_hash(void* memory, nhash::type_t type)
    {
        u32 const       size = (type & nhash::CtxSizeMask) >> nhash::CtxSizeShift;
        hash_instance_t ctxt = memory;
        ctxt_clear(ctxt, size, type);
        return ctxt;
    }

    void destroy_hash(void* memory, hash_instance_t ctxt) {}

    s32 hash_size(nhash::type_t type) { return (s32)((type & nhash::SizeMask) >> nhash::SizeShift); }
    s32 hash_size(hash_instance_t ctxt)
    {
        hash_header_t* h = (hash_header_t*)ctxt;
        return (h->type & nhash::SizeMask) >> nhash::SizeShift;
    }

    void hash_begin(hash_instance_t ctxt)
    {
        hash_header_t* hash = (hash_header_t*)ctxt;
        switch (hash->type)
        {
            case nhash::MD5: ((md5_t*)ctxt)->reset(); break;
            case nhash::SHA1: ((sha1_t*)ctxt)->reset(); break;
            case nhash::SHA256: ((sha256_t*)ctxt)->reset(); break;
            case nhash::Skein256: ((skein256_t*)ctxt)->reset(); break;
            case nhash::Skein512: ((skein512_t*)ctxt)->reset(); break;
            case nhash::Skein1024: ((skein1024_t*)ctxt)->reset(); break;
            case nhash::Murmur32: ((murmur32_t*)ctxt)->reset(); break;
            case nhash::Murmur64: ((murmur64_t*)ctxt)->reset(); break;
            case nhash::XXHash64: ((xxhash64_t*)ctxt)->reset(); break;
            case nhash::SpookyHashV2: ((spookyhashv2_t*)ctxt)->reset(); break;
        }
    }

    void hash_update(hash_instance_t ctxt, const u8* begin, const u8* end)
    {
        hash_header_t* hash = (hash_header_t*)ctxt;
        switch (hash->type)
        {
            case nhash::MD5: ((md5_t*)ctxt)->hash(begin, end); break;
            case nhash::SHA1: ((sha1_t*)ctxt)->hash(begin, end); break;
            case nhash::SHA256: ((sha256_t*)ctxt)->hash(begin, end); break;
            case nhash::Skein256: ((skein256_t*)ctxt)->hash(begin, end); break;
            case nhash::Skein512: ((skein512_t*)ctxt)->hash(begin, end); break;
            case nhash::Skein1024: ((skein1024_t*)ctxt)->hash(begin, end); break;
            case nhash::Murmur32: ((murmur32_t*)ctxt)->hash(begin, end); break;
            case nhash::Murmur64: ((murmur64_t*)ctxt)->hash(begin, end); break;
            case nhash::XXHash64: ((xxhash64_t*)ctxt)->hash(begin, end); break;
            case nhash::SpookyHashV2: ((spookyhashv2_t*)ctxt)->hash(begin, end); break;
        }
    }

    void hash_end(hash_instance_t ctxt, u8* out_hash, s32 size)
    {
        hash_header_t* hash = (hash_header_t*)ctxt;
        switch (hash->type)
        {
            case nhash::MD5: ((md5_t*)ctxt)->end(out_hash); break;
            case nhash::SHA1: ((sha1_t*)ctxt)->end(out_hash); break;
            case nhash::SHA256: ((sha256_t*)ctxt)->end(out_hash); break;
            case nhash::Skein256: ((skein256_t*)ctxt)->end(out_hash); break;
            case nhash::Skein512: ((skein512_t*)ctxt)->end(out_hash); break;
            case nhash::Skein1024: ((skein1024_t*)ctxt)->end(out_hash); break;
            case nhash::Murmur32: ((murmur32_t*)ctxt)->end(out_hash); break;
            case nhash::Murmur64: ((murmur64_t*)ctxt)->end(out_hash); break;
            case nhash::XXHash64: ((xxhash64_t*)ctxt)->end(out_hash); break;
            case nhash::SpookyHashV2: ((spookyhashv2_t*)ctxt)->end(out_hash); break;
        }
    }

} // namespace ncore
