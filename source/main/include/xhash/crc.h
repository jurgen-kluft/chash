// Checksum.h - Core Checksum functions 
#ifndef __CORE_CHECKSUM_H__
#define __CORE_CHECKSUM_H__
#include "CCore\CCore.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif
#include "CSystem\CSystem.h"
BEGIN_CORE_NAMESPACE

/**
@group		CHash
@brief		CRC implementations
**/
class	CRC
{
public:
	static uint32			CRC32(pcVoid inBuffer, int inLength, uint32 inInitVal = 0);

	static uint16			Adler16(pcVoid inBuffer, int inLength, uint16 inInitVal = 1);
	static uint32			Adler32(pcVoid inBuffer, int inLength, uint32 inInitVal = 1);
};


END_CORE_NAMESPACE
#endif
