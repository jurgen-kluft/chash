#include "ccore/c_target.h"
#include "ccore/c_debug.h"
#include "chash/c_crc.h"

namespace ncore
{
	class CRC32
	{
	public:
		enum EConstants
		{
			POLY32		= 0xedb88320,
			TABLE_SIZE	= 256
		};

		static bool		mIsEmpty;
		static u32		mTable[TABLE_SIZE];

		static void		CDO1(u8 const*& ioDataPtr, u32& ioCRC)						{ ioCRC = mTable[(ioCRC ^ (*ioDataPtr++)) & 0xff] ^ (ioCRC >> 8); }
		static void		CDO4(u8 const*& ioDataPtr, u32& ioCRC)						{ CDO1(ioDataPtr, ioCRC); CDO1(ioDataPtr, ioCRC); CDO1(ioDataPtr, ioCRC); CDO1(ioDataPtr, ioCRC); }
		static void		CDO16(u8 const*& ioDataPtr, u32& ioCRC)						{ CDO4(ioDataPtr, ioCRC); CDO4(ioDataPtr, ioCRC); CDO4(ioDataPtr, ioCRC); CDO4(ioDataPtr, ioCRC); }
	};

	bool		CRC32::mIsEmpty = true;
	u32			CRC32::mTable[256];

	/**
	 * @group		xhash
	 * Calculate running CRC of <inInitVal> over <inBuffer> with length <inLength>
	 */
	u32	crc_t::crc32(cbuffer_t const& buffer, u32 inInitVal)
	{
		ASSERT(buffer.m_begin);
		u8 const* p_in = (u8 const*)buffer.m_begin;
		u32	crc  = ~inInitVal;

		// Create CRC32 table
		if (CRC32::mIsEmpty)
		{
			// Fill crc table for first 2^8 bit combinations
			for (s32 n=0; n<(s32)CRC32::TABLE_SIZE; n++)
			{
				u32 c = n;
				for (s32 k=0; k<8; k++)
					c = (c & 1) ? (CRC32::POLY32^(c >> 1)) : (c >> 1);
				CRC32::mTable[n] = c;
			}
			CRC32::mIsEmpty = false;
		}

		for (u32 i=0; i<buffer.size(); i++)
			crc = CRC32::mTable[(crc ^ (*p_in++)) & 0xff] ^ (crc >> 8);

		return ~crc;
	}



	/**
	 *	Adler16 & Adler32 checksums
	 */
	class CRCAdler
	{
	public:
		enum EAdlerSettings
		{
			BASE16		= 251,															///< Largest prime smaller than 256
			NMAX16		= 5802,															///< Largest n such that 255n(n+1)/2 + (n+1)(BASE-1) <= 2^32-1

			BASE32		= 65521,														///< Largest prime smaller than 65536
			NMAX32		= 5552,															///< Largest n such that 255n(n+1)/2 + (n+1)(BASE-1) <= 2^32-1
		};

		static void		ADO1(u8 const*& ioDataPtr, u32& ioA1, u32& ioA2)			{ ioA1 += *ioDataPtr++; ioA2 += ioA1; }
		static void		ADO4(u8 const*& ioDataPtr, u32& ioA1, u32& ioA2)			{ ADO1(ioDataPtr, ioA1, ioA2); ADO1(ioDataPtr, ioA1, ioA2); ADO1(ioDataPtr, ioA1, ioA2); ADO1(ioDataPtr, ioA1, ioA2); }
		static void		ADO16(u8 const*& ioDataPtr, u32& ioA1, u32& ioA2)			{ ADO4(ioDataPtr, ioA1, ioA2); ADO4(ioDataPtr, ioA1, ioA2); ADO4(ioDataPtr, ioA1, ioA2); ADO4(ioDataPtr, ioA1, ioA2); }
	};

	/**
	 * @group		xhash
	 * Calculate running Adler32 of <inInitVal> over <inBuffer> with length <inLength>
	 */
	u32 crc_t::adler32(cbuffer_t const& buffer, u32 inInitVal)
	{
		u32 a1 = inInitVal & 0xFFFF;							///< Adler sum parts
		u32 a2 = inInitVal >> 16;

		u8 const* p_in = (u8 const*)buffer.m_begin;

		// Go on doing bits and pieces of the area until we're done
		s64 len = buffer.size();
		while (len)
		{
			s64 max_do = (len < (s32)CRCAdler::NMAX32) ? len : ((s32)CRCAdler::NMAX32);

			len -= max_do;
			while (max_do >= 16) { CRCAdler::ADO16(p_in, a1, a2); max_do-=16; }
			while (max_do >  0)  { CRCAdler::ADO1(p_in, a1, a2);  max_do--;   }

			a1 %= CRCAdler::BASE32;													// a1 MOD Prime<2^16
			a2 %= CRCAdler::BASE32;
		}

		return (a1 | (a2 << 16));
	}



	/**
	 * @group		xhash
	 * Calculate running Adler16 of <inInitVal> over <inBuffer> with length <inLength>
	 */
	u16	crc_t::adler16(cbuffer_t const& buffer, u16 inInitVal)
	{
		u32 a1 = inInitVal & 0xFF;								// Adler sum parts
		u32 a2 = inInitVal >> 8;

		u8 const* p_in = (u8 const*)buffer.m_begin;

		// Go on doing bits and pieces of the area until we're done
		s64 len = buffer.size();
		while (len)
		{
			s64 max_do = (len < (s32)CRCAdler::NMAX16) ? len : ((s32)CRCAdler::NMAX16);

			len -= max_do;
			while (max_do >= 16) { CRCAdler::ADO16(p_in, a1, a2); max_do-=16; }
			while (max_do >  0)  { CRCAdler::ADO1(p_in, a1, a2);  max_do--; }

			a1 %= CRCAdler::BASE16;									// a1 MOD Prime<2^8
			a2 %= CRCAdler::BASE16;
		}

		return (a1 | (a2<<8));
	}

}
