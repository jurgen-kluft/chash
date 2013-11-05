/**
 * @file x_digest256.h
 *
 * Core Digest 256
 */

// x_digest256.h - Core Digest 256 bits -  
#ifndef __XHASH_DIGEST_256_H__
#define __XHASH_DIGEST_256_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace xcore
{
	struct xdigest256
	{
							xdigest256()												{ clear(); }
							xdigest256(const char* inString)							{ fromString(inString); }

		void				clear()														{ for (s32 i=0; i<4; ++i) mData64[i]=0; }

		u32					size() const												{ return 32; }
		u32					size32() const												{ return 8; }
		u32					size64() const												{ return 4; }

		///@name Equality
		bool				operator==(xdigest256 const& inRHS) const					{ return mData64[0] == inRHS.mData64[0] && mData64[1] == inRHS.mData64[1] && mData64[2] == inRHS.mData64[2] && mData64[3] == inRHS.mData64[3]; }
		bool				operator!=(xdigest256 const& inRHS) const					{ return !(*this == inRHS); }
		bool				operator<(xdigest256 const& inRHS) const;
		bool				operator>(xdigest256 const& inRHS) const;

		///@name The hash value
		void				set32(u32 index, u32 inR);
		void				get32(u32 index, u32& outR) const;
		void				set64(u32 index, u64 inR);
		void				get64(u32 index, u64& outR) const;

		///@name Hashing
		inline u32			getHash32() const											{ return mData32[0]; } ///< Get hash value (when used as a key in a hash set or map)
		inline u64			getHash64() const											{ return mData64[0]; } ///< Get hash value (when used as a key in a hash set or map)

		///@name Access
		u8 const*			data() const												{ return mData8; }
		u8*					data()														{ return mData8; }

		///@name To/From String
		s32					toString(char* ioStr, u32 ioStrLength) const;			///< Convert hash value to String (incoming length > 16)
		bool				fromString(const char* inStr);							///< Set hash value from String

	private:
		union
		{
			u64				mData64[4];
			u32				mData32[8];
			u16				mData16[16];
			u8				mData8[32];
		};
	};


}
#endif	///< __XHASH_DIGEST_256_H__
