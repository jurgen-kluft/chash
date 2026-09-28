#include "ccore/c_target.h"
#include "ccore/c_memory.h"

#include "chash/private/c_internal_hash.h"

namespace ncore
{
    namespace nhash
    {
        // 128bit multiply function
        static inline u64 _wyrot(u64 x) { return (x >> 32) | (x << 32); }

        static inline void _wymum(u64* A, u64* B)
        {
            u64 ha = *A >> 32, hb = *B >> 32, la = (u32)*A, lb = (u32)*B, hi, lo;
            u64 rh = ha * hb, rm0 = ha * lb, rm1 = hb * la, rl = la * lb, t = rl + (rm0 << 32), c = t < rl;
            lo = t + (rm1 << 32);
            c += lo < t;
            hi = rh + (rm0 >> 32) + (rm1 >> 32) + c;
            *A ^= lo;
            *B ^= hi;
        }

        // multiply and xor mix function, aka MUM
        static inline u64 _wymix(u64 A, u64 B)
        {
            _wymum(&A, &B);
            return A ^ B;
        }

// read functions
#ifdef D_LITTLE_ENDIAN
        static inline u64 _wyr8(const u8* p)
        {
            u64 v;
            g_memcpy(&v, p, 8);
            return v;
        }
        static inline u64 _wyr4(const u8* p)
        {
            u32 v;
            g_memcpy(&v, p, 4);
            return v;
        }
#else
        static inline u64 _wyr8(const u8* p)
        {
            u64 v;
            g_memcpy(&v, p, 8);
            return (((v >> 56) & 0xff) | ((v >> 40) & 0xff00) | ((v >> 24) & 0xff0000) | ((v >> 8) & 0xff000000) | ((v << 8) & 0xff00000000) | ((v << 24) & 0xff0000000000) | ((v << 40) & 0xff000000000000) | ((v << 56) & 0xff00000000000000));
        }
        static inline u64 _wyr4(const u8* p)
        {
            u32 v;
            g_memcpy(&v, p, 4);
            return (((v >> 24) & 0xff) | ((v >> 8) & 0xff00) | ((v << 8) & 0xff0000) | ((v << 24) & 0xff000000));
        }
#endif
        static inline u64 _wyr3(const u8* p, uint_t k) { return (((u64)p[0]) << 16) | (((u64)p[k >> 1]) << 8) | p[k - 1]; }

#define _likely_(a) (a)
#define _unlikely_(a) (a)

        // wyhash main function
        static inline u64 wyhash(const void* key, uint_t len, u64 seed, const u64* secret)
        {
            const u8* p = (const u8*)key;
            seed ^= *secret;
            u64 a, b;
            if (_likely_(len <= 16))
            {
                if (_likely_(len >= 4))
                {
                    a = (_wyr4(p) << 32) | _wyr4(p + ((len >> 3) << 2));
                    b = (_wyr4(p + len - 4) << 32) | _wyr4(p + len - 4 - ((len >> 3) << 2));
                }
                else if (_likely_(len > 0))
                {
                    a = _wyr3(p, len);
                    b = 0;
                }
                else
                    a = b = 0;
            }
            else
            {
                uint_t i = len;
                if (_unlikely_(i > 48))
                {
                    u64 see1 = seed, see2 = seed;
                    do
                    {
                        seed = _wymix(_wyr8(p) ^ secret[1], _wyr8(p + 8) ^ seed);
                        see1 = _wymix(_wyr8(p + 16) ^ secret[2], _wyr8(p + 24) ^ see1);
                        see2 = _wymix(_wyr8(p + 32) ^ secret[3], _wyr8(p + 40) ^ see2);
                        p += 48;
                        i -= 48;
                    } while (_likely_(i > 48));
                    seed ^= see1 ^ see2;
                }
                while (_unlikely_(i > 16))
                {
                    seed = _wymix(_wyr8(p) ^ secret[1], _wyr8(p + 8) ^ seed);
                    i -= 16;
                    p += 16;
                }
                a = _wyr8(p + i - 16);
                b = _wyr8(p + i - 8);
            }
            return _wymix(secret[1] ^ len, _wymix(a ^ secret[1], b ^ seed));
        }

        // The default secret used by standard wyhash
        static const u64 _wyp[4] = {0xa0761d6478bd642fULL, 0xe7037ed1a0b428dbULL, 0x8ebc6af09c88c6e3ULL, 0x589965cc75374cc3ULL};

        struct stream_t
        {
            u64 see1;       // Primary internal hash state
            u64 see2;       // Secondary internal hash state
            u64 total_len;  // Accumulated total size of data processed
            u8  buffer[48]; // Internal buffer to cache incomplete chunks
            u8  buf_len;    // Current number of bytes stored in the buffer
        };

        // Initialize state before streaming blocks
        void stream_init(stream_t* ctx, u64 seed)
        {
            ctx->see1      = seed ^ _wyp[0];
            ctx->see2      = seed ^ _wyp[1];
            ctx->total_len = 0;
            ctx->buf_len   = 0;
            g_memset(ctx->buffer, 0, 48);
        }

        void stream_update(stream_t* ctx, const void* data, size_t len)
        {
            if (len == 0)
                return;

            const u8* p = (const u8*)data;
            ctx->total_len += len;

            // 1. Process data remaining in the trailing buffer
            if (ctx->buf_len > 0)
            {
                size_t need = 48 - ctx->buf_len;
                if (len < need)
                {
                    g_memcpy(ctx->buffer + ctx->buf_len, p, len);
                    ctx->buf_len += (u8)len;
                    return;
                }

                g_memcpy(ctx->buffer + ctx->buf_len, p, need);
                p += need;
                len -= need;

                // Corrected using value assignment with _wymix instead of mutating pointers
                ctx->total_len = _wymix(_wyr8(ctx->buffer) ^ _wyp[1], _wyr8(ctx->buffer + 8) ^ ctx->total_len);
                ctx->see1      = _wymix(_wyr8(ctx->buffer + 16) ^ _wyp[2], _wyr8(ctx->buffer + 24) ^ ctx->see1);
                ctx->see2      = _wymix(_wyr8(ctx->buffer + 32) ^ _wyp[3], _wyr8(ctx->buffer + 40) ^ ctx->see2);

                ctx->buf_len = 0;
            }

            // 2. Main processing loop for large chunk streaming (e.g., 64 KiB blocks)
            while (len >= 48)
            {
                ctx->total_len = _wymix(_wyr8(p) ^ _wyp[1], _wyr8(p + 8) ^ ctx->total_len);
                ctx->see1      = _wymix(_wyr8(p + 16) ^ _wyp[2], _wyr8(p + 24) ^ ctx->see1);
                ctx->see2      = _wymix(_wyr8(p + 32) ^ _wyp[3], _wyr8(p + 40) ^ ctx->see2);

                p += 48;
                len -= 48;
            }

            // 3. Keep residual trailing bytes
            if (len > 0)
            {
                g_memcpy(ctx->buffer, p, len);
                ctx->buf_len = (u8)len;
            }
        }

        void stream_finalize(stream_t* ctx, u8* out_hash)
        {
            u8        len = ctx->buf_len;
            const u8* p   = ctx->buffer;
            u64       a, b;

            // Combine intermediate parallelized hashes back into the primary state
            u64 seed = ctx->total_len ^ ctx->see1 ^ ctx->see2;

            // Process final remaining bits (< 48 bytes) matching official end-of-string styles
            if (len > 16)
            {
                seed = _wymix(_wyr8(p) ^ _wyp[1], _wyr8(p + 8) ^ seed);

                if (len > 32)
                {
                    seed = _wymix(_wyr8(p + 16) ^ _wyp[2], _wyr8(p + 24) ^ seed);
                    a    = _wyr8(p + len - 16);
                    b    = _wyr8(p + len - 8);
                }
                else
                {
                    a = _wyr8(p + len - 16);
                    b = _wyr8(p + len - 8);
                }
            }
            else if (len > 0)
            {
                if (len >= 4)
                {
                    a = ((u64)_wyr4(p) << 32) | _wyr4(p + ((len >> 3) << 2));
                    b = ((u64)_wyr4(p + len - 4) << 32) | _wyr4(p + len - 4 - ((len >> 3) << 2));
                }
                else
                {
                    a = _wyr3(p, len);
                    b = 0;
                }
            }
            else
            {
                a = b = 0;
            }

            // Combine trailing blocks with length parameters
            const u64 final_hash = _wymix(_wyp[1] ^ ctx->total_len, _wymix(a ^ _wyp[1], b ^ seed));
            g_memcpy(out_hash, &final_hash, sizeof(u64));
        }
    } // namespace nhash

    namespace nhash
    {
        void wyhash64_t::reset(u64 seed)
        {
            nhash::stream_t* ctx = (nhash::stream_t*)&this->m_ctxt;
            m_seed               = seed;
            nhash::stream_init(ctx, m_seed);
        }

        void wyhash64_t::hash(const u8* begin, const u8* end)
        {
            nhash::stream_t* ctx = (nhash::stream_t*)&this->m_ctxt;
            nhash::stream_update(ctx, begin, (size_t)(end - begin));
        }

        void wyhash64_t::end(u8* out_hash)
        {
            nhash::stream_t* ctx = (nhash::stream_t*)&this->m_ctxt;
            nhash::stream_finalize(ctx, out_hash);
        }
    } // namespace nhash
} // namespace ncore
