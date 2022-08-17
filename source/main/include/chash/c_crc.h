/**
 * @file x_crc.h
 *
 * Core Checksum functions
 */

// x_crc.h - Core Checksum functions 
#ifndef __XHASH_CHECKSUM_H__
#define __XHASH_CHECKSUM_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "cbase/c_buffer.h"

namespace ncore
{
	/**
	 * @group		xhash
	 * @brief		CRC implementations
	 */
	class xcrc
	{
	public:
		static u32			crc32(cbuffer_t const& buffer, u32 inInitVal = 0);

		static u16			adler16(cbuffer_t const& buffer, u16 inInitVal = 1);
		static u32			adler32(cbuffer_t const& buffer, u32 inInitVal = 1);
	};


}
#endif
