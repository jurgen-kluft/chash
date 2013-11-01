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

namespace xcore
{
	struct xdigest128
	{
							xdigest128()												{ clear(); }
							xdigest128(const char* inString)							{ fromString(inString); }

		void				clear()														{ mData64[0]=0; mData64[1]=0; }

		///@name Equality
		bool				operator==(xdigest128 const& inRHS) const					{ return mData64[0] == inRHS.mData64[0] && mData64[1] == inRHS.mData64[1]; }
		bool				operator!=(xdigest128 const& inRHS) const					{ return !(*this == inRHS); }
		bool				operator<(xdigest128 const& inRHS) const;
		bool				operator>(xdigest128 const& inRHS) const;

		///@name The hash value
		void				set32(s32 index, u32 inR);
		void				get32(s32 index, u32& outR) const;
		void				set64(s32 index, u64 inR);
		void				get64(s32 index, u64& outR) const;

		///@name Hashing
		inline u32			getHash32() const											{ return mData32[0]; } ///< Get hash value (when used as a key in a hash set or map)
		inline u64			getHash64() const											{ return mData64[0]; } ///< Get hash value (when used as a key in a hash set or map)

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
