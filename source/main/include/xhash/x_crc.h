/**
 * @file x_crc.h
 *
 * Core Checksum functions
 */

// x_crc.h - Core Checksum functions 
#ifndef __XHASH_CHECKSUM_H__
#define __XHASH_CHECKSUM_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif


namespace xcore
{
	/**
	 * @group		xhash
	 * @brief		CRC implementations
	 */
	class xcrc
	{
	public:
		static u32			crc32(void const* inBuffer, s32 inLength, u32 inInitVal = 0);

		static u16			adler16(void const* inBuffer, s32 inLength, u16 inInitVal = 1);
		static u32			adler32(void const* inBuffer, s32 inLength, u32 inInitVal = 1);
	};


}
#endif
