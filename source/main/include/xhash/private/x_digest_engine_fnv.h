/**
* @file x_digest_engine_fnv.h
*
* xcore Digenst Engine - FNV
*/

// x_digest_engine_fnv.h - xCore Digest - 
#ifndef __XHASH_DIGEST_ENGINE_32_FNV_H__
#define __XHASH_DIGEST_ENGINE_32_FNV_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase/x_chars.h"
#include "xhash/x_digest_engine.h"

namespace xcore
{
	/**
	 *	FNV 32a
	 */
	typedef		u32			xdigest32;

	class xdigest_fnv
	{
	public:
		static xdigest32		buf(xcbuffer const& buffer);
		static xdigest32		buf(xcbuffer const& buffer, xdigest32 prev);

		static xdigest32		str(xcuchars const& str);
		static xdigest32		str(xcuchars const& str, xdigest32 prev);
	};


	class xdigest_engine_fnv : public xdigest_engine
	{
		xdigest32			mHash;
	public:
							xdigest_engine_fnv() : mHash(0)						{ }

		virtual u32			length() const										{ return 4; }
		virtual void		reset()												{ mHash = 0; }
		virtual void		update(xcbuffer const& buffer)						{ xdigest_fnv::buf(buffer, mHash); }
		virtual void		digest(xbuffer& digest)
		{ 
			xbyte const* src = (xbyte const*)&mHash;
			for (s32 i=0; i<4; ++i) digest[i] = *src++;
		}

		bool				digest(xdigest32& digest)							{ digest = mHash; return true; }
	};

}
#endif	///< __XHASH_DIGEST_ENGINE_32_FNV_H__
