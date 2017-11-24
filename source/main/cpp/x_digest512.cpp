#include "xbase/x_target.h"
#include "xbase/x_string_ascii.h"
#include "xhash/x_digest512.h"

namespace xcore
{

	/**
	 * @brief Check if one hash value is smaller than another hash value
	 *
	 * This function implements operator< so that the ToString() of the hash
	 * value results in the same result when done with a dictionary compare.
	 */
	bool					xdigest512::operator<(xdigest512 const& inRHS) const
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
	bool					xdigest512::operator>(xdigest512 const& inRHS) const
	{
		for (u32 i=0; i<size(); i++)
		{
			u8 lhs=mData8[i], rhs=inRHS.mData8[i];
			if (lhs > rhs) return true;
			else if (lhs < rhs) return false;
		}

		return false;
	}

	void					xdigest512::set32(u32 i, u32 inR)
	{
		if (i<size32())
			mData32[i] = inR;
	}


	void					xdigest512::get32(u32 i, u32& outR) const
	{
		if (i<size32())
			outR = mData32[i];
		else
			outR = 0;
	}

	void					xdigest512::set64(u32 i, u64 inR)
	{
		if (i<size64())
			mData64[i] = inR;
	}


	void					xdigest512::get64(u32 i, u64& outR) const
	{
		if (i<size64())
			outR = mData64[i];
		else
			outR = 0;
	}

	/**
	 * @brief Convert hash value to String
	 */
	s32						xdigest512::toString(char* ioStr, u32 ioStrLength) const
	{
		if (ioStrLength < (size() * 2))
			return 0;

		const char* _format = "%02X%02X%02X%02X";
		u32 const l = size();
		char* s = ioStr;
		for (u32 i=0; i<l; i+=4)
			s = s + ascii::sprintf(s, s + 8, _format, NULL, x_va(mData8[i+0]),  x_va(mData8[i+1]),  x_va( mData8[i+2]), x_va(mData8[i+3]));

		return (u32)(s - ioStr);
	}

	/**
	 * @brief Set hash value from String
	 */
	bool					xdigest512::fromString(const char* inString)
	{
		if ((u32)ascii::size(inString) < (size() * 2))
			return false;

		const char* const format = "%02X%02X%02X%02X";
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
