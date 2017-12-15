/**
 * @file x_sha1.h
 *
 * Core Hash128 Generator
 */

// x_sha1.h - Core SHA1 Hash value 
#ifndef __XHASH_SHA1_DIGEST_H__
#define __XHASH_SHA1_DIGEST_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xhash/x_digest160.h"
#include "xhash/x_digest_engine.h"

namespace xcore
{
	/**
	 * @group		xhash
	 * @brief		SHA1 hash value
	 * @desc		This struct represents the SHA1 hash value ("message digest"). 
	 */
	class xsha1 : public xdigest160
	{
	public:
							xsha1()													{ }
							xsha1(const char* inString) : xdigest160(inString)		{ }
	};

	struct xsha1_ctx
	{
		u64		size;
		u32		H[5];
		u32		W[16];
	};


	/**
	 * @group		xhash
	 * @brief		SHA1 hash value
	 * @desc		This is a SHA1 digest engine
	 */
	class xdigest_engine_sha1 : public xdigest_engine
	{
		enum EState
		{
			OPEN,
			CLOSED,
		};

	public:
						xdigest_engine_sha1();

		///@name Updating
		virtual u32		length() const { return 20; }
		virtual void	reset();
		virtual void	update(xcbuffer const& buffer);
		virtual void	digest(xbuffer & digest);

		bool			digest(xsha1& hash);

	private:
		void			reset2();

		bool			mComputed;									// Is the digest computed?
		xsha1_ctx		mCtx;
	};

	/**
	 *	Utilities
	 */
	extern xsha1	x_Sha1Hash(xcbuffer const& buffer);			///< Get sha1 value of a block of data

}
#endif	// __XHASH_SHA1_DIGEST_H__
