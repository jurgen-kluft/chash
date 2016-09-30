/**
 * @file x_digest_engine.h
 *
 * Core Digest Engine
 */

// x_digest_engine.h - Core Digest Engine - 
#ifndef __XHASH_DIGEST_ENGINE_H__
#define __XHASH_DIGEST_ENGINE_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif


namespace xcore
{
	class xdigest_engine
	{
	public:
		virtual u32			length() const = 0;
		virtual void		reset() = 0;
		virtual void		update(void const* inBuffer, u32 inLength) = 0;
		virtual void		digest(xbyte* digest) = 0;
	};
}

#endif	///< __XHASH_DIGEST_ENGINE_H__
