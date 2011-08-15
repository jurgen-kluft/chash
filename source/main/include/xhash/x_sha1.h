// MD5.h - Core MD5 Hash value 
#ifndef __XHASH_MD5HASH_H__
#define __XHASH_MD5HASH_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase\x_types.h"

#include "xhash\x_hash160.h"
#include "xhash\x_hash160_generator.h"

namespace xcore
{
	/**
	@group		xhash
	@brief		SHA1 hash value
	@desc		This struct represents the SHA1 hash value ("message digest"). 
	**/
	class xsha1 : public xhash160
	{
	public:
							xsha1()													{ }
							xsha1(const char* inString) : xhash160(inString)		{ }
	};

	/**
	@group		xhash
	@brief		SHA1 hash value
	@desc		This is a SHA1 hash generator
	**/
	class xsha1_generator : public xihash160_generator
	{
		enum EState
		{
			OPEN,
			CLOSED,
		};
	public:
					xsha1_generator();

		///@name Updating
		void		open();
		void		compute(void const* inBuffer, u32 inLength);
		bool		close(xhash160& hash);

	private:
		void		reset();

		u32			mH[5];										// Message digest buffers

		u64			mMessage_Length;							// Message length in bits

		xbyte		mMessage_Block[320];						// 512-bit message blocks
		s32			mMessage_Block_Index;						// Index into message block array

		bool		mComputed;									// Is the digest computed?
		bool		mCorrupted;									// Is the message digest corrupted?
    
	};

	//---------------------------------------------------------------------------------------------------------------------
	//	Utilities
	//---------------------------------------------------------------------------------------------------------------------
	extern xsha1	x_Sha1Hash(void const* inBuffer, s32 inLength);			///< Get sha1 value of a block of data

}
#endif
