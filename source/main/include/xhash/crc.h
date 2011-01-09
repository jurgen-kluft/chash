// Checksum.h - Core Checksum functions 
#ifndef __XHASH_CHECKSUM_H__
#define __XHASH_CHECKSUM_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif
#include "xbase\x_types.h"

namespace xcore
{

	/**
	@group		xhash
	@brief		CRC implementations
	**/
	class CRC
	{
	public:
		static u32			CRC32(void const* inBuffer, int inLength, u32 inInitVal = 0);

		static u16			Adler16(void const* inBuffer, int inLength, u16 inInitVal = 1);
		static u32			Adler32(void const* inBuffer, int inLength, u32 inInitVal = 1);
	};


}
#endif
