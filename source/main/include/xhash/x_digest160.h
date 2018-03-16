/**
 * @file x_digest160.h
 *
 * Core Digest 160 bits
 */

// x_digest160.h - Core Digest 160 - 
#ifndef __XHASH_DIGEST_160_H__
#define __XHASH_DIGEST_160_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase/x_chars.h"

namespace xcore
{
	struct xdigest160
	{
							xdigest160()											{ clear(); }
							xdigest160(const char* inString)						{ fromString(inString); }

		u32					size() const											{ return 20; }
		u32					size32() const											{ return 5; }

		///@name Equality
		bool				operator==(xdigest160 const& inRHS) const				{ return mData32[0] == inRHS.mData32[0] && mData32[1] == inRHS.mData32[1] && mData32[2] == inRHS.mData32[2] && mData32[3] == inRHS.mData32[3] && mData32[4] == inRHS.mData32[4]; }
		bool				operator!=(xdigest160 const& inRHS) const				{ return !(*this == inRHS); }
		bool				operator<(xdigest160 const& inRHS) const;
		bool				operator>(xdigest160 const& inRHS) const;

		///@name The hash value
		void				set(u32 inR1, u32 inR2, u32 inR3, u32 inR4, u32 inR5);
		void				get(u32& outR1, u32& outR2, u32& outR3, u32& outR4, u32& outR5) const;

		///@name Hashing
		inline u32			getHash() const											{ return mData32[0]; } ///< Get hash value (when used as a key in a hash set or map)

		///@name To/From String
		s32					toString(xuchars& str) const;								//< Convert hash value to String (incoming length > 16)
		bool				fromString(xcuchars const& str);							//< Set hash value from String

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
#endif	///< __XHASH_DIGEST_160_H__
