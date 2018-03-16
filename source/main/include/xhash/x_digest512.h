/**
 * @file x_digest512.h
 *
 * Core Digest 512
 */

// x_digest512.h - Core Digest 512 bits -  
#ifndef __XHASH_DIGEST_512_H__
#define __XHASH_DIGEST_512_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase/x_chars.h"

namespace xcore
{
	struct xdigest512
	{
							xdigest512()												{ clear(); }
							xdigest512(const char* inString)							{ fromString(inString); }

		void				clear()														{ for (s32 i=0; i<8; ++i) mData64[i]=0; }
		
		u32					size() const												{ return 64; }
		u32					size32() const												{ return 16; }
		u32					size64() const												{ return 8; }

		//@name Equality
		bool				operator==(xdigest512 const& inRHS) const					{ for (s32 i=0; i<8; ++i) if (mData64[i]!=inRHS.mData64[i]) return false; return true; }
		bool				operator!=(xdigest512 const& inRHS) const					{ return !(*this == inRHS); }
		bool				operator<(xdigest512 const& inRHS) const;
		bool				operator>(xdigest512 const& inRHS) const;

		//@name The hash value
		void				set32(u32 index, u32 inR);
		void				get32(u32 index, u32& outR) const;
		void				set64(u32 index, u64 inR);
		void				get64(u32 index, u64& outR) const;

		//@name Hashing
		inline u32			getHash32() const											{ return mData32[0]; } //< Get hash value (when used as a key in a hash set or map)
		inline u64			getHash64() const											{ return mData64[0]; } //< Get hash value (when used as a key in a hash set or map)

		//@name Access
		u8 const*			data() const												{ return mData8; }
		u8*					data()														{ return mData8; }

		//@name To/From String
		s32					toString(xuchars& str) const;								//< Convert hash value to String (incoming length > 16)
		bool				fromString(xcuchars const& str);							//< Set hash value from String

	private:
		union
		{
			u64				mData64[8];
			u32				mData32[16];
			u16				mData16[32];
			u8				mData8[64];
		};
	};


}
#endif	///< __XHASH_DIGEST_512_H__
