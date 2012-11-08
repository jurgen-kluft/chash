/**
 * @file x_digest_murmur64.h
 *
 * xcore Digest Engine
 */

// x_digest_murmur64.h - Core Digest - 
#ifndef __XHASH_DIGEST_MURMUR_64_H__
#define __XHASH_DIGEST_MURMUR_64_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace xcore
{
	/**
	 *	Murmur 64 bits
	 */
	typedef		u64			xdigest64;

	class xdigest_murmur64
	{
	public:
		/// Murmur hash
		static xdigest64			buf(void const* inData, u32 inLength);
		static xdigest64			buf(void const* inData, u32 inLength, xdigest64 inPrevious);
		static xdigest64			str(char const* inStr);
		static xdigest64			str(char const* inStr, xdigest64 inPrevious);
	};

}
#endif	///< __XHASH_DIGEST_MURMUR_64_H__
