#include "ccore/c_target.h"
#include "ccore/c_allocator.h"

#include "chash/c_hash.h"
#include "chash/private/c_internal_hash.h"

namespace ncore
{
    static inline void ctxt_clear(hash_t ctxt, u32 size, u16 type)
    {
        u8* data = (u8*)ctxt;
        for (u32 i = 0; i < size; ++i)
            data[i] = 0;

        hash_header_t* hdr = (hash_header_t*)ctxt;
        hdr->type          = type;
    }

    static u32 s_hash_type_size[] = {sizeof(md5_t), sizeof(sha1_t), sizeof(skein256_t), sizeof(skein512_t), sizeof(skein1024_t), sizeof(murmur32_t), sizeof(murmur64_t), sizeof(xxhash64_t)};
    hash_t     create_hash_ctxt(alloc_t* allocator, hash_type_t type)
    {
        u32 const size = s_hash_type_size[(type.value & hash_type_t::IndexMask) >> hash_type_t::IndexShift];
        hash_t    ctxt = allocator->allocate(size);
        ctxt_clear(ctxt, size, type.value);
        return ctxt;
    }

    void destroy_hash_ctxt(alloc_t* allocator, hash_t ctxt)
    {
        allocator->deallocate(ctxt);
    }

    s32 hash_size(hash_type_t type) { return (s32)((type.value & hash_type_t::SizeMask) >> hash_type_t::SizeShift); }
    s32 hash_size(hash_t ctxt)
    {
        hash_header_t* h = (hash_header_t*)ctxt;
        return (h->type & hash_type_t::SizeMask) >> hash_type_t::SizeShift;
    }

    void hash_begin(hash_t ctxt)
    {
        hash_header_t* hash = (hash_header_t*)ctxt;
        switch (hash->type)
        {
            case hash_type_t::MD5: ((md5_t*)ctxt)->reset(); break;
        }
    }

    void hash_update(hash_t ctxt, const u8* begin, const u8* end)
    {
        hash_header_t* hash = (hash_header_t*)ctxt;
        switch (hash->type)
        {
            case hash_type_t::MD5: ((md5_t*)ctxt)->hash(begin, end); break;
        }
    }

    void hash_end(hash_t ctxt, u8* out_hash, s32 size)
    {
        hash_header_t* hash = (hash_header_t*)ctxt;
        switch (hash->type)
        {
            case hash_type_t::MD5: ((md5_t*)ctxt)->end(out_hash); break;
        }
    }

} // namespace ncore
