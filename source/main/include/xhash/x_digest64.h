/**
 * @file x_digest64.h
 *
 * Core Hash functions
 */

// x_digest64.h - Core Hash functions - 
#ifndef __XHASH_HASHABLE_64_H__
#define __XHASH_HASHABLE_64_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace xcore
{
	/**
	 *	Hash Utility functions
	 */
	typedef		u64			xdigest64;


	/**
	 *	Utilities, default 64 bit hash, murmur hash
	 */
	extern xdigest64		x_Hash64(const char* inBuffer, u32 inLength);			///< Get 64 bit hash value of a string

}
#endif	///< __XHASH_HASHABLE_32_H__
