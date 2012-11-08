/**
 * @file x_digest_murmur32.h
 *
 * xcore Digest Engine
 */

// x_digest_murmur32.h - Core Digest - 
#ifndef __XHASH_DIGEST_MURMUR_32_H__
#define __XHASH_DIGEST_MURMUR_32_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace xcore
{
	/**
	 *	Murmur 32 bits
	 */
	typedef		u32			xdigest32;

	class xdigest_murmur32
	{
	public:
		/// Murmur hash
		static xdigest32			buf(void const* inData, u32 inLength);
		static xdigest32			buf(void const* inData, u32 inLength, xdigest32 inPrevious);
		static xdigest32			str(char const* inStr);
		static xdigest32			str(char const* inStr, xdigest32 inPrevious);
	};



}
#endif	///< __XHASH_DIGEST_MURMUR_32_H__
