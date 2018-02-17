/**
 * @file x_digest_murmur32.h
 *
 * xcore Digest Engine
 */

// x_digest_murmur32.h - Core Digest - 
#ifndef __XHASH_DIGEST_MURMUR_32_H__
#define __XHASH_DIGEST_MURMUR_32_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase/x_buffer.h"
#include "xbase/x_chars.h"

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
		static xdigest32			buf(xcbuffer const& buffer);
		static xdigest32			buf(xcbuffer const& buffer, xdigest32 inPrevious);
		static xdigest32			str(xcuchars const& _str);
		static xdigest32			str(xcuchars const& _str, xdigest32 inPrevious);
	};



}
#endif	///< __XHASH_DIGEST_MURMUR_32_H__
