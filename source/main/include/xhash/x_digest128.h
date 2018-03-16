/**
 * @file x_digest128.h
 *
 * Core Digest 128
 */

// x_digest128.h - Core Digest 128 bits -  
#ifndef __XHASH_DIGEST_128_H__
#define __XHASH_DIGEST_128_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase/x_chars.h"

namespace xcore
{
	struct xdigest128
	{
							xdigest128()												{ clear(); }
							xdigest128(ascii::crunes const& str)						{ fromString(str); }

		void				clear()														{ mData64[0]=0; mData64[1]=0; }

		u32					size() const												{ return 16; }
		u32					size32() const												{ return 4; }
		u32					size64() const												{ return 2; }

		///@name Equality
		bool				operator==(xdigest128 const& inRHS) const					{ return mData64[0] == inRHS.mData64[0] && mData64[1] == inRHS.mData64[1]; }
		bool				operator!=(xdigest128 const& inRHS) const					{ return !(*this == inRHS); }
		bool				operator<(xdigest128 const& inRHS) const;
		bool				operator>(xdigest128 const& inRHS) const;

		///@name The hash value
		void				set8(u32 index, u8 inR)										{ ASSERT(index < sizeof(mData8)); mData8[index] = inR;}
		void				get8(u32 index, u8& outR) const								{ ASSERT(index < sizeof(mData8)); outR = mData8[index]; }
		void				set32(u32 index, u32 inR);
		void				get32(u32 index, u32& outR) const;
		void				set64(u32 index, u64 inR);
		void				get64(u32 index, u64& outR) const;

		void				set(u32 R0, u32 R1, u32 R2, u32 R3)							{ mData32[0]=R0; mData32[1]=R1; mData32[2]=R2; mData32[3]=R3; } 
		void				get(u32&R0, u32&R1, u32&R2, u32&R3)							{ R0=mData32[0]; R1=mData32[1]; R2=mData32[2]; R3=mData32[3]; } 

		///@name Hashing
		inline u32			getHash32() const											{ return mData32[0]; } ///< Get hash value (when used as a key in a hash set or map)
		inline u64			getHash64() const											{ return mData64[0]; } ///< Get hash value (when used as a key in a hash set or map)

		///@name To/From String
		s32					toString(xuchars& str) const;								//< Convert hash value to String (incoming length > 16)
		bool				fromString(xcuchars const& str);							//< Set hash value from String

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
