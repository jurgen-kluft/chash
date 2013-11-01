/**
 * @file x_md5.h
 *
 * xCore MD5 Digest
 */

// x_md5.h - xCore MD5 Digest
#ifndef __XHASH_MD5_DIGEST_H__
#define __XHASH_MD5_DIGEST_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xhash\x_digest128.h"
#include "xhash\x_digest_engine.h"

namespace xcore
{
	/**
	 * @group		xhash
	 * @brief		MD5 hash value
	 * @desc		This struct represents the MD5 digest ("message digest"). It contains a
	 *			    16-byte (128 bits) value, returned by the md5_ class.
	 */
	class xmd5 : public xdigest128
	{
	public:
							xmd5()													{ }
							xmd5(const char* inString) : xdigest128(inString)		{ }
	};

	/**
	 * @group	xhash
	 * @brief	MD5 hash function
	 * @desc	This class represents the MD5 hash function. This code is a modification from
	 * 			the implementation by Colin Plumb. He placed this code in the public domain,
	 * 			no copyright is claimed.

	 * 			The original algorithm was developed by Ron Rivest (the 'R' in RSA Security).

	 * 			The MD5 hash algorithm calculates a hash value ("message digest") of a set of
	 * 			bytes (the "message"). MD5 hash is performed over chunks of 64 bytes.

	 * 			To retrieve the MD5 hash value of an arbitrary set of bytes, construct an
	 * 			MD5Hash object, call Update() on all of the data that needs to be hashed
	 * 			and call the GetHash() function to retrieve the MD5Hash.
	 */
	class xdigest_engine_md5 : public xidigest_engine
	{
		enum EState
		{
			OPEN,
			CLOSED,
		};
	public:
		///@name Construction/Destruction
							xdigest_engine_md5();

		///@name Updating
		virtual u32			length() const { return 16; }
		virtual void		reset();
		virtual void		update(void const* inBuffer, u32 inLength);
		virtual void		digest(xbyte* digest);

		void				digest(xmd5& md5);

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


#endif	// __XHASH_MD5_DIGEST_H__
