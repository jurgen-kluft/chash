// x_hash160.h - Core Hash160 - 
#ifndef __XHASH_HASH160_H__
#define __XHASH_HASH160_H__
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
	struct xhash160
	{
							xhash160()												{ clear(); }
							xhash160(const char* inString)							{ fromString(inString); }

		///@name Equality
		bool				operator==(xhash160 const& inRHS) const					{ return mData32[0] == inRHS.mData32[0] && mData32[1] == inRHS.mData32[1] && mData32[2] == inRHS.mData32[2] && mData32[3] == inRHS.mData32[3] && mData32[4] == inRHS.mData32[4]; }
		bool				operator!=(xhash160 const& inRHS) const					{ return !(*this == inRHS); }
		bool				operator<(xhash160 const& inRHS) const;
		bool				operator>(xhash160 const& inRHS) const;

		///@name The hash value
		void				set(u32 inR1, u32 inR2, u32 inR3, u32 inR4, u32 inR5);
		void				get(u32& outR1, u32& outR2, u32& outR3, u32& outR4, u32& outR5) const;

		///@name Hashing
		inline u32			getHash() const											{ return mData32[0]; } ///< Get hash value (when used as a key in a hash set or map)

		///@name To/From String
		bool				toString(char* ioStr, u32& ioStrLength) const;			///< Convert hash value to String (incoming length > 16)
		bool				fromString(const char* inStr);							///< Set hash value from String

		void				clear()													{ set(0,0,0,0,0); }

	private:
		union
		{
			u32				mData32[5];
			u16				mData16[10];
			u8				mData8[20];
		};
	};


}
#endif	///< __XHASH_HASH160_H__
