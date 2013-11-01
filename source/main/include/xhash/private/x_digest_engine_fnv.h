/**
* @file x_digest_engine_fnv.h
*
* xcore Digenst Engine - FNV
*/

// x_digest_engine_fnv.h - xCore Digest - 
#ifndef __XHASH_DIGEST_ENGINE_32_FNV_H__
#define __XHASH_DIGEST_ENGINE_32_FNV_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xhash\x_digest_engine.h"

namespace xcore
{
	/**
	 *	FNV 32a
	 */
	typedef		u32			xdigest32;

	class xdigest_fnv
	{
	public:
		static xdigest32		buf(void const* inData, u32 inLength);
		static xdigest32		buf(void const* inData, u32 inLength, xdigest32 inPrevious);
		static xdigest32		str(char const* inStr);
		static xdigest32		str(char const* inStr, xdigest32 inPrevious);
	};


	class xdigest_engine_fnv : public xidigest_engine
	{
		xdigest32			mHash;
	public:
							xdigest_engine_fnv() : mHash(0)						{ }

		virtual u32			length() const										{ return 4; }
		virtual void		reset()												{ mHash = 0; }
		virtual void		update(void const* inBuffer, s32 inLength)			{ xdigest_fnv::buf(inBuffer, inLength, mHash); }
		virtual void		digest(xbyte* digest)								
		{ 
			xbyte const* src = (xbyte const*)&mHash;
			for (s32 i=0; i<4; ++i) *digest++ = *src++;
		}

		bool				digest(xdigest32& digest)							{ digest = mHash; return true; }
	};

}
#endif	///< __XHASH_DIGEST_ENGINE_32_FNV_H__
