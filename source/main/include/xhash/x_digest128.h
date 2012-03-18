/**
 * @file x_digest128.h
 *
 * Core Digest 128
 */

// x_digest128.h - Core Digest 128 bits -  
#ifndef __XHASH_DIGEST_128_H__
#define __XHASH_DIGEST_128_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase\x_types.h"

namespace xcore
{
	struct xdigest128
	{
							xdigest128()												{ clear(); }
							xdigest128(const char* inString)							{ fromString(inString); }

		void				clear()														{ set(0,0,0,0); }

		///@name Equality
		bool				operator==(xdigest128 const& inRHS) const					{ return mData32[0] == inRHS.mData32[0] && mData32[1] == inRHS.mData32[1] && mData32[2] == inRHS.mData32[2] && mData32[3] == inRHS.mData32[3]; }
		bool				operator!=(xdigest128 const& inRHS) const					{ return !(*this == inRHS); }
		bool				operator<(xdigest128 const& inRHS) const;
		bool				operator>(xdigest128 const& inRHS) const;

		///@name The hash value
		void				set(u32 inR1, u32 inR2, u32 inR3, u32 inR4);
		void				get(u32& outR1, u32& outR2, u32& outR3, u32& outR4) const;

		///@name Hashing
		inline u32			getHash() const											{ return mData32[0]; } ///< Get hash value (when used as a key in a hash set or map)

		///@name To/From String
		s32					toString(char* ioStr, u32 ioStrLength) const;			///< Convert hash value to String (incoming length > 16)
		bool				fromString(const char* inStr);							///< Set hash value from String

	private:
		union
		{
			u64				mData64[2];
			u32				mData32[4];
			u16				mData16[8];
			u8				mData8[16];
		};
	};


}
#endif	///< __XHASH_DIGEST_128_H__
