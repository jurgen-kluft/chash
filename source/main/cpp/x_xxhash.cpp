#include "xbase/x_target.h"
#include "xbase/x_va_list.h"
#include "xbase/x_integer.h"
#include "xbase/x_memory.h"
#include "xbase/x_endian.h"

#include "xhash/x_hash.h"

namespace xcore
{
	static constexpr u64 PRIME64_1 = 11400714785074694791ULL;
	static constexpr u64 PRIME64_2 = 14029467366897019727ULL;
	static constexpr u64 PRIME64_3 = 1609587929392839161ULL;
	static constexpr u64 PRIME64_4 = 9650029242287828579ULL;
	static constexpr u64 PRIME64_5 = 2870177450012600261ULL;

	struct xxhash64_ctxt
	{
		u64 m_total_len;
		u64 m_v1;
		u64 m_v2;
		u64 m_v3;
		u64 m_v4;
		u64 m_mem64[4];
		s32 m_memsize;

		void reset(unsigned long long seed)
		{
			m_total_len = 0;
			m_v1 = seed + PRIME64_1 + PRIME64_2;
			m_v2 = seed + PRIME64_2;
			m_v3 = seed + 0;
			m_v4 = seed - PRIME64_1;
			m_mem64[0] = 0;
			m_mem64[1] = 0;
			m_mem64[2] = 0;
			m_mem64[3] = 0;
			m_memsize = 0;
		}

#define XXH_rotl32(x, r) ((x << r) | (x >> (32 - r)))
#define XXH_rotl64(x, r) ((x << r) | (x >> (64 - r)))

#define XXH_COPY_MEMORY(dst, src, length)         \
	{                                             \
		xbyte *pdst = (xbyte *)(dst);             \
		xbyte const *psrc = (xbyte const *)(src); \
		for (s32 i = 0; i < (length); ++i)        \
		{                                         \
			pdst[i] = psrc[i];                    \
		}                                         \
	}

		static u32 read32bits(const void *memPtr)
		{
			xbyte const *bptr = (xbyte const *)memPtr;
			u32 val = bptr[0];
			val = val << 8 | bptr[1];
			val = val << 8 | bptr[2];
			val = val << 8 | bptr[3];
			return val;
		}

		static u64 read64bits(const void *memPtr)
		{
			xbyte const *bptr = (xbyte const *)memPtr;
			u64 val = bptr[0];
			val = val << 8 | bptr[1];
			val = val << 8 | bptr[2];
			val = val << 8 | bptr[3];
			val = val << 8 | bptr[4];
			val = val << 8 | bptr[5];
			val = val << 8 | bptr[6];
			val = val << 8 | bptr[7];
			return val;
		}

		static u64 round(u64 acc, u64 input)
		{
			acc += input * PRIME64_2;
			acc = XXH_rotl64(acc, 31);
			acc *= PRIME64_1;
			return acc;
		}

		static u64 mergeRound(u64 acc, u64 val)
		{
			val = round(0, val);
			acc ^= val;
			acc = acc * PRIME64_1 + PRIME64_4;
			return acc;
		}

		static u64 avalanche(u64 h64)
		{
			h64 ^= h64 >> 33;
			h64 *= PRIME64_2;
			h64 ^= h64 >> 29;
			h64 *= PRIME64_3;
			h64 ^= h64 >> 32;
			return h64;
		}

		void update(xcbuffer const &_buffer)
		{
			const s32 len = _buffer.size();
			const xbyte *p = (const xbyte *)_buffer.m_const;
			const xbyte *const bEnd = p + len;

			m_total_len += len;

			if (m_memsize + len < 32)
			{ /* fill in tmp buffer */
				XXH_COPY_MEMORY(((xbyte *)m_mem64) + m_memsize, p, len);
				m_memsize += (u32)len;
				return;
			}

			if (m_memsize)
			{ /* tmp buffer is full */
				XXH_COPY_MEMORY(((xbyte *)m_mem64) + m_memsize, p, 32 - m_memsize);
				m_v1 = round(m_v1, read64bits(m_mem64 + 0));
				m_v2 = round(m_v2, read64bits(m_mem64 + 1));
				m_v3 = round(m_v3, read64bits(m_mem64 + 2));
				m_v4 = round(m_v4, read64bits(m_mem64 + 3));
				p += 32 - m_memsize;
				m_memsize = 0;
			}

			if (p + 32 <= bEnd)
			{
				const xbyte *const limit = bEnd - 32;
				u64 v1 = m_v1;
				u64 v2 = m_v2;
				u64 v3 = m_v3;
				u64 v4 = m_v4;

				do
				{
					v1 = round(v1, read64bits(p));
					p += 8;
					v2 = round(v2, read64bits(p));
					p += 8;
					v3 = round(v3, read64bits(p));
					p += 8;
					v4 = round(v4, read64bits(p));
					p += 8;
				} while (p <= limit);

				m_v1 = v1;
				m_v2 = v2;
				m_v3 = v3;
				m_v4 = v4;
			}

			if (p < bEnd)
			{
				XXH_COPY_MEMORY(m_mem64, p, (u32)(bEnd - p));
				m_memsize = (unsigned)(bEnd - p);
			}
		}

		u64 finalize(u64 h64, const void *ptr, u32 len)
		{
			const xbyte *p = (const xbyte *)ptr;

#define PROCESS1_64            \
	h64 ^= (*p++) * PRIME64_5; \
	h64 = XXH_rotl64(h64, 11) * PRIME64_1;

#define PROCESS4_64                          \
	h64 ^= (u64)(read32bits(p)) * PRIME64_1; \
	p += 4;                                  \
	h64 = XXH_rotl64(h64, 23) * PRIME64_2 + PRIME64_3;

#define PROCESS8_64                                        \
	{                                                      \
		u64 const k1 = round(0, read64bits(p));            \
		p += 8;                                            \
		h64 ^= k1;                                         \
		h64 = XXH_rotl64(h64, 27) * PRIME64_1 + PRIME64_4; \
	}

			len &= 31;
			while (len >= 8)
			{
				PROCESS8_64;
				len -= 8;
			}
			if (len >= 4)
			{
				PROCESS4_64;
				len -= 4;
			}
			while (len > 0)
			{
				PROCESS1_64;
				--len;
			}
			return avalanche(h64);
		}

		void digest(xbuffer &hash)
		{
			u64 h64;
			if (m_total_len >= 32)
			{
				u64 const v1 = m_v1;
				u64 const v2 = m_v2;
				u64 const v3 = m_v3;
				u64 const v4 = m_v4;

				h64 = XXH_rotl64(v1, 1) + XXH_rotl64(v2, 7) + XXH_rotl64(v3, 12) + XXH_rotl64(v4, 18);
				h64 = mergeRound(h64, v1);
				h64 = mergeRound(h64, v2);
				h64 = mergeRound(h64, v3);
				h64 = mergeRound(h64, v4);
			}
			else
			{
				h64 = m_v3 /*seed*/ + PRIME64_5;
			}

			h64 += (u64)m_total_len;
			u64 digest = finalize(h64, m_mem64, (u32)m_total_len);

			hash.m_mutable[0] = (digest & 0xFF);
			digest = digest >> 8;
			hash.m_mutable[1] = (digest & 0xFF);
			digest = digest >> 8;
			hash.m_mutable[2] = (digest & 0xFF);
			digest = digest >> 8;
			hash.m_mutable[3] = (digest & 0xFF);
			digest = digest >> 8;
			hash.m_mutable[4] = (digest & 0xFF);
			digest = digest >> 8;
			hash.m_mutable[5] = (digest & 0xFF);
			digest = digest >> 8;
			hash.m_mutable[6] = (digest & 0xFF);
			digest = digest >> 8;
			hash.m_mutable[7] = (digest & 0xFF);
		}
	};

	xhash::xxhash64::xxhash64(u64 seed)
		: m_seed(seed)
	{

		xxhash64_ctxt *ctx = (xxhash64_ctxt *)&this->m_ctxt;
		ctx->reset(m_seed);
	}

	void xhash::xxhash64::reset()
	{
		xxhash64_ctxt *ctx = (xxhash64_ctxt *)&this->m_ctxt;
		ctx->reset(m_seed);
	}

	void xhash::xxhash64::hash(xcbuffer const &_buffer)
	{
		xxhash64_ctxt *ctx = (xxhash64_ctxt *)&this->m_ctxt;
		ctx->update(_buffer);
	}

	void xhash::xxhash64::end(hash::xxhash64 &out_hash)
	{
		xxhash64_ctxt *ctx = (xxhash64_ctxt *)&this->m_ctxt;
		xbuffer digest = out_hash.buffer();
		ctx->digest(digest);
	}

	void xhash::xxhash64::compute(xcbuffer const &data, xhash::hash::xxhash64 &out_hash)
	{
		reset();
		hash(data);
		end(out_hash);
	}

	xhash::hash::xxhash64 xhash::xxhash64::compute(xcbuffer const &data)
	{
		hash::xxhash64 hash;
		compute(data, hash);
		return hash;
	}

} // namespace xcore
