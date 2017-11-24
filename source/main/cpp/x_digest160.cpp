#include "xbase/x_target.h"
#include "xbase/x_string_ascii.h"
#include "xhash/x_digest160.h"

namespace xcore
{

	/**
	 * @brief Check if one hash value is smaller than another hash value
	 *
	 * This function implements operator< so that the ToString() of the hash
	 * value results in the same result when done with a dictionary compare.
	 */
	bool					xdigest160::operator<(xdigest160 const& inRHS) const
	{
		for (s32 i=0; i<20; i++)
		{
			u8 lhs=mData8[i], rhs=inRHS.mData8[i];

			if (lhs < rhs) return true;
			else if (lhs > rhs) return false;
		}
		return true;
	}


	/**
	 * @brief Check if one hash value is larger than another hash value 
	 *
	 * This function implements operator> so that the ToString() of the hash
	 * value results in the same result when done with a dictionary compare.
	 */
	bool					xdigest160::operator>(xdigest160 const& inRHS) const
	{
		for (s32 i=0; i<20; i++)
		{
			u8 lhs=mData8[i], rhs=inRHS.mData8[i];
			if (lhs > rhs) return true;
			else if (lhs < rhs) return false;
		}

		return false;
	}


	void					xdigest160::set(u32 inR1, u32 inR2, u32 inR3, u32 inR4, u32 inR5)
	{
		mData32[0] = inR1;
		mData32[1] = inR2;
		mData32[2] = inR3;
		mData32[3] = inR4;
		mData32[4] = inR5;
	}


	void					xdigest160::get(u32& outR1, u32& outR2, u32& outR3, u32& outR4, u32& outR5) const
	{
		outR1 = mData32[0];
		outR2 = mData32[1];
		outR3 = mData32[2];
		outR4 = mData32[3];
		outR5 = mData32[4];
	}


	/**
	 * @brief Convert MD5 hash value to String
	 */
	s32						xdigest160::toString(char* ioStr, u32 ioStrLength) const
	{
		if (ioStrLength < (size() * 2))
			return 0;

		const char* _format = "%02x%02x%02x%02x";
		u32 const l = size();
		char* s = ioStr;
		for (u32 i=0; i<l; i+=4)
			s = s + ascii::sprintf(s, s + 8, _format, NULL, x_va(mData8[i+0]),  x_va(mData8[i+1]),  x_va( mData8[i+2]), x_va(mData8[i+3]));

		return (u32)(s - ioStr);

	}



	/**
	 * @brief Set hash value from String
	 */
	bool					xdigest160::fromString(const char* inString)
	{
		if ((u32)ascii::size(inString) < (size() * 2))
			return false;

		const char* const format = "%02x%02x%02x%02x";
		u32 const l = size();
		for (u32 i=0; i<l; i+=4)
		{
			u8 d[4];
			if (ascii::sscanf(inString + (i*2), NULL, format, NULL, x_va_r(&d[0]), x_va_r(&d[1]), x_va_r(&d[2]), x_va_r(&d[3])) == 4)
			{
				for (s32 j=0; j<4; j++)
					mData8[i+j] = d[j];
			}
			else
			{
				return false;
			}
		}
		return true;
	}

}
