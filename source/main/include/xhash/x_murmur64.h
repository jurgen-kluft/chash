/**
 * @file x_murmur64.h
 *
 * xcore Digest Engine, 64 bit murmur hash
 */

// x_murmur32.h - Core Digest - 
#ifndef __XHASH_DIGEST_ENGINE_MURMUR_64_H__
#define __XHASH_DIGEST_ENGINE_MURMUR_64_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xhash\x_digest_engine.h"

namespace xcore
{
	/**
	 *	64 bits digest
	 */
	typedef		u64			xdigest64;

	class xdigest_engine_murmur64 : public xidigest_engine
	{
		xdigest64			mHash;

	public:
							xdigest_engine_murmur64() : mHash(0)				{ }

		virtual u32			length() const										{ return 8; }
		virtual void		reset()												{ mHash = 0; }
		virtual void		update(void const* inBuffer, s32 inLength);
		virtual void		digest(xbyte* digest)								{ *((u64*)digest) = mHash; }

		bool				digest(xdigest64& digest)							{ digest = mHash; return true; }
	};


}
#endif	///< __XHASH_DIGEST_ENGINER_MURMUR_64_H__
