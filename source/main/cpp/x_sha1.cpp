#include "xbase/x_target.h"
#include "xbase/x_va_list.h"
#include "xbase/x_integer.h"
#include "xbase/x_memory.h"
#include "xbase/x_endian.h"

#include "xhash/x_hash.h"

namespace ncore
{
    /**
     *  URL:
     *      http: * en.wikipedia.org/wiki/SHA-1
     *
     *  Description:
     *      This class implements the Secure Hashing Standard as defined
     *      in FIPS PUB 180-1 published April 17, 1995.
     *
     *      The Secure Hashing Standard, which uses the Secure Hashing
     *      Algorithm (SHA), produces a 160-bit message digest for a
     *      given data stream.  In theory, it is highly improbable that
     *      two messages will produce the same message digest.  Therefore,
     *      this algorithm can serve as a means of providing a "fingerprint"
     *      for a message.
     *
     *  Portability Issues:
     *      SHA-1 is defined in terms of 32-bit "words".  This code was
     *      written with the expectation that the processor has at least
     *      a 32-bit machine word size.  If the machine word size is larger,
     *      the code should still function properly.  One caveat to that
     *      is that the input functions taking characters and character arrays
     *      assume that only 8 bits of information are stored in each character.
     *
     *  Caveats:
     *      SHA-1 is designed to work with messages less than 2^64 bits long.
     *      Although SHA-1 allows a message digest to be generated for
     *      messages of any number of bits less than 2^64, this implementation
     *      only works with messages with a length that is a multiple of 8
     *      bits.
     *
     *  Code: From the implementation of Git
     */

    struct xsha1_ctx
    {
        u64 size;
        u32 H[5];
        u32 W[16];
        u32 computed;
    };

#define SHA_ROT(X, l, r) (((X) << (l)) | ((X) >> (r)))
#define SHA_ROL(X, n) SHA_ROT(X, n, 32 - (n))
#define SHA_ROR(X, n) SHA_ROT(X, 32 - (n), n)

#ifdef X_LITTLE_ENDIAN
#define xntohl(x) ((((u32)(x)&0xff000000) >> 24) | (((u32)(x)&0x00ff0000) >> 8) | (((u32)(x)&0x0000ff00) << 8) | (((u32)(x)&0x000000ff) << 24))

#define xhtonl(x) xntohl(x)
#else
#define xntohl(x) (x)
#define xhtonl(x) (x)
#endif

#define get_be32(p) xntohl(*(u32*)(p))
#define put_be32(p, v)          \
    do                          \
    {                           \
        *(u32*)(p) = xhtonl(v); \
    } while (0)

// This "rolls" over the 512-bit array
#define W(x) (array[(x)&15])
#define setW(x, val) (W(x) = (val))

/**
 *  Where do we get the source from? The first 16 iterations get it from
 *  the input data, the next mix it from the 512-bit array.
 */
#define SHA_SRC(t) get_be32(data + t)
#define SHA_MIX(t) SHA_ROL(W(t + 13) ^ W(t + 8) ^ W(t + 2) ^ W(t), 1)

#define SHA_ROUND(t, input, fn, constant, A, B, C, D, E) \
    do                                                   \
    {                                                    \
        u32 TEMP = input(t);                             \
        setW(t, TEMP);                                   \
        E += TEMP + SHA_ROL(A, 5) + (fn) + (constant);   \
        B = SHA_ROR(B, 2);                               \
    } while (0)

#define T_0_15(t, A, B, C, D, E) SHA_ROUND(t, SHA_SRC, (((C ^ D) & B) ^ D), 0x5a827999, A, B, C, D, E)
#define T_16_19(t, A, B, C, D, E) SHA_ROUND(t, SHA_MIX, (((C ^ D) & B) ^ D), 0x5a827999, A, B, C, D, E)
#define T_20_39(t, A, B, C, D, E) SHA_ROUND(t, SHA_MIX, (B ^ C ^ D), 0x6ed9eba1, A, B, C, D, E)
#define T_40_59(t, A, B, C, D, E) SHA_ROUND(t, SHA_MIX, ((B & C) + (D & (B ^ C))), 0x8f1bbcdc, A, B, C, D, E)
#define T_60_79(t, A, B, C, D, E) SHA_ROUND(t, SHA_MIX, (B ^ C ^ D), 0xca62c1d6, A, B, C, D, E)

    void xsha1_ctx_init(xsha1_ctx* ctx)
    {
        ctx->size     = 0;
        ctx->computed = 0;

        // Initialize H with the magic constants (see FIPS180 for constants)
        ctx->H[0] = 0x67452301;
        ctx->H[1] = 0xefcdab89;
        ctx->H[2] = 0x98badcfe;
        ctx->H[3] = 0x10325476;
        ctx->H[4] = 0xc3d2e1f0;
    }

    void xsha1_ctx_block(xsha1_ctx* ctx, const u32* data)
    {
        u32 array[16];

        u32 A = ctx->H[0];
        u32 B = ctx->H[1];
        u32 C = ctx->H[2];
        u32 D = ctx->H[3];
        u32 E = ctx->H[4];

        // Round 1 - iterations 0-16 take their input from 'data'
        T_0_15(0, A, B, C, D, E);
        T_0_15(1, E, A, B, C, D);
        T_0_15(2, D, E, A, B, C);
        T_0_15(3, C, D, E, A, B);
        T_0_15(4, B, C, D, E, A);
        T_0_15(5, A, B, C, D, E);
        T_0_15(6, E, A, B, C, D);
        T_0_15(7, D, E, A, B, C);
        T_0_15(8, C, D, E, A, B);
        T_0_15(9, B, C, D, E, A);
        T_0_15(10, A, B, C, D, E);
        T_0_15(11, E, A, B, C, D);
        T_0_15(12, D, E, A, B, C);
        T_0_15(13, C, D, E, A, B);
        T_0_15(14, B, C, D, E, A);
        T_0_15(15, A, B, C, D, E);

        // Round 1 - tail. Input from 512-bit mixing array
        T_16_19(16, E, A, B, C, D);
        T_16_19(17, D, E, A, B, C);
        T_16_19(18, C, D, E, A, B);
        T_16_19(19, B, C, D, E, A);

        // Round 2
        T_20_39(20, A, B, C, D, E);
        T_20_39(21, E, A, B, C, D);
        T_20_39(22, D, E, A, B, C);
        T_20_39(23, C, D, E, A, B);
        T_20_39(24, B, C, D, E, A);
        T_20_39(25, A, B, C, D, E);
        T_20_39(26, E, A, B, C, D);
        T_20_39(27, D, E, A, B, C);
        T_20_39(28, C, D, E, A, B);
        T_20_39(29, B, C, D, E, A);
        T_20_39(30, A, B, C, D, E);
        T_20_39(31, E, A, B, C, D);
        T_20_39(32, D, E, A, B, C);
        T_20_39(33, C, D, E, A, B);
        T_20_39(34, B, C, D, E, A);
        T_20_39(35, A, B, C, D, E);
        T_20_39(36, E, A, B, C, D);
        T_20_39(37, D, E, A, B, C);
        T_20_39(38, C, D, E, A, B);
        T_20_39(39, B, C, D, E, A);

        // Round 3
        T_40_59(40, A, B, C, D, E);
        T_40_59(41, E, A, B, C, D);
        T_40_59(42, D, E, A, B, C);
        T_40_59(43, C, D, E, A, B);
        T_40_59(44, B, C, D, E, A);
        T_40_59(45, A, B, C, D, E);
        T_40_59(46, E, A, B, C, D);
        T_40_59(47, D, E, A, B, C);
        T_40_59(48, C, D, E, A, B);
        T_40_59(49, B, C, D, E, A);
        T_40_59(50, A, B, C, D, E);
        T_40_59(51, E, A, B, C, D);
        T_40_59(52, D, E, A, B, C);
        T_40_59(53, C, D, E, A, B);
        T_40_59(54, B, C, D, E, A);
        T_40_59(55, A, B, C, D, E);
        T_40_59(56, E, A, B, C, D);
        T_40_59(57, D, E, A, B, C);
        T_40_59(58, C, D, E, A, B);
        T_40_59(59, B, C, D, E, A);

        // Round 4
        T_60_79(60, A, B, C, D, E);
        T_60_79(61, E, A, B, C, D);
        T_60_79(62, D, E, A, B, C);
        T_60_79(63, C, D, E, A, B);
        T_60_79(64, B, C, D, E, A);
        T_60_79(65, A, B, C, D, E);
        T_60_79(66, E, A, B, C, D);
        T_60_79(67, D, E, A, B, C);
        T_60_79(68, C, D, E, A, B);
        T_60_79(69, B, C, D, E, A);
        T_60_79(70, A, B, C, D, E);
        T_60_79(71, E, A, B, C, D);
        T_60_79(72, D, E, A, B, C);
        T_60_79(73, C, D, E, A, B);
        T_60_79(74, B, C, D, E, A);
        T_60_79(75, A, B, C, D, E);
        T_60_79(76, E, A, B, C, D);
        T_60_79(77, D, E, A, B, C);
        T_60_79(78, C, D, E, A, B);
        T_60_79(79, B, C, D, E, A);

        ctx->H[0] += A;
        ctx->H[1] += B;
        ctx->H[2] += C;
        ctx->H[3] += D;
        ctx->H[4] += E;
    }

    void xsha1_ctx_update(xsha1_ctx* ctx, cbuffer_t const& buffer)
    {
        u32 lenW = ctx->size & 63;

        u32          len  = (u32)buffer.size();
        u8 const* data = buffer.m_const;
        ctx->size += len;

        // Read the data into W and process blocks as they get full
        if (lenW)
        {
            u32 left = 64 - lenW;
            if (len < left)
                left = len;
            x_memcpy(lenW + (char*)ctx->W, data, left);
            lenW = (lenW + left) & 63;
            len -= left;
            data = ((const u8*)data + left);
            if (lenW)
                return;
            xsha1_ctx_block(ctx, ctx->W);
        }
        while (len >= 64)
        {
            xsha1_ctx_block(ctx, (u32 const*)data);
            data = ((const u8*)data + 64);
            len -= 64;
        }
        if (len)
            x_memcpy(ctx->W, data, len);
    }

    static const u8 xsha1_ctx_pad[64] = {0x80};
    void            xsha1_ctx_close(xsha1_ctx* ctx)
    {
        // Pad with a binary 1 (ie 0x80), then zeroes, then length
        u32 padlen[2];
        padlen[0] = xhtonl((u32)(ctx->size >> 29));
        padlen[1] = xhtonl((u32)(ctx->size << 3));

        s32 i = ctx->size & 63;
        // xsha1_ctx_update(ctx, pad, 1 + (63 & (55 - i)));
        xsha1_ctx_update(ctx, cbuffer_t(1 + (63 & (55 - i)), xsha1_ctx_pad));
        xsha1_ctx_update(ctx, cbuffer_t(8, (u8 const*)padlen));
    }

    sha1_t::sha1_t()
    {
        xsha1_ctx* ctx = (xsha1_ctx*)&this->m_ctxt;
        xsha1_ctx_init(ctx);
    }

    void sha1_t::reset()
    {
        xsha1_ctx* ctx = (xsha1_ctx*)&this->m_ctxt;
        xsha1_ctx_init(ctx);
    }

    void sha1_t::hash(cbuffer_t const& _buffer)
    {
        xsha1_ctx* ctx = (xsha1_ctx*)&this->m_ctxt;
        xsha1_ctx_update(ctx, _buffer);
    }

    inline void to_bytes(buffer_t& bytes, u32 p)
    {
        p                   = x_NetworkEndian::swap(p);
        u8 const*    src = (u8 const*)&p;
        binary_writer_t writer(bytes);
        writer.write(*src++);
        writer.write(*src++);
        writer.write(*src++);
        writer.write(*src++);
    }

    void sha1_t::end(xdigest::sha1& _hash)
    {
        xsha1_ctx* ctx = (xsha1_ctx*)&this->m_ctxt;
        if (ctx->computed == 0)
        {
            xsha1_ctx_close(ctx);
            ctx->computed = 1;
        }

        u32      idx = 0;
        buffer_t h   = _hash.buffer();
        for (s32 i = 0; i < 5; ++i)
            to_bytes(h, ctx->H[i]);
    }

    void sha1_t::compute(cbuffer_t const& data, xdigest::sha1& hash)
    {
        reset();
        compute(data);
        end(hash);
    }

    xdigest::sha1 sha1_t::compute(cbuffer_t const& data)
    {
        xdigest::sha1 hash;
        compute(data, hash);
        return hash;
    }

} // namespace ncore
