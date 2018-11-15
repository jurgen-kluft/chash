#ifndef __XHASH_HASH_H__
#define __XHASH_HASH_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase/x_allocator.h"
#include "xbase/x_buffer.h"

namespace xcore
{
	struct md5ctx;
	s32			md5_size();
	md5ctx*		md5_begin(xalloc* alloc);
	void		md5_hash(md5ctx* ctx, xbuffer& data);
	void		md5_end(md5ctx* ctx, xbuffer& hash);

	struct sha1ctx;
	s32			sha1_size();
	sha1ctx*	sha1_begin(xalloc* alloc);
	void		sha1_hash(sha1ctx* ctx, xbuffer& data);
	void		sha1_end(sha1ctx* ctx, xbuffer& hash);

	struct skeinctx;
	skeinctx*	skein_create(xalloc* alloc);

	s32			skein_size();
	void		skein_begin(skeinctx* ctx);
	void		skein_hash(skeinctx* ctx, xbuffer& data);
	void		skein_end(skeinctx* ctx, xbuffer& hash);

	void		x_skein256Hash256(xcbuffer const& buffer, xbuffer& hash);
	void		x_skein256Hash256B(xcbuffer const& buffer, u32 inLengthInBits, xbuffer& hash);

	void		x_skein512Hash256B(xcbuffer const& buffer, u32 inLengthInBits, xbuffer& hash);
	void		x_skein512Hash512B(xcbuffer const& buffer, u32 inLengthInBits, xbuffer& hash);

	u32			murmur32_hash(xcbuffer const& data, u32 seed = 0);
	u64			murmur64_hash(xcbuffer const& data, u64 seed = 0);
}
#endif
