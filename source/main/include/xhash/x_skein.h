/**
 * @file x_skein.h
 *
 * Core Hash256, Hash512 Generators
 */

// x_skein.h - Core Skein Hash value 
#ifndef __XHASH_SKEIN_DIGEST_H__
#define __XHASH_SKEIN_DIGEST_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xhash/x_digest256.h"
#include "xhash/x_digest512.h"
#include "xhash/x_digest_engine.h"

namespace xcore
{
	/**
	 * @group		xhash
	 * @brief		skein hash value
	 * @desc		This struct represents the skein hash value ("message digest"). 
	 */
	class xskein256 : public xdigest256
	{
	public:
							xskein256()													{ }
							xskein256(const char* inString) : xdigest256(inString)		{ }
	};

	struct xskein256_ctx
	{
		u64		header[3];
		u64		X[4];
		u8		b[32];
	};


	class xskein512 : public xdigest512
	{
	public:
							xskein512()													{ }
							xskein512(const char* inString) : xdigest512(inString)		{ }
	};

	struct xskein512_ctx
	{
		u64		header[3];
		u64		X[8];
		u8		b[64];
	};


	/**
	 * @group		xhash
	 * @brief		skein hash value
	 * @desc		This is a skein digest engine
	 */
	class xdigest_engine_skein256 : public xdigest_engine
	{
		enum EState
		{
			OPEN,
			CLOSED,
		};

	public:
						xdigest_engine_skein256();

		///@name Updating
		virtual u32		length() const { return 32; }
		virtual void	reset();
		virtual void	update(xcbuffer const& buffer);
		virtual void	digest(xbuffer & digest);
		bool			digest(xskein256& hash);

	private:
		void			reset2();

		EState			mState;									// Is the digest computed?
		xskein256_ctx	mCtx;
	};

	class xdigest_engine_skein512 : public xdigest_engine
	{
		enum EState
		{
			OPEN,
			CLOSED,
		};

	public:
						xdigest_engine_skein512();

		///@name Updating
		virtual u32		length() const { return 64; }
		virtual void	reset();
		virtual void	update(xcbuffer const& buffer);
		virtual void	digest(xbuffer & digest);
		bool			digest(xskein512& hash);

	private:
		void			reset2();

		EState			mState;									// Is the digest computed?
		xskein512_ctx	mCtx;
	};

	/**
	 *	Utilities
	 */
	extern xskein256	x_skein256Hash256(xcbuffer const& buffer);				///< Get skein256 256 bits hash value of a block of data
	extern xskein256	x_skein512Hash256(xcbuffer const& buffer);				///< Get skein512 256 bits hash value of a block of data
	extern xskein512	x_skein512Hash512(xcbuffer const& buffer);				///< Get skein512 512 bits hash value of a block of data

	extern xskein256	x_skein256Hash256B(xcbuffer const& buffer, u32 inLengthInBits);		///< Get skein256 256 bits hash value of a bit-stream
	extern xskein256	x_skein512Hash256B(xcbuffer const& buffer, u32 inLengthInBits);		///< Get skein512 256 bits hash value of a bit-stream
	extern xskein512	x_skein512Hash512B(xcbuffer const& buffer, u32 inLengthInBits);		///< Get skein512 512 bits hash value of a bit-stream
}
#endif	// __XHASH_skein_DIGEST_H__
