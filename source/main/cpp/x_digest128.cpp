#include "xbase/x_target.h"
#include "xbase/x_string_ascii.h"
#include "xhash/x_digest128.h"

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
	s32						xdigest128::toString(xuchars& str) const
	{
		if (str.size() < (size() * 2))
			return 0;

		xcuchars _format("%02X%02X%02X%02X");
		u32 const l = size();
		for (u32 i = 0; i<l; i += 4)
			ascii::sprintf(str, _format, x_va(mData8[i + 0]), x_va(mData8[i + 1]), x_va(mData8[i + 2]), x_va(mData8[i + 3]));

		return str.size();
	}

	/**
	* @brief Set hash value from String
	*/
	bool					xdigest128::fromString(xcuchars const& _str)
	{
		if (_str.size() < (size() * 2))
			return false;

		xcuchars str(_str);
		xcuchars format("%02X%02X%02X%02X");
		u32 const l = size();
		for (u32 i = 0; i<l; i += 4)
		{
			u8 d[4];
			if (ascii::sscanf(str, format, x_va_r(&d[0]), x_va_r(&d[1]), x_va_r(&d[2]), x_va_r(&d[3])) == 4)
			{
				for (s32 j = 0; j<4; j++)
					mData8[i + j] = d[j];
			}
			else
			{
				return false;
			}
		}
		return true;
	}
}
