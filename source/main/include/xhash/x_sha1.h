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

	struct xsha1_ctx
	{
		u64		size;
		u32		H[5];
		u32		W[16];
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
		virtual void open();
		virtual void compute(void const* inBuffer, u32 inLength);
		virtual bool close(xhash160& hash);

	private:
		void		reset();

		bool		mComputed;									// Is the digest computed?
		xsha1_ctx	mCtx;
	};

	//---------------------------------------------------------------------------------------------------------------------
	//	Utilities
	//---------------------------------------------------------------------------------------------------------------------
	extern xsha1	x_Sha1Hash(void const* inBuffer, s32 inLength);			///< Get sha1 value of a block of data

}
#endif
