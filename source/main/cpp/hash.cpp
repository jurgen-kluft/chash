// Hash.cpp - Core Hash functions 
#include "xbase\x_types.h"
#include "xhash\hash.h"

namespace xcore
{
	//
	// Hash Mix utility function, by Bob Jenkins (see pigs_checksum.cpp for more info) - Mix 3 32-bit values reversibly
	//
	// - If gHashMix() is run forward or backward, at least 32 bits in a,b,c have at
	//   least 1/4 probability of changing.
	//
	// - If gHashMix() is run forward, every bit of c will change between 1/3 and
	//   2/3 of the time.
	//
	static inline void	sHashMix(u32& a, u32& b, u32& c)
	{
		a -= b; a -= c; a ^= (c>>13);
		b -= c; b -= a; b ^= (a<<8);
		c -= a; c -= b; c ^= (b>>13);
		a -= b; a -= c; a ^= (c>>12);
		b -= c; b -= a; b ^= (a<<16);
		c -= a; c -= b; c ^= (b>>5);
		a -= b; a -= c; a ^= (c>>3);
		b -= c; b -= a; b ^= (a<<10);
		c -= a; c -= b; c ^= (b>>15);
	}


	//---------------------------------------------------------------------------------------------------------------------
	//	Hash function
	//
	//	This hashing algorithm was developed by Bob Jenkins (bob_jenkins@compuserve.com)
	//	and further explained in Dr. Dobbs Journal (www.ddj.com), see also:
	//
	//	http://burtleburtle.net/bob/hash/doobs.html
	//---------------------------------------------------------------------------------------------------------------------

	enum { GOLDEN_RATIO	 = 0x9e3779b9 };											// arbitrary value used to init hash value

	//
	// Return a hash value from a string with length <inLength> stored at <inData>,
	// the initial hash value is <inInitVal>
	//
	u32		Hashing::Hash(void const* inData, u32 inLength, u32 inInitValue)
	{
		u32  length = inLength;
		u32 a = (u32)GOLDEN_RATIO;
		u32 b = (u32)GOLDEN_RATIO;
		u32 c = inInitValue;															// the previous hash value
		u8  *p_in = (u8 *)inData;

		// Do the largest part of the key
		while (length >= 12)
		{
			a += (p_in[0] + ((u32)p_in[1]<<8) + ((u32)p_in[2] <<16) + ((u32)p_in[3] <<24));
			b += (p_in[4] + ((u32)p_in[5]<<8) + ((u32)p_in[6] <<16) + ((u32)p_in[7] <<24));
			c += (p_in[8] + ((u32)p_in[9]<<8) + ((u32)p_in[10]<<16) + ((u32)p_in[11]<<24));
			sHashMix(a, b, c);
			p_in += 12; length -= 12;
		}

		// Handle the last 11 bytes
		c += inLength;
		switch(length)
		{
		case 11: c+=((u32)p_in[10]<<24);
		case 10: c+=((u32)p_in[9]<<16);
		case 9 : c+=((u32)p_in[8]<<8);											// the first byte of c is reserved for the length
		case 8 : b+=((u32)p_in[7]<<24);
		case 7 : b+=((u32)p_in[6]<<16);
		case 6 : b+=((u32)p_in[5]<<8);
		case 5 : b+=p_in[4];
		case 4 : a+=((u32)p_in[3]<<24);
		case 3 : a+=((u32)p_in[2]<<16);
		case 2 : a+=((u32)p_in[1]<<8);
		case 1 : a+=p_in[0];
		}
		sHashMix(a, b, c);

		return c;
	}



	/*
	-------------------------------------------------------------------------------
	lookup3.c, by Bob Jenkins, May 2006, Public Domain.

	These are functions for producing 32-bit hashes for hash table lookup.
	hashword(), hashlittle(), hashlittle2(), hashbig(), mix(), and final() 
	are externally useful functions.  Routines to test the hash are included 
	if SELF_TEST is defined.  You can use this free for any purpose.  It's in
	the public domain.  It has no warranty.

	You probably want to use hashlittle().hashlittle() and hashbig()
	hash byte arrays. hashlittle() is faster than hashbig() on
	little-endian machines.  Intel and AMD are little-endian machines.
	On second thought, you probably want hashlittle2(), which is identical to
	hashlittle() except it returns two 32-bit hashes for the price of one.  
	You could implement hashbig2() if you wanted but I haven't bothered here.

	If you want to find a hash of, say, exactly 7 integers, do
	a = i1;  b = i2;  c = i3;
	mix(a,b,c);
	a += i4; b += i5; c += i6;
	mix(a,b,c);
	a += i7;
	final(a,b,c);
	then use c as the hash value.  If you have a variable length array of
	4-byte integers to hash, use hashword().  If you have a byte array (like
	a character string), use hashlittle().  If you have several byte arrays, or
	a mix of things, see the comments above hashlittle().  

	Why is this so big?  I read 12 bytes at a time into 3 4-byte integers, 
	then mix those integers.  This is fast (you can do a lot more thorough
	mixing with 12*3 instructions on 3 integers than you can with 3 instructions
	on 1 byte), but shoehorning those bytes into integers efficiently is messy.
	-------------------------------------------------------------------------------
	*/
	//#define SELF_TEST 1

#ifdef SELF_TEST
#include <stdio.h>      /* defines printf for tests */
#include <time.h>       /* defines time_t for timings in the test */
#endif

#define		VALGRIND

#define		hashsize(n)			((u32)1<<(n))
#define		hashmask(n)			(hashsize(n)-1)
#define		rot(x,k)			(((x)<<(k)) | ((x)>>(32-(k))))

	/*
	-------------------------------------------------------------------------------
	mix -- mix 3 32-bit values reversibly.

	This is reversible, so any information in (a,b,c) before mix() is
	still in (a,b,c) after mix().

	If four pairs of (a,b,c) inputs are run through mix(), or through
	mix() in reverse, there are at least 32 bits of the output that
	are sometimes the same for one pair and different for another pair.
	This was tested for:
	* pairs that differed by one bit, by two bits, in any combination
	of top bits of (a,b,c), or in any combination of bottom bits of
	(a,b,c).
	* "differ" is defined as +, -, ^, or ~^.  For + and -, I transformed
	the output delta to a Gray code (a^(a>>1)) so a string of 1's (as
	is commonly produced by subtraction) look like a single 1-bit
	difference.
	* the base values were pseudo random, all zero but one bit set, or 
	all zero plus a counter that starts at zero.

	Some k values for my "a-=c; a^=rot(c,k); c+=b;" arrangement that
	satisfy this are
	4  6  8 16 19  4
	9 15  3 18 27 15
	14  9  3  7 17  3
	Well, "9 15 3 18 27 15" didn't quite get 32 bits diffing
	for "differ" defined as + with a one-bit base and a two-bit delta.  I
	used http://burtleburtle.net/bob/hash/avalanche.html to choose 
	the operations, constants, and arrangements of the variables.

	This does not achieve avalanche.  There are input bits of (a,b,c)
	that fail to affect some output bits of (a,b,c), especially of a.  The
	most thoroughly mixed value is c, but it doesn't really even achieve
	avalanche in c.

	This allows some parallelism.  Read-after-writes are good at doubling
	the number of bits affected, so the goal of mixing pulls in the opposite
	direction as the goal of parallelism.  I did what I could.  Rotates
	seem to cost as much as shifts on every machine I could lay my hands
	on, and rotates are much kinder to the top and bottom bits, so I used
	rotates.
	-------------------------------------------------------------------------------
	*/
#define mix(a,b,c) \
	{ \
	a -= c;  a ^= rot(c, 4);  c += b; \
	b -= a;  b ^= rot(a, 6);  a += c; \
	c -= b;  c ^= rot(b, 8);  b += a; \
	a -= c;  a ^= rot(c,16);  c += b; \
	b -= a;  b ^= rot(a,19);  a += c; \
	c -= b;  c ^= rot(b, 4);  b += a; \
	}

	/*
	-------------------------------------------------------------------------------
	final -- final mixing of 3 32-bit values (a,b,c) into c

	Pairs of (a,b,c) values differing in only a few bits will usually
	produce values of c that look totally different.  This was tested for
	* pairs that differed by one bit, by two bits, in any combination
	of top bits of (a,b,c), or in any combination of bottom bits of
	(a,b,c).
	* "differ" is defined as +, -, ^, or ~^.  For + and -, I transformed
	the output delta to a Gray code (a^(a>>1)) so a string of 1's (as
	is commonly produced by subtraction) look like a single 1-bit
	difference.
	* the base values were pseudorandom, all zero but one bit set, or 
	all zero plus a counter that starts at zero.

	These constants passed:
	14 11 25 16 4 14 24
	12 14 25 16 4 14 24
	and these came close:
	4  8 15 26 3 22 24
	10  8 15 26 3 22 24
	11  8 15 26 3 22 24
	-------------------------------------------------------------------------------
	*/
#define final(a,b,c) \
	{ \
	c ^= b; c -= rot(b,14); \
	a ^= c; a -= rot(c,11); \
	b ^= a; b -= rot(a,25); \
	c ^= b; c -= rot(b,16); \
	a ^= c; a -= rot(c,4);  \
	b ^= a; b -= rot(a,14); \
	c ^= b; c -= rot(b,24); \
	}

	/*
	--------------------------------------------------------------------
	This works on all machines.  To be useful, it requires
	-- that the key be an array of u32's, and
	-- that the length be the number of u32's in the key

	The function hash() is identical to hashlittle() on little-endian
	machines, and identical to hashbig() on big-endian machines,
	except that the length has to be measured in u32s rather than in
	bytes.  hashlittle() is more complicated than hash() only because
	hashlittle() has to dance around fitting the key bytes into registers.
	--------------------------------------------------------------------
	*/

	static void Hash2_NE(u32 const* inData, u32 inLength, u32& pc, u32& pb)
	{
		u32 a,b,c;
		a = b = c = 0xdeadbeef + ((u32)(inLength<<2)) + pc;
		c += pb;

		while (inLength > 3)
		{
			a += inData[0];
			b += inData[1];
			c += inData[2];
			mix(a,b,c);
			inLength -= 3;
			inData += 3;
		}

		switch(inLength)
		{ 
		case 3 : c+=inData[2];
		case 2 : b+=inData[1];
		case 1 : a+=inData[0];
			final(a,b,c);
		case 0:
			break;
		}

		pc=c;
		pb=b;
	}


	static u32 Hash2_NE(u32 const* inData, u32 inLength, u32 pc)         								
	{
		u32 pb = 0;
		Hash2_NE(inData, inLength, pc, pb);
		return pc;
	}



	int hashInternal(const u8*& k, u32& length, u32& a, u32& b, u32& c)
	{
		while (length > 12)
		{
			a += k[0];
			a += ((u32)k[1])<<8;
			a += ((u32)k[2])<<16;
			a += ((u32)k[3])<<24;
			b += k[4];
			b += ((u32)k[5])<<8;
			b += ((u32)k[6])<<16;
			b += ((u32)k[7])<<24;
			c += k[8];
			c += ((u32)k[9])<<8;
			c += ((u32)k[10])<<16;
			c += ((u32)k[11])<<24;
			mix(a,b,c);
			length -= 12;
			k += 12;
		}

		switch(length)
		{
		case 12: c+=((u32)k[11])<<24;
		case 11: c+=((u32)k[10])<<16;
		case 10: c+=((u32)k[9])<<8;
		case 9 : c+=k[8];
		case 8 : b+=((u32)k[7])<<24;
		case 7 : b+=((u32)k[6])<<16;
		case 6 : b+=((u32)k[5])<<8;
		case 5 : b+=k[4];
		case 4 : a+=((u32)k[3])<<24;
		case 3 : a+=((u32)k[2])<<16;
		case 2 : a+=((u32)k[1])<<8;
		case 1 : a+=k[0];
		case 0 : break;
		}

		k += length;
		return length;
	}

	static void Hash2_LE(void const* inData, u32 inLength, u32& pc, u32& pb)
	{
		u32 a,b,c;
		a = b = c = 0xdeadbeef + ((u32)inLength) + pc;
		c += pb;

		if ((((int)inData & 0x3) == 0))
		{
			const u32 *k = (const u32 *)inData;

			while (inLength > 12)
			{
				a += k[0];
				b += k[1];
				c += k[2];
				mix(a,b,c);
				inLength -= 12;
				k += 3;
			}

#ifndef VALGRIND

			switch(inLength)
			{
			case 12: c+=k[2]; b+=k[1]; a+=k[0]; break;
			case 11: c+=k[2]&0xffffff; b+=k[1]; a+=k[0]; break;
			case 10: c+=k[2]&0xffff; b+=k[1]; a+=k[0]; break;
			case 9 : c+=k[2]&0xff; b+=k[1]; a+=k[0]; break;
			case 8 : b+=k[1]; a+=k[0]; break;
			case 7 : b+=k[1]&0xffffff; a+=k[0]; break;
			case 6 : b+=k[1]&0xffff; a+=k[0]; break;
			case 5 : b+=k[1]&0xff; a+=k[0]; break;
			case 4 : a+=k[0]; break;
			case 3 : a+=k[0]&0xffffff; break;
			case 2 : a+=k[0]&0xffff; break;
			case 1 : a+=k[0]&0xff; break;
			case 0 : pc=c; pb=b; return;
			}

#else /* make valgrind happy */

			const u8* k8 = (const u8*)k;
			switch(inLength)
			{
			case 12: c+=k[2]; b+=k[1]; a+=k[0]; break;
			case 11: c+=((u32)k8[10])<<16;		
			case 10: c+=((u32)k8[9])<<8;			
			case 9 : c+=k8[8];						
			case 8 : b+=k[1]; a+=k[0]; break;
			case 7 : b+=((u32)k8[6])<<16;		
			case 6 : b+=((u32)k8[5])<<8;			
			case 5 : b+=k8[4];						
			case 4 : a+=k[0]; break;
			case 3 : a+=((u32)k8[2])<<16;		
			case 2 : a+=((u32)k8[1])<<8;			
			case 1 : a+=k8[0]; break;
			case 0 : pc=c; pb=b; return;			
			}

#endif /* !valgrind */

		}
		else if ((((int)inData & 0x1) == 0))
		{
			// read 16-bit chunks
			const u16 *k = (const u16 *)inData;
			const u8  *k8;

			while (inLength > 12)
			{
				a += k[0] + (((u32)k[1])<<16);
				b += k[2] + (((u32)k[3])<<16);
				c += k[4] + (((u32)k[5])<<16);
				mix(a,b,c);
				inLength -= 12;
				k += 6;
			}

			k8 = (const u8 *)k;
			switch(inLength)
			{
			case 12:
				c+=k[4]+(((u32)k[5])<<16);
				b+=k[2]+(((u32)k[3])<<16);
				a+=k[0]+(((u32)k[1])<<16);
				break;
			case 11:
				c+=((u32)k8[10])<<16;		
			case 10:
				c+=k[4];
				b+=k[2]+(((u32)k[3])<<16);
				a+=k[0]+(((u32)k[1])<<16);
				break;
			case 9:
				c+=k8[8];						
			case 8:
				b+=k[2]+(((u32)k[3])<<16);
				a+=k[0]+(((u32)k[1])<<16);
				break;
			case 7:
				b+=((u32)k8[6])<<16;		
			case 6:
				b+=k[2];
				a+=k[0]+(((u32)k[1])<<16);
				break;
			case 5:
				b+=k8[4];						
			case 4:
				a+=k[0]+(((u32)k[1])<<16);
				break;
			case 3:
				a+=((u32)k8[2])<<16;		
			case 2:
				a+=k[0];
				break;
			case 1:
				a+=k8[0];
				break;
			case 0:
				pc=c;
				pb=b; 
				return;
			}

		}
		else
		{
			const u8* k = (const u8*)inData;
			if (hashInternal(k,inLength,a,b,c)==0)
			{
				pc = c;
				pb = b;
				return;
			}
		}

		final(a,b,c);

		pc=c;
		pb=b;
	}


	static u32 Hash2_LE(void const* inData, u32 inLength, u32 pc)
	{
		u32 pb = 0;
		Hash2_LE(inData, inLength, pc, pb);
		return pc;
	}


	/*
	hash():
	This is the same as hash() on big-endian machines. It is different
	from little endian hash() on all machines. This hash() takes advantage
	of big-endian byte ordering. 
	*/
	static void Hash2_BE(void const* inData, u32 inLength, u32& pc, u32& pb)
	{
		u32 a,b,c;
		a = b = c = 0xdeadbeef + ((u32)inLength) + pc;
		c += pb;

		if ((((int)inData & 0x3) == 0))
		{
			const u32 *k = (const u32 *)inData;         /* read 32-bit chunks */

			while (inLength > 12)
			{
				a += k[0];
				b += k[1];
				c += k[2];
				mix(a,b,c);
				inLength -= 12;
				k += 3;
			}

#ifndef VALGRIND

			switch(inLength)
			{
			case 12: c+=k[2]; b+=k[1]; a+=k[0]; break;
			case 11: c+=k[2]&0xffffff00; b+=k[1]; a+=k[0]; break;
			case 10: c+=k[2]&0xffff0000; b+=k[1]; a+=k[0]; break;
			case 9 : c+=k[2]&0xff000000; b+=k[1]; a+=k[0]; break;
			case 8 : b+=k[1]; a+=k[0]; break;
			case 7 : b+=k[1]&0xffffff00; a+=k[0]; break;
			case 6 : b+=k[1]&0xffff0000; a+=k[0]; break;
			case 5 : b+=k[1]&0xff000000; a+=k[0]; break;
			case 4 : a+=k[0]; break;
			case 3 : a+=k[0]&0xffffff00; break;
			case 2 : a+=k[0]&0xffff0000; break;
			case 1 : a+=k[0]&0xff000000; break;
			case 0 : pc=c; pb=b; return;
			}

#else  /* make valgrind happy */

			const u8* k8 = (const u8 *)k;
			switch(inLength)
			{
			case 12: c+=k[2]; b+=k[1]; a+=k[0]; break;
			case 11: c+=((u32)k8[10])<<8;  
			case 10: c+=((u32)k8[9])<<16;  
			case 9 : c+=((u32)k8[8])<<24;  
			case 8 : b+=k[1]; a+=k[0]; break;
			case 7 : b+=((u32)k8[6])<<8;   
			case 6 : b+=((u32)k8[5])<<16;  
			case 5 : b+=((u32)k8[4])<<24;  
			case 4 : a+=k[0]; break;
			case 3 : a+=((u32)k8[2])<<8;   
			case 2 : a+=((u32)k8[1])<<16;  
			case 1 : a+=((u32)k8[0])<<24; break;
			case 0 : pc=c; pb=b; return;
			}

#endif /* !VALGRIND */

		}
		else
		{
			const u8* k = (const u8 *)inData;
			if (hashInternal(k,inLength,a,b,c)==0)
			{
				pc = c;
				pb = b;
				return;
			}
		}

		final(a,b,c);

		pc=c;
		pb=b;
	}

	static u32 Hash2_BE(void const* inData, u32 inLength, u32 pc)
	{
		u32 pb = 0;
		Hash2_BE(inData, inLength, pc, pb);
		return pc;
	}

	static xbool IsBigEndian()
	{
		u32 val = 0x01020304;
		xbyte* ptr = (xbyte*)&val;
		return ptr[3] == 4;
	}

	void		Hashing::Hash2(void const* inData, u32 inLength, u32& pc, u32& pb)
	{
		if (IsBigEndian())
			Hash2_BE(inData, inLength, pc, pb);
		else
			Hash2_LE(inData, inLength, pc, pb);
	}

	u32			Hashing::Hash2(void const* inData, u32 inLength, u32 pc)
	{
		if (IsBigEndian())
			return Hash2_BE(inData, inLength, pc);
		else
			return Hash2_LE(inData, inLength, pc);
	}

	void		Hashing::Hash2(u32 const* inData, u32 inLength, u32& pc, u32& pb)
	{
		if (IsBigEndian())
			Hash2_BE(inData, inLength, pc, pb);
		else
			Hash2_LE(inData, inLength, pc, pb);
	}

	u32			Hashing::Hash2(u32 const* inData, u32 inLength, u32 pc)
	{
		if (IsBigEndian())
			return Hash2_BE(inData, inLength, pc);
		else
			return Hash2_LE(inData, inLength, pc);
	}
}
