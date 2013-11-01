/**
 * @file x_hash128_generator.cpp
 *
 * Core Hash128 Generator
 */

// x_hash128_generator.cpp - Core Hash128 Generator
#include "xbase\x_target.h"
#include "xbase\x_string_std.h"
#include "xhash\x_digest128.h"

namespace xcore
{

	/**
	 * @brief Check if one hash value is smaller than another hash value
	 *
	 * This function implements operator< so that the ToString() of the hash
	 * value results in the same result when done with a dictionary compare.
	 */
	bool					xdigest128::operator<(xdigest128 const& inRHS) const
	{
		for (s32 i=0; i<16; i++)
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
	 * This function implements operator> so that the toString() of the hash
	 * value results in the same result when done with a dictionary compare.
	 */
	bool					xdigest128::operator>(xdigest128 const& inRHS) const
	{
		for (s32 i=0; i<16; i++)
		{
			u8 lhs=mData8[i], rhs=inRHS.mData8[i];
			if (lhs > rhs) return true;
			else if (lhs < rhs) return false;
		}

		return false;
	}


	void					xdigest128::set32(s32 i, u32 inR)
	{
		if (i>=0 && i<4)
			mData32[i] = inR;
	}


	void					xdigest128::get32(s32 i, u32& outR) const
	{
		if (i>=0 && i<4)
			outR = mData32[i];
		else
			outR = 0xFFFFFFFF;
	}


	/**
	 * @brief Convert hash value to String
	 */
	s32						xdigest128::toString(char* ioStr, u32 ioStrLength) const
	{
		if (ioStrLength < 32)
			return 0;

		const char* _format = "%02x%02x%02x%02x";

		char* s = ioStr;
		s = s + x_sprintf(s, 8, _format, x_va(mData8[0]),  x_va(mData8[1]),  x_va( mData8[2]), x_va(mData8[3]));
		s = s + x_sprintf(s, 8, _format, x_va(mData8[4]),  x_va(mData8[5]),  x_va( mData8[6]), x_va(mData8[7]));
		s = s + x_sprintf(s, 8, _format, x_va(mData8[8]),  x_va(mData8[9]),  x_va( mData8[10]), x_va(mData8[11]));
		s = s + x_sprintf(s, 8, _format, x_va(mData8[12]),  x_va(mData8[13]),  x_va( mData8[14]), x_va(mData8[15]));

		return (u32)(s - ioStr);
	}



	/**
	 * @brief Set hash value from String
	 */
	bool					xdigest128::fromString(const char* inString)
	{
		if (x_strlen(inString) < 32)
			return false;

		u32 d[16];
		const char* format = "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x";
		if (x_sscanf(inString, format, x_va_r(&d[0]), x_va_r(&d[1]), x_va_r(&d[2]), x_va_r(&d[3]), x_va_r(&d[4]), x_va_r(&d[5]), x_va_r(&d[6]), x_va_r(&d[7]), x_va_r(&d[8]), x_va_r(&d[9]), x_va_r(&d[10]), x_va_r(&d[11]), x_va_r(&d[12]), x_va_r(&d[13]), x_va_r(&d[14]), x_va_r(&d[15])) == 16)
		{
			for (s32 i=0; i<16; i++)
				mData8[i] = d[i];
		}
		else
			return false;

		return true;
	}

}
