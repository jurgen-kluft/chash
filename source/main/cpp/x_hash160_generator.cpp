/**
 * @file x_hash160_generator.cpp
 *
 * Core Hash160 Generator
 */

// x_hash160_generator.cpp - Core Hash160 Generator
#include "xbase\x_types.h"
#include "xbase\x_string_std.h"
#include "xhash\x_hash160.h"
#include "xhash\x_hash160_generator.h"

namespace xcore
{

	/**
	 * @brief Check if one hash value is smaller than another hash value
	 *
	 * This function implements operator< so that the ToString() of the hash
	 * value results in the same result when done with a dictionary compare.
	 */
	bool					xhash160::operator<(xhash160 const& inRHS) const
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
	bool					xhash160::operator>(xhash160 const& inRHS) const
	{
		for (s32 i=0; i<20; i++)
		{
			u8 lhs=mData8[i], rhs=inRHS.mData8[i];
			if (lhs > rhs) return true;
			else if (lhs < rhs) return false;
		}

		return false;
	}


	void					xhash160::set(u32 inR1, u32 inR2, u32 inR3, u32 inR4, u32 inR5)
	{
		mData32[0] = inR1;
		mData32[1] = inR2;
		mData32[2] = inR3;
		mData32[3] = inR4;
		mData32[4] = inR5;
	}


	void					xhash160::get(u32& outR1, u32& outR2, u32& outR3, u32& outR4, u32& outR5) const
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
	bool					xhash160::toString(char* ioStr, u32& ioStrLength) const
	{
		if (ioStrLength < 40)
			return false;

		const char* _format = "%02x%02x%02x%02x";

		char* s = ioStr;
		s = s + x_sprintf(s, 8, _format, x_va(mData8[0]),  x_va(mData8[1]),  x_va( mData8[2]), x_va(mData8[3]));
		s = s + x_sprintf(s, 8, _format, x_va(mData8[4]),  x_va(mData8[5]),  x_va( mData8[6]), x_va(mData8[7]));
		s = s + x_sprintf(s, 8, _format, x_va(mData8[8]),  x_va(mData8[9]),  x_va( mData8[10]), x_va(mData8[11]));
		s = s + x_sprintf(s, 8, _format, x_va(mData8[12]),  x_va(mData8[13]),  x_va( mData8[14]), x_va(mData8[15]));
		s = s + x_sprintf(s, 8, _format, x_va(mData8[16]),  x_va(mData8[17]),  x_va( mData8[18]), x_va(mData8[19]));

		ioStrLength = (u32)(s - ioStr);
		return true;
	}



	/**
	 * @brief Set hash value from String
	 */
	bool					xhash160::fromString(const char* inString)
	{
		char part[21];
		for (s32 i=0; i<20; ++i)
			part[i] = inString[i];
		part[20] = '\0';

		u32 d[10];
		const char* format = "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x";
		if (x_sscanf(part, format, x_va_r(&d[0]), x_va_r(&d[1]), x_va_r(&d[2]), x_va_r(&d[3]), x_va_r(&d[4]), x_va_r(&d[5]), x_va_r(&d[6]), x_va_r(&d[7]), x_va_r(&d[8]), x_va_r(&d[9])) == 10)
		{
			for (s32 i=0; i<10; i++)
				mData8[i] = d[i];
		}
		else
			return false;

		for (s32 i=0; i<20; ++i)
			part[i] = inString[20+i];
		part[20] = '\0';

		if (x_sscanf(part, format, x_va_r(&d[0]), x_va_r(&d[1]), x_va_r(&d[2]), x_va_r(&d[3]), x_va_r(&d[4]), x_va_r(&d[5]), x_va_r(&d[6]), x_va_r(&d[7]), x_va_r(&d[8]), x_va_r(&d[9])) == 10)
		{
			for (s32 i=0; i<10; i++)
				mData8[10+i] = d[i];
		}
		else
			return false;

		return true;
	}


	xhash160_generator::xhash160_generator(xihash160_generator* generator)
		: mGenerator(generator)
	{
	}

	///@name Updating
	void			xhash160_generator::open()
	{
		mGenerator->open();
	}

	void			xhash160_generator::compute(void const* inBuffer, u32 inLength)
	{
		mGenerator->compute(inBuffer, inLength);
	}

	bool			xhash160_generator::close(xhash160& hash)
	{
		return mGenerator->close(hash);
	}

}
