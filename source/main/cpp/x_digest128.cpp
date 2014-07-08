#include "xbase\x_target.h"
#include "xbase\x_string_ascii.h"
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
		for (u32 i=0; i<size(); i++)
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
		for (u32 i=0; i<size(); i++)
		{
			u8 lhs=mData8[i], rhs=inRHS.mData8[i];
			if (lhs > rhs) return true;
			else if (lhs < rhs) return false;
		}

		return false;
	}


	void					xdigest128::set32(u32 i, u32 inR)
	{
		if (i<size32())
			mData32[i] = inR;
	}


	void					xdigest128::get32(u32 i, u32& outR) const
	{
		if (i>=0 && i<size32())
			outR = mData32[i];
		else
			outR = 0;
	}


	/**
	 * @brief Convert hash value to String
	 */
	s32						xdigest128::toString(char* ioStr, u32 ioStrLength) const
	{
		if (ioStrLength < (size() * 2))
			return 0;

		const char* _format = "%02x%02x%02x%02x";
		u32 const l = size();
		char* s = ioStr;
		for (u32 i=0; i<l; i+=4)
			s = s + x_sprintf(s, 8, _format, x_va(mData8[i+0]),  x_va(mData8[i+1]),  x_va( mData8[i+2]), x_va(mData8[i+3]));

		return (u32)(s - ioStr);
	}


	/**
	 * @brief Set hash value from String
	 */
	bool					xdigest128::fromString(const char* inString)
	{
		if ((u32)x_strlen(inString) < (size() * 2))
			return false;

		const char* const format = "%02x%02x%02x%02x";
		u32 const l = size();
		for (u32 i=0; i<l; i+=4)
		{
			u8 d[4];
			if (x_sscanf(inString + (i*2), format, x_va_r(&d[0]), x_va_r(&d[1]), x_va_r(&d[2]), x_va_r(&d[3])) == 4)
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
