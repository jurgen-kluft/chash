// Hash.h - Core Hash functions - 
#ifndef __XHASH_HASHING_H__
#define __XHASH_HASHING_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase\x_types.h"
#include "xhash\x_hash32_generator.h"

namespace xcore
{
	//---------------------------------------------------------------------------------------------------------------------
	//	Murmur 32 Hash
	//---------------------------------------------------------------------------------------------------------------------
	typedef		u32			xhash32;

	class xhash_generator_murmur
	{
	public:
		// Murmur hash
		static xhash32			buf(void const* inData, u32 inLength);
		static xhash32			buf(void const* inData, u32 inLength, xhash32 inPrevious);
		static xhash32			str(char const* inStr);
		static xhash32			str(char const* inStr, xhash32 inPrevious);
	};

	class xmurmur32_generator : public xihash32_generator
	{
		xhash32				mHash;
	public:
							xmurmur32_generator() : mHash(0)					{ }

		virtual void		open()												{ mHash = 0; }
		virtual void		compute(void const* inBuffer, s32 inLength)			{ xhash_generator_murmur::buf(inBuffer, inLength, mHash); }
		virtual bool		close(xhash32& hash)								{ hash = mHash; return true; }
	};


}
#endif	///< __XHASH_HASHING_H__
