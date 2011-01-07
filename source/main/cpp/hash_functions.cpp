// Hash.cpp - Core Hash functions 
#include "CSystem\CSystem.h"
#include "CHash\Hash.h"
#include "CSystem\Endian.h"
#include "CHash\HashFunctions.h"

BEGIN_CORE_NAMESPACE

uint32 HashFunctions::RSHash(int8* str, uint32 len)
{
	uint32 b    = 378551;
	uint32 a    = 63689;
	uint32 hash = 0;
	uint32 i    = 0;

	for(i = 0; i < len; str++, i++)
	{
		hash = hash * a + (*str);
		a    = a * b;
	}

	return hash;
}

// Red Dragon Book, * 65599
uint32 HashFunctions::RDBHash(int8* str, uint32 len)
{
	uint32 hash = 0;

	for(uint32 i = 0; i < len; str++, i++)
		hash = hash * 65599 + (*str);

	return hash ^ (hash >> 16);
}

uint32 HashFunctions::JSHash(int8* str, uint32 len)
{
	uint32 hash = 1315423911;
	uint32 i    = 0;

	for(i = 0; i < len; str++, i++)
	{
		hash ^= ((hash << 5) + (*str) + (hash >> 2));
	}

	return hash;
}

// P. J. Weinberger Hash Function
uint32 HashFunctions::PJWHash(int8* str, uint32 len)
{
	const uint32 BitsInUnsignedInt = (uint32)(sizeof(uint32) * 8);
	const uint32 ThreeQuarters     = (uint32)((BitsInUnsignedInt  * 3) / 4);
	const uint32 OneEighth         = (uint32)(BitsInUnsignedInt / 8);
	const uint32 HighBits          = (uint32)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
	uint32 hash              = 0;
	uint32 test              = 0;
	uint32 i                 = 0;

	for(i = 0; i < len; str++, i++)
	{
		hash = (hash << OneEighth) + (*str);

		if((test = hash & HighBits)  != 0)
		{
			hash = (( hash ^ (test >> ThreeQuarters)) & (~HighBits));
		}
	}

	return hash;
}

// ELF Hash Function
uint32 HashFunctions::ELFHash(int8* str, uint32 len)
{
	uint32 hash = 0;
	uint32 x    = 0;
	uint32 i    = 0;

	for(i = 0; i < len; str++, i++)
	{
		hash = (hash << 4) + (*str);
		if((x = hash & 0xF0000000L) != 0)
		{
			hash ^= (x >> 24);
		}
		hash &= ~x;
	}

	return hash;
}

// BKDR Hash Function
uint32 HashFunctions::BKDRHash(int8* str, uint32 len)
{
	uint32 seed = 131; /* 31 131 1313 13131 131313 etc.. */
	uint32 hash = 0;
	uint32 i    = 0;

	for(i = 0; i < len; str++, i++)
	{
		hash = (hash * seed) + (*str);
	}

	return hash;
}

uint32 HashFunctions::SDBMHash(int8* str, uint32 len)
{
	uint32 hash = 0;
	uint32 i    = 0;

	for(i = 0; i < len; str++, i++)
	{
		hash = (*str) + (hash << 6) + (hash << 16) - hash;
	}

	return hash;
}

uint32 HashFunctions::DJBHash(int8* str, uint32 len)
{
	uint32 hash = 5381;
	uint32 i    = 0;

	for(i = 0; i < len; str++, i++)
	{
		hash = ((hash << 5) + hash) + (*str);
	}

	return hash;
}


uint32 HashFunctions::DEKHash(int8* str, uint32 len)
{
	uint32 hash = len;
	uint32 i    = 0;

	for(i = 0; i < len; str++, i++)
	{
		hash = ((hash << 5) ^ (hash >> 27)) ^ (*str);
	}
	return hash;
}

uint32 HashFunctions::BPHash(int8* str, uint32 len)
{
	uint32 hash = 0;
	uint32 i    = 0;
	for(i = 0; i < len; str++, i++)
	{
		hash = hash << 7 ^ (*str);
	}

	return hash;
}

uint32 HashFunctions::FNVHash(int8* str, uint32 len)
{
	const uint32 fnv_prime = 0x811C9DC5;
	uint32 hash      = 0;
	uint32 i         = 0;

	for(i = 0; i < len; str++, i++)
	{
		hash *= fnv_prime;
		hash ^= (*str);
	}

	return hash;
}

uint32 HashFunctions::APHash(int8* str, uint32 len)
{
	uint32 hash = 0xAAAAAAAA;
	uint32 i    = 0;

	for(i = 0; i < len; str++, i++)
	{
		hash ^= ((i & 1) == 0) ? (  (hash <<  7) ^ (*str) ^ (hash >> 3)) : (~((hash << 11) ^ (*str) ^ (hash >> 5)));
	}

	return hash;
}

END_CORE_NAMESPACE
