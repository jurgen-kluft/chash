/**
 * @file x_digest_murmur64.h
 *
 * xcore Digest Engine
 */

// x_digest_murmur64.h - Core Digest - 
#ifndef __XHASH_DIGEST_MURMUR_64_H__
#define __XHASH_DIGEST_MURMUR_64_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase/x_buffer.h"
#include "xbase/x_chars.h"

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
		static xdigest64			buf(xcbuffer const& _buffer);
		static xdigest64			buf(xcbuffer const& _buffer, xdigest64 prev);
		static xdigest64			str(xcuchars const& _str);
		static xdigest64			str(xcuchars const& _str, xdigest64 prev);
	};

}
#endif	///< __XHASH_DIGEST_MURMUR_64_H__
