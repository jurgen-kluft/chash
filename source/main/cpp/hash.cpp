// Hash.cpp - Core Hash functions 
#include "CSystem\CSystem.h"
#include "CHash\Hash.h"
#include "CSystem\Endian.h"

BEGIN_CORE_NAMESPACE


//
// Hash Mix utility function, by Bob Jenkins (see pigs_checksum.cpp for more info) - Mix 3 32-bit values reversibly
//
// - If gHashMix() is run forward or backward, at least 32 bits in a,b,c have at
//   least 1/4 probability of changing.
//
// - If gHashMix() is run forward, every bit of c will change between 1/3 and
//   2/3 of the time.
//
static inline void	sHashMix(uint32& a, uint32& b, uint32& c)
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
uint32		Hashing::Hash(pcVoid inData, uint32 inLength, uint32 inInitValue)
{
	uint  length = inLength;
	uint32 a = (uint32)GOLDEN_RATIO;
	uint32 b = (uint32)GOLDEN_RATIO;
	uint32 c = inInitValue;															// the previous hash value
	uint8  *p_in = (uint8 *)inData;

	// Do the largest part of the key
	while (length >= 12)
	{
		a += (p_in[0] + ((uint32)p_in[1]<<8) + ((uint32)p_in[2] <<16) + ((uint32)p_in[3] <<24));
		b += (p_in[4] + ((uint32)p_in[5]<<8) + ((uint32)p_in[6] <<16) + ((uint32)p_in[7] <<24));
		c += (p_in[8] + ((uint32)p_in[9]<<8) + ((uint32)p_in[10]<<16) + ((uint32)p_in[11]<<24));
		sHashMix(a, b, c);
		p_in += 12; length -= 12;
	}

	// Handle the last 11 bytes
	c += inLength;
	switch(length)
	{
		case 11: c+=((uint32)p_in[10]<<24);
		case 10: c+=((uint32)p_in[9]<<16);
		case 9 : c+=((uint32)p_in[8]<<8);											// the first byte of c is reserved for the length
		case 8 : b+=((uint32)p_in[7]<<24);
		case 7 : b+=((uint32)p_in[6]<<16);
		case 6 : b+=((uint32)p_in[5]<<8);
		case 5 : b+=p_in[4];
		case 4 : a+=((uint32)p_in[3]<<24);
		case 3 : a+=((uint32)p_in[2]<<16);
		case 2 : a+=((uint32)p_in[1]<<8);
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

#define		hashsize(n)			((uint32)1<<(n))
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
	-- that the key be an array of uint32's, and
	-- that the length be the number of uint32's in the key

	The function hash() is identical to hashlittle() on little-endian
	machines, and identical to hashbig() on big-endian machines,
	except that the length has to be measured in uint32s rather than in
	bytes.  hashlittle() is more complicated than hash() only because
	hashlittle() has to dance around fitting the key bytes into registers.
	--------------------------------------------------------------------
*/

void Hashing::Hash2(pcUInt32 inData, uint32 inLength, uint32& pc, uint32& pb)
{
	uint32 a,b,c;
	a = b = c = 0xdeadbeef + ((uint32)(inLength<<2)) + pc;
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


uint32 Hashing::Hash2(pcUInt32 inData, uint32 inLength, uint32 pc)         								
{
	uint32 pb = 0;
	Hash2(inData, inLength, pc, pb);
	return pc;
}



int hashInternal(const uint8*& k, uint32& length, uint32& a, uint32& b, uint32& c)
{
	while (length > 12)
	{
		a += k[0];
		a += ((uint32)k[1])<<8;
		a += ((uint32)k[2])<<16;
		a += ((uint32)k[3])<<24;
		b += k[4];
		b += ((uint32)k[5])<<8;
		b += ((uint32)k[6])<<16;
		b += ((uint32)k[7])<<24;
		c += k[8];
		c += ((uint32)k[9])<<8;
		c += ((uint32)k[10])<<16;
		c += ((uint32)k[11])<<24;
		mix(a,b,c);
		length -= 12;
		k += 12;
	}

	switch(length)
	{
		case 12: c+=((uint32)k[11])<<24;
		case 11: c+=((uint32)k[10])<<16;
		case 10: c+=((uint32)k[9])<<8;
		case 9 : c+=k[8];
		case 8 : b+=((uint32)k[7])<<24;
		case 7 : b+=((uint32)k[6])<<16;
		case 6 : b+=((uint32)k[5])<<8;
		case 5 : b+=k[4];
		case 4 : a+=((uint32)k[3])<<24;
		case 3 : a+=((uint32)k[2])<<16;
		case 2 : a+=((uint32)k[1])<<8;
		case 1 : a+=k[0];
		case 0 : break;
	}

	k += length;
	return length;
}



#ifdef LITTLE_ENDIAN

void Hashing::Hash2(pcVoid inData, uint32 inLength, uint32& pc, uint32& pb)
{
	uint32 a,b,c;
	a = b = c = 0xdeadbeef + ((uint32)inLength) + pc;
	c += pb;

	if ((((int)inData & 0x3) == 0))
	{
		const uint32 *k = (const uint32 *)inData;

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
			case 0 : *pc=c; *pb=b; return;
		}

#else /* make valgrind happy */

		const uint8* k8 = (const uint8*)k;
		switch(inLength)
		{
			case 12: c+=k[2]; b+=k[1]; a+=k[0]; break;
			case 11: c+=((uint32)k8[10])<<16;		
			case 10: c+=((uint32)k8[9])<<8;			
			case 9 : c+=k8[8];						
			case 8 : b+=k[1]; a+=k[0]; break;
			case 7 : b+=((uint32)k8[6])<<16;		
			case 6 : b+=((uint32)k8[5])<<8;			
			case 5 : b+=k8[4];						
			case 4 : a+=k[0]; break;
			case 3 : a+=((uint32)k8[2])<<16;		
			case 2 : a+=((uint32)k8[1])<<8;			
			case 1 : a+=k8[0]; break;
			case 0 : pc=c; pb=b; return;			
		}

#endif /* !valgrind */

	}
	else if ((((int)inData & 0x1) == 0))
	{
		// read 16-bit chunks
		const uint16 *k = (const uint16 *)inData;
		const uint8  *k8;

		while (inLength > 12)
		{
			a += k[0] + (((uint32)k[1])<<16);
			b += k[2] + (((uint32)k[3])<<16);
			c += k[4] + (((uint32)k[5])<<16);
			mix(a,b,c);
			inLength -= 12;
			k += 6;
		}

		k8 = (const uint8 *)k;
		switch(inLength)
		{
			case 12:
				c+=k[4]+(((uint32)k[5])<<16);
				b+=k[2]+(((uint32)k[3])<<16);
				a+=k[0]+(((uint32)k[1])<<16);
				break;
			case 11:
				c+=((uint32)k8[10])<<16;		
			case 10:
				c+=k[4];
				b+=k[2]+(((uint32)k[3])<<16);
				a+=k[0]+(((uint32)k[1])<<16);
				break;
			case 9:
				c+=k8[8];						
			case 8:
				b+=k[2]+(((uint32)k[3])<<16);
				a+=k[0]+(((uint32)k[1])<<16);
				break;
			case 7:
				b+=((uint32)k8[6])<<16;		
			case 6:
				b+=k[2];
				a+=k[0]+(((uint32)k[1])<<16);
				break;
			case 5:
				b+=k8[4];						
			case 4:
				a+=k[0]+(((uint32)k[1])<<16);
				break;
			case 3:
				a+=((uint32)k8[2])<<16;		
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
		const uint8* k = (const uint8*)inData;
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


uint32 Hashing::Hash2(pcVoid inData, uint32 inLength, uint32 pc)
{
	uint32 pb = 0;
	Hash2(inData, inLength, pc, pb);
	return pc;
}



#endif	// LITTLE_ENDIAN


#ifdef BIG_ENDIAN

/*
	hash():
	This is the same as hash() on big-endian machines. It is different
	from little endian hash() on all machines. This hash() takes advantage
	of big-endian byte ordering. 
*/
uint32 Hashing::Hash2(pcVoid inData, uint32 inLength, uint32 initval)
{
	uint32 a,b,c;
	a = b = c = 0xdeadbeef + ((uint32)inLength) + initval;

	if ((((int)inData & 0x3) == 0))
	{
		const uint32 *k = (const uint32 *)inData;         /* read 32-bit chunks */

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
			case 0 : return c;
		}

#else  /* make valgrind happy */

		const uint8* k8 = (const uint8 *)k;
		switch(inLength)
		{
			case 12: c+=k[2]; b+=k[1]; a+=k[0]; break;
			case 11: c+=((uint32)k8[10])<<8;  
			case 10: c+=((uint32)k8[9])<<16;  
			case 9 : c+=((uint32)k8[8])<<24;  
			case 8 : b+=k[1]; a+=k[0]; break;
			case 7 : b+=((uint32)k8[6])<<8;   
			case 6 : b+=((uint32)k8[5])<<16;  
			case 5 : b+=((uint32)k8[4])<<24;  
			case 4 : a+=k[0]; break;
			case 3 : a+=((uint32)k8[2])<<8;   
			case 2 : a+=((uint32)k8[1])<<16;  
			case 1 : a+=((uint32)k8[0])<<24; break;
			case 0 : return c;
		}

#endif /* !VALGRIND */

	}
	else
	{
		const uint8* k = (const uint8 *)inData;
		if (hashInternal(k,inLength,a,b,c)==0)
			return c;
	}

	final(a,b,c);
	return c;
}


#endif	///< BIG_ENDIAN





#ifdef SELF_TEST

/* used for timings */
void driver1()
{
	uint8 buf[256];
	uint32 i;
	uint32 h=0;
	time_t a,z;

	time(&a);
	for (i=0; i<256; ++i)
		buf[i] = 'x';

	for (i=0; i<1; ++i) 
	{
		h = Hashing::Hash2((const void*)&buf[0],1,h);
	}
	time(&z);
	if (z-a > 0)
		Tracer::Trace("time %d %.8x\n", xva_list(xva(z-a), xva(h)));
}

/* check that every input bit changes every output bit half the time */
#define HASHSTATE 1
#define HASHLEN   1
#define MAXPAIR 60
#define MAXLEN  70
void driver2()
{
	uint8 qa[MAXLEN+1], qb[MAXLEN+2], *a = &qa[0], *b = &qb[1];
	uint32 c[HASHSTATE], d[HASHSTATE], i=0, j=0, k, l, m=0, z;
	uint32 e[HASHSTATE],f[HASHSTATE],g[HASHSTATE],h[HASHSTATE];
	uint32 x[HASHSTATE],y[HASHSTATE];
	uint32 hlen;

	printf("No more than %d trials should ever be needed \n",MAXPAIR/2);
	for (hlen=0; hlen < MAXLEN; ++hlen)
	{
		z=0;
		for (i=0; i<hlen; ++i)  /*----------------------- for each input byte, */
		{
			for (j=0; j<8; ++j)   /*------------------------ for each input bit, */
			{
				for (m=1; m<8; ++m) /*------------ for serveral possible initvals, */
				{
					for (l=0; l<HASHSTATE; ++l)
						e[l]=f[l]=g[l]=h[l]=x[l]=y[l]=~((uint32)0);

					/*---- check that every output bit is affected by that input bit */
					for (k=0; k<MAXPAIR; k+=2)
					{ 
						uint32 finished=1;
						/* keys have one bit different */
						for (l=0; l<hlen+1; ++l) {a[l] = b[l] = (uint8)0;}
						/* have a and b be two keys differing in only one bit */
						a[i] ^= (k<<j);
						a[i] ^= (k>>(8-j));
						c[0] = Hashing::Hash2(a, hlen, m);
						b[i] ^= ((k+1)<<j);
						b[i] ^= ((k+1)>>(8-j));
						d[0] = Hashing::Hash2(b, hlen, m);
						/* check every bit is 1, 0, set, and not set at least once */
						for (l=0; l<HASHSTATE; ++l)
						{
							e[l] &= (c[l]^d[l]);
							f[l] &= ~(c[l]^d[l]);
							g[l] &= c[l];
							h[l] &= ~c[l];
							x[l] &= d[l];
							y[l] &= ~d[l];
							if (e[l]|f[l]|g[l]|h[l]|x[l]|y[l]) finished=0;
						}
						if (finished) break;
					}
					if (k>z) z=k;
					if (k==MAXPAIR) 
					{
						printf("Some bit didn't change: ");
						printf("%.8x %.8x %.8x %.8x %.8x %.8x  ",
							e[0],f[0],g[0],h[0],x[0],y[0]);
						printf("i %d j %d m %d len %d\n", i, j, m, hlen);
					}
					if (z==MAXPAIR) goto done;
				}
			}
		}
done:
		if (z < MAXPAIR)
		{
			printf("Mix success  %2d bytes  %2d initvals  ",i,m);
			printf("required  %d  trials\n", z/2);
		}
	}
	printf("\n");
}

/* Check for reading beyond the end of the buffer and alignment problems */
void driver3()
{
	uint8 buf[MAXLEN+20], *b;
	uint32 len;
	uint8 q[] = "This is the time for all good men to come to the aid of their country...";
	uint32 h;
	uint8 qq[] = "xThis is the time for all good men to come to the aid of their country...";
	uint32 i;
	uint8 qqq[] = "xxThis is the time for all good men to come to the aid of their country...";
	uint32 j;
	uint8 qqqq[] = "xxxThis is the time for all good men to come to the aid of their country...";
	uint32 ref,x,y;
	uint8 *p;

	printf("%.8x     %.8x     %.8x     %.8x\n",
		Hashing::Hash2(q, (sizeof(q)-1), 13),
		Hashing::Hash2(qq, (sizeof(qq)-1), 13),
		Hashing::Hash2(qqq, (sizeof(qqq)-1), 13),
		Hashing::Hash2(qqqq, (sizeof(qqqq)-1), 13));

	printf("Endianness.  These lines should all be the same (for values filled in):\n");
	printf("%.8x                            %.8x                            %.8x\n",
		Hashing::Hash2((const uint32 *)q, (sizeof(q)-1)/4, 13),
		Hashing::Hash2((const uint32 *)q, (sizeof(q)-5)/4, 13),
		Hashing::Hash2((const uint32 *)q, (sizeof(q)-9)/4, 13));
	p = q;
	printf("%.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x\n",
		Hashing::Hash2(p, sizeof(q)-1, 13), Hashing::Hash2(p, sizeof(q)-2, 13),
		Hashing::Hash2(p, sizeof(q)-3, 13), Hashing::Hash2(p, sizeof(q)-4, 13),
		Hashing::Hash2(p, sizeof(q)-5, 13), Hashing::Hash2(p, sizeof(q)-6, 13),
		Hashing::Hash2(p, sizeof(q)-7, 13), Hashing::Hash2(p, sizeof(q)-8, 13),
		Hashing::Hash2(p, sizeof(q)-9, 13), Hashing::Hash2(p, sizeof(q)-10, 13),
		Hashing::Hash2(p, sizeof(q)-11, 13), Hashing::Hash2(p, sizeof(q)-12, 13));
	p = &qq[1];
	printf("%.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x\n",
		Hashing::Hash2(p, sizeof(q)-1, 13), Hashing::Hash2(p, sizeof(q)-2, 13),
		Hashing::Hash2(p, sizeof(q)-3, 13), Hashing::Hash2(p, sizeof(q)-4, 13),
		Hashing::Hash2(p, sizeof(q)-5, 13), Hashing::Hash2(p, sizeof(q)-6, 13),
		Hashing::Hash2(p, sizeof(q)-7, 13), Hashing::Hash2(p, sizeof(q)-8, 13),
		Hashing::Hash2(p, sizeof(q)-9, 13), Hashing::Hash2(p, sizeof(q)-10, 13),
		Hashing::Hash2(p, sizeof(q)-11, 13), Hashing::Hash2(p, sizeof(q)-12, 13));
	p = &qqq[2];
	printf("%.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x\n",
		Hashing::Hash2(p, sizeof(q)-1, 13), Hashing::Hash2(p, sizeof(q)-2, 13),
		Hashing::Hash2(p, sizeof(q)-3, 13), Hashing::Hash2(p, sizeof(q)-4, 13),
		Hashing::Hash2(p, sizeof(q)-5, 13), Hashing::Hash2(p, sizeof(q)-6, 13),
		Hashing::Hash2(p, sizeof(q)-7, 13), Hashing::Hash2(p, sizeof(q)-8, 13),
		Hashing::Hash2(p, sizeof(q)-9, 13), Hashing::Hash2(p, sizeof(q)-10, 13),
		Hashing::Hash2(p, sizeof(q)-11, 13), Hashing::Hash2(p, sizeof(q)-12, 13));
	p = &qqqq[3];
	printf("%.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x\n",
		Hashing::Hash2(p, sizeof(q)-1, 13), Hashing::Hash2(p, sizeof(q)-2, 13),
		Hashing::Hash2(p, sizeof(q)-3, 13), Hashing::Hash2(p, sizeof(q)-4, 13),
		Hashing::Hash2(p, sizeof(q)-5, 13), Hashing::Hash2(p, sizeof(q)-6, 13),
		Hashing::Hash2(p, sizeof(q)-7, 13), Hashing::Hash2(p, sizeof(q)-8, 13),
		Hashing::Hash2(p, sizeof(q)-9, 13), Hashing::Hash2(p, sizeof(q)-10, 13),
		Hashing::Hash2(p, sizeof(q)-11, 13), Hashing::Hash2(p, sizeof(q)-12, 13));
	printf("\n");

	/* check that hashlittle2 and hashlittle produce the same results */
	i=47; j=0;
	uint32 l=sizeof(q);
	pcVoid d=q;
	Hashing::Hash2(d, l, i, j);
	if (Hashing::Hash2(q, sizeof(q), 47) != i)
		printf("hashlittle2 and hashlittle mismatch\n");

	/* check that hashword2 and hashword produce the same results */
	len = 0xdeadbeef;
	i=47, j=0;
	Hashing::Hash2(&len, 1, i, j);
	if (Hashing::Hash2(&len, 1, 47) != i)
		printf("hashword2 and hashword mismatch %x %x\n", i, Hashing::Hash2(&len, 1, 47));

	/* check hashlittle doesn't read before or after the ends of the string */
	for (h=0, b=buf+1; h<8; ++h, ++b)
	{
		for (i=0; i<MAXLEN; ++i)
		{
			len = i;
			for (j=0; j<i; ++j)
				*(b+j)=0;

			/* these should all be equal */
			ref = Hashing::Hash2(b, len, (uint32)1);
			*(b+i)=(uint8)~0;
			*(b-1)=(uint8)~0;
			x = Hashing::Hash2(b, len, (uint32)1);
			y = Hashing::Hash2(b, len, (uint32)1);
			if ((ref != x) || (ref != y)) 
			{
				printf("alignment error: %.8x %.8x %.8x %d %d\n",ref,x,y,h,i);
			}
		}
	}
}

/* check for problems with nulls */
void driver4()
{
	uint8 buf[1];
	uint32 h,i,state[HASHSTATE];

	buf[0] = ~0;
	for (i=0; i<HASHSTATE; ++i) state[i] = 1;
	printf("These should all be different\n");
	for (i=0, h=0; i<8; ++i)
	{
		h = Hashing::Hash2(buf, 0, h);
		printf("%2ld  0-byte strings, hash is  %.8x\n", i, h);
	}
}


int TestLookup3()
{
	driver1();   /* test that the key is hashed: used for timings */
	driver2();   /* test that whole key is hashed thoroughly */
	driver3();   /* test that nothing but the key is hashed */
	driver4();   /* test hashing multiple buffers (all buffers are null) */
	return 1;
}

#endif  /* SELF_TEST */

#if 0	// The OUTPUT
No more than 30 trials should ever be needed 
Mix success   0 bytes  3795 initvals  required  0  trials
Mix success   1 bytes   8 initvals  required  13  trials
Mix success   2 bytes   8 initvals  required  13  trials
Mix success   3 bytes   8 initvals  required  14  trials
Mix success   4 bytes   8 initvals  required  16  trials
Mix success   5 bytes   8 initvals  required  16  trials
Mix success   6 bytes   8 initvals  required  15  trials
Mix success   7 bytes   8 initvals  required  16  trials
Mix success   8 bytes   8 initvals  required  21  trials
Mix success   9 bytes   8 initvals  required  21  trials
Mix success  10 bytes   8 initvals  required  21  trials
Mix success  11 bytes   8 initvals  required  21  trials
Mix success  12 bytes   8 initvals  required  16  trials
Mix success  13 bytes   8 initvals  required  18  trials
Mix success  14 bytes   8 initvals  required  18  trials
Mix success  15 bytes   8 initvals  required  18  trials
Mix success  16 bytes   8 initvals  required  18  trials
Mix success  17 bytes   8 initvals  required  18  trials
Mix success  18 bytes   8 initvals  required  18  trials
Mix success  19 bytes   8 initvals  required  18  trials
Mix success  20 bytes   8 initvals  required  15  trials
Mix success  21 bytes   8 initvals  required  16  trials
Mix success  22 bytes   8 initvals  required  19  trials
Mix success  23 bytes   8 initvals  required  19  trials
Mix success  24 bytes   8 initvals  required  19  trials
Mix success  25 bytes   8 initvals  required  18  trials
Mix success  26 bytes   8 initvals  required  18  trials
Mix success  27 bytes   8 initvals  required  18  trials
Mix success  28 bytes   8 initvals  required  17  trials
Mix success  29 bytes   8 initvals  required  16  trials
Mix success  30 bytes   8 initvals  required  16  trials
Mix success  31 bytes   8 initvals  required  16  trials
Mix success  32 bytes   8 initvals  required  17  trials
Mix success  33 bytes   8 initvals  required  17  trials
Mix success  34 bytes   8 initvals  required  17  trials
Mix success  35 bytes   8 initvals  required  16  trials
Mix success  36 bytes   8 initvals  required  16  trials
Mix success  37 bytes   8 initvals  required  19  trials
Mix success  38 bytes   8 initvals  required  19  trials
Mix success  39 bytes   8 initvals  required  19  trials
Mix success  40 bytes   8 initvals  required  19  trials
Mix success  41 bytes   8 initvals  required  19  trials
Mix success  42 bytes   8 initvals  required  19  trials
Mix success  43 bytes   8 initvals  required  18  trials
Mix success  44 bytes   8 initvals  required  18  trials
Mix success  45 bytes   8 initvals  required  19  trials
Mix success  46 bytes   8 initvals  required  18  trials
Mix success  47 bytes   8 initvals  required  18  trials
Mix success  48 bytes   8 initvals  required  18  trials
Mix success  49 bytes   8 initvals  required  18  trials
Mix success  50 bytes   8 initvals  required  18  trials
Mix success  51 bytes   8 initvals  required  18  trials
Mix success  52 bytes   8 initvals  required  18  trials
Mix success  53 bytes   8 initvals  required  18  trials
Mix success  54 bytes   8 initvals  required  17  trials
Mix success  55 bytes   8 initvals  required  18  trials
Mix success  56 bytes   8 initvals  required  18  trials
Mix success  57 bytes   8 initvals  required  18  trials
Mix success  58 bytes   8 initvals  required  18  trials
Mix success  59 bytes   8 initvals  required  18  trials
Mix success  60 bytes   8 initvals  required  18  trials
Mix success  61 bytes   8 initvals  required  20  trials
Mix success  62 bytes   8 initvals  required  20  trials
Mix success  63 bytes   8 initvals  required  20  trials
Mix success  64 bytes   8 initvals  required  20  trials
Mix success  65 bytes   8 initvals  required  20  trials
Mix success  66 bytes   8 initvals  required  20  trials
Mix success  67 bytes   8 initvals  required  19  trials
Mix success  68 bytes   8 initvals  required  19  trials
Mix success  69 bytes   8 initvals  required  19  trials

Endianness.  These lines should all be the same (for values filled in):
499ae8fa                            b1946734                            f4688dd0
499ae8fa b9bef31c 8efefdfd a56b7aab b1946734 9f31c5ce 0826585d 55b69dea f4688dd0 e87eb146 b202fb17 711fe56a
499ae8fa b9bef31c 8efefdfd a56b7aab b1946734 9f31c5ce 0826585d 55b69dea f4688dd0 e87eb146 b202fb17 711fe56a
499ae8fa b9bef31c 8efefdfd a56b7aab b1946734 9f31c5ce 0826585d 55b69dea f4688dd0 e87eb146 b202fb17 711fe56a
499ae8fa b9bef31c 8efefdfd a56b7aab b1946734 9f31c5ce 0826585d 55b69dea f4688dd0 e87eb146 b202fb17 711fe56a

These should all be different
0  0-byte strings, hash is  deadbeef
1  0-byte strings, hash is  bd5b7dde
2  0-byte strings, hash is  9c093ccd
3  0-byte strings, hash is  7ab6fbbc
4  0-byte strings, hash is  5964baab
5  0-byte strings, hash is  3812799a
6  0-byte strings, hash is  16c03889
7  0-byte strings, hash is  f56df778

#endif




END_CORE_NAMESPACE
