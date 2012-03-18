/**
 * @file x_digest.cpp
 *
 * Core Digest
 */

#include "xhash\x_digest_engine.h"

namespace xcore
{

	u32					xdigest_engine::length() const
	{
		return mEngine->length();
	}

	void				xdigest_engine::reset()
	{
		mEngine->reset();
	}

	void				xdigest_engine::update(void const* inBuffer, u32 inLength)
	{
		mEngine->update(inBuffer, inLength);
	}

	void				xdigest_engine::digest(xbyte* digest)
	{
		mEngine->digest(digest);
	}

}
