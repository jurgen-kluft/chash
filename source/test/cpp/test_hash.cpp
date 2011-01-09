#include "xbase\x_target.h"
#include "xbase\x_string_std.h"
#include "xhash\xhash.h"

#include "xunittest\xunittest.h"

using namespace xcore;


UNITTEST_SUITE_BEGIN(xhash)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(hash)
		{
		}
	}
}
UNITTEST_SUITE_END


/* used for timings */
void driver1()
{
	u8 buf[256];
	u32 i;
	u32 h=0;

	for (i=0; i<256; ++i)
		buf[i] = 'x';

	for (i=0; i<1; ++i) 
	{
		h = Hashing::Hash2((const void*)&buf[0],1,h);
	}
}

/* check that every input bit changes every output bit half the time */
#define HASHSTATE 1
#define HASHLEN   1
#define MAXPAIR 60
#define MAXLEN  70
void driver2()
{
	u8 qa[MAXLEN+1], qb[MAXLEN+2], *a = &qa[0], *b = &qb[1];
	u32 c[HASHSTATE], d[HASHSTATE], i=0, j=0, k, l, m=0, z;
	u32 e[HASHSTATE],f[HASHSTATE],g[HASHSTATE],h[HASHSTATE];
	u32 x[HASHSTATE],y[HASHSTATE];
	u32 hlen;

	x_printf("No more than %d trials should ever be needed \n", x_va(MAXPAIR/2));
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
						e[l]=f[l]=g[l]=h[l]=x[l]=y[l]=~((u32)0);

					/*---- check that every output bit is affected by that input bit */
					for (k=0; k<MAXPAIR; k+=2)
					{ 
						u32 finished=1;
						/* keys have one bit different */
						for (l=0; l<hlen+1; ++l) {a[l] = b[l] = (u8)0;}
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
						x_printf("Some bit didn't change: ");
						x_printf("%.8x %.8x %.8x %.8x %.8x %.8x  ", x_va_list(x_va(e[0]), x_va(f[0]), x_va(g[0]), x_va(h[0]), x_va(x[0]), x_va(y[0])));
						x_printf("i %d j %d m %d len %d\n", x_va_list(x_va(i), x_va(j), x_va(m), x_va(hlen)));
					}
					if (z==MAXPAIR) goto done;
				}
			}
		}
done:
		if (z < MAXPAIR)
		{
			x_printf("Mix success  %2d bytes  %2d initvals  ", x_va_list(x_va(i),x_va(m)));
			x_printf("required  %d  trials\n", x_va_list(x_va(z/2)));
		}
	}
	x_printf("\n");
}

/* Check for reading beyond the end of the buffer and alignment problems */
void driver3()
{
	u8 buf[MAXLEN+20], *b;
	u32 len;
	u8 q[] = "This is the time for all good men to come to the aid of their country...";
	u32 h;
	u8 qq[] = "xThis is the time for all good men to come to the aid of their country...";
	u32 i;
	u8 qqq[] = "xxThis is the time for all good men to come to the aid of their country...";
	u32 j;
	u8 qqqq[] = "xxxThis is the time for all good men to come to the aid of their country...";
	u32 ref,x,y;
	u8 *p;

	x_printf("%.8x     %.8x     %.8x     %.8x\n", x_va_list(
		x_va(Hashing::Hash2(q, (sizeof(q)-1), 13)),
		x_va(Hashing::Hash2(qq, (sizeof(qq)-1), 13)),
		x_va(Hashing::Hash2(qqq, (sizeof(qqq)-1), 13)),
		x_va(Hashing::Hash2(qqqq, (sizeof(qqqq)-1), 13))));

	x_printf("Endianness.  These lines should all be the same (for values filled in):\n");
	x_printf("%.8x                            %.8x                            %.8x\n", x_va_list(
		x_va(Hashing::Hash2((const u32 *)q, (sizeof(q)-1)/4, 13)),
		x_va(Hashing::Hash2((const u32 *)q, (sizeof(q)-5)/4, 13)),
		x_va(Hashing::Hash2((const u32 *)q, (sizeof(q)-9)/4, 13))));
	p = q;
	x_printf("%.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x\n", x_va_list(
		x_va(Hashing::Hash2(p, sizeof(q)-1 , 13)), x_va(Hashing::Hash2(p, sizeof(q)-2, 13)),
		x_va(Hashing::Hash2(p, sizeof(q)-3 , 13)), x_va(Hashing::Hash2(p, sizeof(q)-4, 13)),
		x_va(Hashing::Hash2(p, sizeof(q)-5 , 13)), x_va(Hashing::Hash2(p, sizeof(q)-6, 13)),
		x_va(Hashing::Hash2(p, sizeof(q)-7 , 13)), x_va(Hashing::Hash2(p, sizeof(q)-8, 13)),
		x_va(Hashing::Hash2(p, sizeof(q)-9 , 13)), x_va(Hashing::Hash2(p, sizeof(q)-10, 13)),
		x_va(Hashing::Hash2(p, sizeof(q)-11, 13)), x_va(Hashing::Hash2(p, sizeof(q)-12, 13))));
	p = &qq[1];
	x_printf("%.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x\n", x_va_list(
		x_va(Hashing::Hash2(p, sizeof(q)-1 , 13)), x_va(Hashing::Hash2(p, sizeof(q)-2, 13)),
		x_va(Hashing::Hash2(p, sizeof(q)-3 , 13)), x_va(Hashing::Hash2(p, sizeof(q)-4, 13)),
		x_va(Hashing::Hash2(p, sizeof(q)-5 , 13)), x_va(Hashing::Hash2(p, sizeof(q)-6, 13)),
		x_va(Hashing::Hash2(p, sizeof(q)-7 , 13)), x_va(Hashing::Hash2(p, sizeof(q)-8, 13)),
		x_va(Hashing::Hash2(p, sizeof(q)-9 , 13)), x_va(Hashing::Hash2(p, sizeof(q)-10, 13)),
		x_va(Hashing::Hash2(p, sizeof(q)-11, 13)), x_va(Hashing::Hash2(p, sizeof(q)-12, 13))));
	p = &qqq[2];
	x_printf("%.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x\n", x_va_list(
		x_va(Hashing::Hash2(p, sizeof(q)-1 , 13)), x_va(Hashing::Hash2(p, sizeof(q)-2, 13)),
		x_va(Hashing::Hash2(p, sizeof(q)-3 , 13)), x_va(Hashing::Hash2(p, sizeof(q)-4, 13)),
		x_va(Hashing::Hash2(p, sizeof(q)-5 , 13)), x_va(Hashing::Hash2(p, sizeof(q)-6, 13)),
		x_va(Hashing::Hash2(p, sizeof(q)-7 , 13)), x_va(Hashing::Hash2(p, sizeof(q)-8, 13)),
		x_va(Hashing::Hash2(p, sizeof(q)-9 , 13)), x_va(Hashing::Hash2(p, sizeof(q)-10, 13)),
		x_va(Hashing::Hash2(p, sizeof(q)-11, 13)), x_va(Hashing::Hash2(p, sizeof(q)-12, 13))));
	p = &qqqq[3];
	x_printf("%.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x %.8x\n", x_va_list(
		x_va(Hashing::Hash2(p, sizeof(q)-1 , 13)), x_va(Hashing::Hash2(p, sizeof(q)-2, 13)),
		x_va(Hashing::Hash2(p, sizeof(q)-3 , 13)), x_va(Hashing::Hash2(p, sizeof(q)-4, 13)),
		x_va(Hashing::Hash2(p, sizeof(q)-5 , 13)), x_va(Hashing::Hash2(p, sizeof(q)-6, 13)),
		x_va(Hashing::Hash2(p, sizeof(q)-7 , 13)), x_va(Hashing::Hash2(p, sizeof(q)-8, 13)),
		x_va(Hashing::Hash2(p, sizeof(q)-9 , 13)), x_va(Hashing::Hash2(p, sizeof(q)-10, 13)),
		x_va(Hashing::Hash2(p, sizeof(q)-11, 13)), x_va(Hashing::Hash2(p, sizeof(q)-12, 13))));
	x_printf("\n");

	/* check that hashlittle2 and hashlittle produce the same results */
	i=47; j=0;
	u32 l=sizeof(q);
	void const* d=q;
	Hashing::Hash2(d, l, i, j);
	if (Hashing::Hash2(q, sizeof(q), 47) != i)
		x_printf("hashlittle2 and hashlittle mismatch\n");

	/* check that hashword2 and hashword produce the same results */
	len = 0xdeadbeef;
	i=47, j=0;
	Hashing::Hash2(&len, 1, i, j);
	if (Hashing::Hash2(&len, 1, 47) != i)
		x_printf("hashword2 and hashword mismatch %x %x\n", x_va_list(x_va(i), x_va(Hashing::Hash2(&len, 1, 47))));

	/* check hashlittle doesn't read before or after the ends of the string */
	for (h=0, b=buf+1; h<8; ++h, ++b)
	{
		for (i=0; i<MAXLEN; ++i)
		{
			len = i;
			for (j=0; j<i; ++j)
				*(b+j)=0;

			/* these should all be equal */
			ref = Hashing::Hash2(b, len, (u32)1);
			*(b+i)=(u8)~0;
			*(b-1)=(u8)~0;
			x = Hashing::Hash2(b, len, (u32)1);
			y = Hashing::Hash2(b, len, (u32)1);
			if ((ref != x) || (ref != y)) 
			{
				x_printf("alignment error: %.8x %.8x %.8x %d %d\n",x_va_list(x_va(ref),x_va(x),x_va(y),x_va(h),x_va(i)));
			}
		}
	}
}

/* check for problems with nulls */
void driver4()
{
	u8 buf[1];
	u32 h,i,state[HASHSTATE];

	buf[0] = ~0;
	for (i=0; i<HASHSTATE; ++i) state[i] = 1;
	x_printf("These should all be different\n");
	for (i=0, h=0; i<8; ++i)
	{
		h = Hashing::Hash2(buf, 0, h);
		x_printf("%2ld  0-byte strings, hash is  %.8x\n", x_va_list(x_va(i), x_va(h)));
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
