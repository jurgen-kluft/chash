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
	md5ctx*		md5_begin(xalloc*);
	s32			md5_size(md5ctx*);
	void 		md5_reset(md5ctx*);
	void		md5_hash(md5ctx* ctx, xcbuffer const& data);
	void		md5_end(md5ctx* ctx, xbuffer& hash);
	void		md5_close(xalloc*, md5ctx*);

	struct sha1ctx;
	sha1ctx*	sha1_begin(xalloc*);
	s32			sha1_size(sha1ctx*);
	void 		sha1_reset(sha1ctx*);
	void		sha1_hash(sha1ctx* ctx, xcbuffer const& data);
	void		sha1_end(sha1ctx* ctx, xbuffer& hash);
	void		sha1_close(xalloc*, sha1ctx*);

	struct skeinctx;
	skeinctx*	skein256_begin(xalloc*);
	s32			skein256_size(skeinctx*);
	void 		skein256_reset(skeinctx*);
	void		skein256_hash(skeinctx* ctx, xcbuffer const& data);
	void		skein256_end(skeinctx* ctx, xbuffer& hash);
	void		skein256_close(xalloc*, skeinctx*);

	skeinctx*	skein512_begin(xalloc*, u32 hashbits = 512);
	s32			skein512_size(skeinctx*);
	void 		skein512_reset(skeinctx*);
	void		skein512_hash(skeinctx* ctx, xcbuffer const& data);
	void		skein512_end(skeinctx* ctx, xbuffer& hash);
	void		skein512_close(xalloc*, skeinctx*);

	skeinctx*	skein1024_begin(xalloc*);
	s32			skein1024_size(skeinctx*);
	void 		skein1024_reset(skeinctx*);
	void		skein1024_hash(skeinctx* ctx, xcbuffer const& data);
	void		skein1024_end(skeinctx* ctx, xbuffer& hash);
	void		skein1024_close(xalloc*, skeinctx*);

	u32			murmur32_hash(xcbuffer const& data, u32 seed = 0);
	u64			murmur64_hash(xcbuffer const& data, u64 seed = 0);
}
#endif
