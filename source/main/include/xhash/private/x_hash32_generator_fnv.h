// x_hash32_generator_fnv.h - xCore Hash - 
#ifndef __XHASH_HASH32_GENERATOR_FNV_H__
#define __XHASH_HASH32_GENERATOR_FNV_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase\x_types.h"
#include "xhash\x_hash32_generator.h"

namespace xcore
{
	//---------------------------------------------------------------------------------------------------------------------
	//	FNV 32a Hash
	//---------------------------------------------------------------------------------------------------------------------
	typedef		u32			xhash32;

	class xhash_generator_fnv
	{
	public:
		static xhash32		buf(void const* inData, u32 inLength);
		static xhash32		buf(void const* inData, u32 inLength, xhash32 inPrevious);
		static xhash32		str(char const* inStr);
		static xhash32		str(char const* inStr, xhash32 inPrevious);
	};


	class xfnv32_generator : public xihash32_generator
	{
		xhash32				mHash;
	public:
							xfnv32_generator() : mHash(0)						{ }

		virtual void		open()												{ mHash = 0; }
		virtual void		compute(void const* inBuffer, s32 inLength)			{ xhash_generator_fnv::buf(inBuffer, inLength, mHash); }
		virtual bool		close(xhash32& hash)								{ hash = mHash; }
	};

}
#endif	///< __XHASH_HASH32_GENERATOR_FNV_H__
