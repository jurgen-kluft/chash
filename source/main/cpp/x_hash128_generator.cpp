// x_hash128_generator.cpp - Core Hash128 Generator
#include "xbase\x_types.h"
#include "xbase\x_string_std.h"
#include "xhash\x_hash128.h"
#include "xhash\x_hash128_generator.h"

namespace xcore
{

	/**
	@brief Check if one hash value is smaller than another hash value

	This function implements operator< so that the ToString() of the hash
	value results in the same result when done with a dictionary compare.
	**/
	bool					xhash128::operator<(xhash128 const& inRHS) const
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
	@brief Check if one hash value is larger than another hash value 

	This function implements operator> so that the toString() of the hash
	value results in the same result when done with a dictionary compare.
	**/
	bool					xhash128::operator>(xhash128 const& inRHS) const
	{
		for (s32 i=0; i<16; i++)
		{
			u8 lhs=mData8[i], rhs=inRHS.mData8[i];
			if (lhs > rhs) return true;
			else if (lhs < rhs) return false;
		}

		return false;
	}


	void					xhash128::set(u32 inR1, u32 inR2, u32 inR3, u32 inR4)
	{
		mData32[0] = inR1;
		mData32[1] = inR2;
		mData32[2] = inR3;
		mData32[3] = inR4;
	}


	void					xhash128::get(u32& outR1, u32& outR2, u32& outR3, u32& outR4) const
	{
		outR1 = mData32[0];
		outR2 = mData32[1];
		outR3 = mData32[2];
		outR4 = mData32[3];
	}


	/**
	@brief Convert hash value to String
	**/
	bool					xhash128::toString(char* ioStr, u32& ioStrLength) const
	{
		if (ioStrLength < 32)
			return false;

		const char* _format = "%02x%02x%02x%02";

		char* s = ioStr;
		s = s + x_sprintf(s, 8, _format, x_va(mData8[0]),  x_va(mData8[1]),  x_va( mData8[2]), x_va(mData8[3]));
		s = s + x_sprintf(s, 8, _format, x_va(mData8[4]),  x_va(mData8[5]),  x_va( mData8[6]), x_va(mData8[7]));
		s = s + x_sprintf(s, 8, _format, x_va(mData8[8]),  x_va(mData8[9]),  x_va( mData8[10]), x_va(mData8[11]));
		s = s + x_sprintf(s, 8, _format, x_va(mData8[12]),  x_va(mData8[13]),  x_va( mData8[14]), x_va(mData8[15]));

		ioStrLength = (u32)(s - ioStr);
		return true;
	}



	/**
	@brief Set hash value from String
	**/
	bool					xhash128::fromString(const char* inString)
	{
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


	xhash128_generator::xhash128_generator(xihash128_generator* generator)
		: mGenerator(generator)
	{
	}

	///@name Updating
	void			xhash128_generator::open()
	{
		mGenerator->open();
	}

	void			xhash128_generator::compute(void const* inBuffer, s32 inLength)
	{
		mGenerator->compute(inBuffer, inLength);
	}

	bool			xhash128_generator::close(xhash128& hash)
	{
		return mGenerator->close(hash);
	}

}
