/**
 * @file x_md5.h
 *
 * xCore MD5 Hash value
 */

// x_md5.h - xCore MD5 Hash value 
#ifndef __XHASH_MD5HASH_H__
#define __XHASH_MD5HASH_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase\x_types.h"

#include "xhash\x_hash128.h"
#include "xhash\x_hash128_generator.h"

namespace xcore
{
	/**
	 * @group		xhash
	 * @brief		MD5 hash value
	 * @desc		This struct represents the MD5 hash value ("message digest"). It contains a
	 *			16-byte (128 bits) value, returned by the MD5Hash class.
	 */
	class xmd5 : public xhash128
	{
	public:
							xmd5()													{ }
							xmd5(const char* inString) : xhash128(inString)			{ }
	};

	/**
	 * @group		xhash
	 * @brief		MD5 hash function
	 * @desc		This class represents the MD5 hash function. This code is a modification from
	 * 			the implementation by Colin Plumb. He placed this code in the public domain,
	 * 			no copyright is claimed.

	 * 			The original algorithm was developed by Ron Rivest (the 'R' in RSA Security).

	 * 			The MD5 hash algorithm calculates a hash value ("message digest") of a set of
	 * 			bytes (the "message"). MD5 hash is performed over chunks of 64 bytes.

	 * 			To retrieve the MD5 hash value of an arbitrary set of bytes, construct an
	 * 			MD5Hash object, call Update() on all of the data that needs to be hashed
	 * 			and call the GetHash() function to retrieve the MD5Hash.
	 */
	class xmd5_generator : public xihash128_generator
	{
		enum EState
		{
			OPEN,
			CLOSED,
		};
	public:
		///@name Construction/Destruction
							xmd5_generator();

		///@name Updating
		virtual void		open();
		virtual void		compute(void const* inBuffer, s32 inLength);
		virtual bool		close(xhash128& hash);

	private:
		void				transform();

		EState				mState;
		u32					mMD5[4];											///< 128 bits MD5 hash value
		u64					mLength;

		struct buffer_t
		{
			u32			mInput[16];												///< 64 byte input buffer
			u8			mSlack[7];												///< Slack space because internal memcopy copies gready (i.e. up to 7 bytes past mInput)
		};
		buffer_t			mBuffer;
	};

	/**
	 *	Utilities
	 */
	extern xmd5		x_MD5Hash(void const* inBuffer, s32 inLength);			///< Get MD5 hash value of a block of data
}


#endif
