// x_hash128.h - Core Hash128 - 
#ifndef __XHASH_HASH128_H__
#define __XHASH_HASH128_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase\x_types.h"

namespace xcore
{
	//---------------------------------------------------------------------------------------------------------------------
	//	Hash Utility functions
	//---------------------------------------------------------------------------------------------------------------------
	struct xhash128
	{
							xhash128()												{ clear(); }
							xhash128(const char* inString)							{ fromString(inString); }

		void				clear()													{ set(0,0,0,0); }

		///@name Equality
		bool				operator==(xhash128 const& inRHS) const					{ return mData32[0] == inRHS.mData32[0] && mData32[1] == inRHS.mData32[1] && mData32[2] == inRHS.mData32[2] && mData32[3] == inRHS.mData32[3]; }
		bool				operator!=(xhash128 const& inRHS) const					{ return !(*this == inRHS); }
		bool				operator<(xhash128 const& inRHS) const;
		bool				operator>(xhash128 const& inRHS) const;

		///@name The hash value
		void				set(u32 inR1, u32 inR2, u32 inR3, u32 inR4);
		void				get(u32& outR1, u32& outR2, u32& outR3, u32& outR4) const;

		///@name Hashing
		inline u32			getHash() const											{ return mData32[0]; } ///< Get hash value (when used as a key in a hash set or map)

		///@name To/From String
		bool				toString(char* ioStr, u32& ioStrLength) const;			///< Convert hash value to String (incoming length > 16)
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
#endif	///< __XHASH_HASH128_H__
