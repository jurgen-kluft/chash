/**
 * @file x_murmur32.h
 *
 * xcore Digest Engine, 32 bit murmur hash
 */

// x_murmur32.h - Core Digest - 
#ifndef __XHASH_DIGEST_ENGINE_MURMUR_32_H__
#define __XHASH_DIGEST_ENGINE_MURMUR_32_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xhash/x_digest_engine.h"

namespace xcore
{
	/**
	 *	32 bits digest
	 */
	typedef		u32			xdigest32;

	class xdigest_engine_murmur32 : public xdigest_engine
	{
		xdigest32			mHash;

	public:
							xdigest_engine_murmur32() : mHash(0)				{ }

		virtual u32			length() const										{ return 4; }
		virtual void		reset()												{ mHash = 0; }
		virtual void		update(xcbuffer const& buffer);
		virtual void		digest(xbuffer & digest)							{ digest = xcbuffer(4, (xbyte const*)&mHash);}

		bool				digest(xdigest32& digest)							{ digest = mHash; return true; }
	};


}
#endif	///< __XHASH_DIGEST_ENGINE_MURMUR_32_H__
