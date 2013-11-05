#include "xbase\x_target.h"
#include "xhash\x_skein.h"
#include "xhash\private\x_digest_murmur32.h"

#include <string.h>


namespace skein_hash
{
	typedef xcore::u32        uint_t;             /* native unsigned integer */
	typedef xcore::u8         u08b_t;             /*  8-bit unsigned integer */
	typedef xcore::u64        u64b_t;             /* 64-bit unsigned integer */
	typedef xcore::u32        size_t;

	/*
	 * Skein is "natively" little-endian (unlike SHA-xxx), for optimal
	 * performance on x86 CPUs.  The Skein code requires the following
	 * definitions for dealing with endianness:
	 *
	 *    Skein_Put64_LSB_First
	 *    Skein_Get64_LSB_First
	 *    Skein_Swap64
	 *
	 * In the reference code, these functions are implemented in a
	 * very portable (and thus slow) fashion, for clarity. See the file
	 * "skein_port.h" in the Optimized_Code directory for ways to make
	 * these functions fast(er) on x86 platforms.
	 */

	u64b_t Skein_Swap64(u64b_t w64);
	void   Skein_Put64_LSB_First(u08b_t *dst,const u64b_t *src,size_t bCnt);
	void   Skein_Get64_LSB_First(u64b_t *dst,const u08b_t *src,size_t wCnt);







	enum
	{
		SKEIN_SUCCESS         =      0,          /* return codes from Skein calls */
		SKEIN_FAIL            =      1,
		SKEIN_BAD_HASHLEN     =      2
	};

#define  SKEIN_MODIFIER_WORDS  ( 2)          /* number of modifier (tweak) words */

#define  SKEIN_256_STATE_WORDS ( 4)
#define  SKEIN_512_STATE_WORDS ( 8)
#define  SKEIN1024_STATE_WORDS (16)
#define  SKEIN_MAX_STATE_WORDS (16)

#define  SKEIN_256_STATE_BYTES ( 8*SKEIN_256_STATE_WORDS)
#define  SKEIN_512_STATE_BYTES ( 8*SKEIN_512_STATE_WORDS)
#define  SKEIN1024_STATE_BYTES ( 8*SKEIN1024_STATE_WORDS)

#define  SKEIN_256_STATE_BITS  (64*SKEIN_256_STATE_WORDS)
#define  SKEIN_512_STATE_BITS  (64*SKEIN_512_STATE_WORDS)
#define  SKEIN1024_STATE_BITS  (64*SKEIN1024_STATE_WORDS)

#define  SKEIN_256_BLOCK_BYTES ( 8*SKEIN_256_STATE_WORDS)
#define  SKEIN_512_BLOCK_BYTES ( 8*SKEIN_512_STATE_WORDS)
#define  SKEIN1024_BLOCK_BYTES ( 8*SKEIN1024_STATE_WORDS)

	typedef struct
	{
		size_t  hashBitLen;                      /* size of hash result, in bits */
		size_t  bCnt;                            /* current byte count in buffer b[] */
		u64b_t  T[SKEIN_MODIFIER_WORDS];         /* tweak words: T[0]=byte cnt, T[1]=flags */
	} Skein_Ctxt_Hdr_t;

	typedef struct                               /*  256-bit Skein hash context structure */
	{
		Skein_Ctxt_Hdr_t h;                      /* common header context variables */
		u64b_t  X[SKEIN_256_STATE_WORDS];        /* chaining variables */
		u08b_t  b[SKEIN_256_BLOCK_BYTES];        /* partial block buffer (8-byte aligned) */
	} Skein_256_Ctxt_t;

	typedef struct                               /*  512-bit Skein hash context structure */
	{
		Skein_Ctxt_Hdr_t h;                      /* common header context variables */
		u64b_t  X[SKEIN_512_STATE_WORDS];        /* chaining variables */
		u08b_t  b[SKEIN_512_BLOCK_BYTES];        /* partial block buffer (8-byte aligned) */
	} Skein_512_Ctxt_t;

	typedef struct                               /* 1024-bit Skein hash context structure */
	{
		Skein_Ctxt_Hdr_t h;                      /* common header context variables */
		u64b_t  X[SKEIN1024_STATE_WORDS];        /* chaining variables */
		u08b_t  b[SKEIN1024_BLOCK_BYTES];        /* partial block buffer (8-byte aligned) */
	} Skein1024_Ctxt_t;

	/*   Skein APIs for (incremental) "straight hashing" */
	int  Skein_256_Init  (Skein_256_Ctxt_t *ctx, size_t hashBitLen);
	int  Skein_512_Init  (Skein_512_Ctxt_t *ctx, size_t hashBitLen);
	int  Skein1024_Init  (Skein1024_Ctxt_t *ctx, size_t hashBitLen);

	int  Skein_256_Update(Skein_256_Ctxt_t *ctx, const u08b_t *msg, size_t msgByteCnt);
	int  Skein_512_Update(Skein_512_Ctxt_t *ctx, const u08b_t *msg, size_t msgByteCnt);
	int  Skein1024_Update(Skein1024_Ctxt_t *ctx, const u08b_t *msg, size_t msgByteCnt);

	int  Skein_256_Final (Skein_256_Ctxt_t *ctx, u08b_t * hashVal);
	int  Skein_512_Final (Skein_512_Ctxt_t *ctx, u08b_t * hashVal);
	int  Skein1024_Final (Skein1024_Ctxt_t *ctx, u08b_t * hashVal);

	/*
	**   Skein APIs for "extended" initialization: MAC keys, tree hashing.
	**   After an InitExt() call, just use Update/Final calls as with Init().
	**
	**   Notes: Same parameters as _Init() calls, plus treeInfo/key/keyBytes.
	**          When keyBytes == 0 and treeInfo == SKEIN_SEQUENTIAL, 
	**              the results of InitExt() are identical to calling Init().
	**          The function Init() may be called once to "precompute" the IV for
	**              a given hashBitLen value, then by saving a copy of the context
	**              the IV computation may be avoided in later calls.
	**          Similarly, the function InitExt() may be called once per MAC key 
	**              to precompute the MAC IV, then a copy of the context saved and
	**              reused for each new MAC computation.
	**/
	int  Skein_256_InitExt(Skein_256_Ctxt_t *ctx, size_t hashBitLen, u64b_t treeInfo, const u08b_t *key, size_t keyBytes);
	int  Skein_512_InitExt(Skein_512_Ctxt_t *ctx, size_t hashBitLen, u64b_t treeInfo, const u08b_t *key, size_t keyBytes);
	int  Skein1024_InitExt(Skein1024_Ctxt_t *ctx, size_t hashBitLen, u64b_t treeInfo, const u08b_t *key, size_t keyBytes);

	/*
	**   Skein APIs for MAC and tree hash:
	**      Final_Pad:  pad, do final block, but no OUTPUT type
	**      Output:     do just the output stage
	*/
	int  Skein_256_Final_Pad(Skein_256_Ctxt_t *ctx, u08b_t * hashVal);
	int  Skein_512_Final_Pad(Skein_512_Ctxt_t *ctx, u08b_t * hashVal);
	int  Skein1024_Final_Pad(Skein1024_Ctxt_t *ctx, u08b_t * hashVal);

#ifndef SKEIN_TREE_HASH
	#define SKEIN_TREE_HASH (1)
#endif
#if  SKEIN_TREE_HASH
	int  Skein_256_Output   (Skein_256_Ctxt_t *ctx, u08b_t * hashVal);
	int  Skein_512_Output   (Skein_512_Ctxt_t *ctx, u08b_t * hashVal);
	int  Skein1024_Output   (Skein1024_Ctxt_t *ctx, u08b_t * hashVal);
#endif

	/*****************************************************************
	** "Internal" Skein definitions
	**    -- not needed for sequential hashing API, but will be 
	**           helpful for other uses of Skein (e.g., tree hash mode).
	**    -- included here so that they can be shared between
	**           reference and optimized code.
	******************************************************************/

	/* tweak word T[1]: bit field starting positions */
	#define SKEIN_T1_BIT(BIT)       ((BIT) - 64)            /* offset 64 because it's the second word  */

	#define SKEIN_T1_POS_TREE_LVL   SKEIN_T1_BIT(112)       /* bits 112..118: level in hash tree       */
	#define SKEIN_T1_POS_BIT_PAD    SKEIN_T1_BIT(119)       /* bit  119     : partial final input byte */
	#define SKEIN_T1_POS_BLK_TYPE   SKEIN_T1_BIT(120)       /* bits 120..125: type field               */
	#define SKEIN_T1_POS_FIRST      SKEIN_T1_BIT(126)       /* bits 126     : first block flag         */
	#define SKEIN_T1_POS_FINAL      SKEIN_T1_BIT(127)       /* bit  127     : final block flag         */

	/* tweak word T[1]: flag bit definition(s) */
	#define SKEIN_T1_FLAG_FIRST     (((u64b_t)  1 ) << SKEIN_T1_POS_FIRST)
	#define SKEIN_T1_FLAG_FINAL     (((u64b_t)  1 ) << SKEIN_T1_POS_FINAL)
	#define SKEIN_T1_FLAG_BIT_PAD   (((u64b_t)  1 ) << SKEIN_T1_POS_BIT_PAD)

	/* tweak word T[1]: tree level bit field mask */
	#define SKEIN_T1_TREE_LVL_MASK  (((u64b_t)0x7F) << SKEIN_T1_POS_TREE_LVL)
	#define SKEIN_T1_TREE_LEVEL(n)  (((u64b_t) (n)) << SKEIN_T1_POS_TREE_LVL)

	/* tweak word T[1]: block type field */
	#define SKEIN_BLK_TYPE_KEY      ( 0)                    /* key, for MAC and KDF */
	#define SKEIN_BLK_TYPE_CFG      ( 4)                    /* configuration block */
	#define SKEIN_BLK_TYPE_PERS     ( 8)                    /* personalization string */
	#define SKEIN_BLK_TYPE_PK       (12)                    /* public key (for digital signature hashing) */
	#define SKEIN_BLK_TYPE_KDF      (16)                    /* key identifier for KDF */
	#define SKEIN_BLK_TYPE_NONCE    (20)                    /* nonce for PRNG */
	#define SKEIN_BLK_TYPE_MSG      (48)                    /* message processing */
	#define SKEIN_BLK_TYPE_OUT      (63)                    /* output stage */
	#define SKEIN_BLK_TYPE_MASK     (63)                    /* bit field mask */

	#define SKEIN_T1_BLK_TYPE(T)   (((u64b_t) (SKEIN_BLK_TYPE_##T)) << SKEIN_T1_POS_BLK_TYPE)
	#define SKEIN_T1_BLK_TYPE_KEY   SKEIN_T1_BLK_TYPE(KEY)  /* key, for MAC and KDF */
	#define SKEIN_T1_BLK_TYPE_CFG   SKEIN_T1_BLK_TYPE(CFG)  /* configuration block */
	#define SKEIN_T1_BLK_TYPE_PERS  SKEIN_T1_BLK_TYPE(PERS) /* personalization string */
	#define SKEIN_T1_BLK_TYPE_PK    SKEIN_T1_BLK_TYPE(PK)   /* public key (for digital signature hashing) */
	#define SKEIN_T1_BLK_TYPE_KDF   SKEIN_T1_BLK_TYPE(KDF)  /* key identifier for KDF */
	#define SKEIN_T1_BLK_TYPE_NONCE SKEIN_T1_BLK_TYPE(NONCE)/* nonce for PRNG */
	#define SKEIN_T1_BLK_TYPE_MSG   SKEIN_T1_BLK_TYPE(MSG)  /* message processing */
	#define SKEIN_T1_BLK_TYPE_OUT   SKEIN_T1_BLK_TYPE(OUT)  /* output stage */
	#define SKEIN_T1_BLK_TYPE_MASK  SKEIN_T1_BLK_TYPE(MASK) /* field bit mask */

	#define SKEIN_T1_BLK_TYPE_CFG_FINAL       (SKEIN_T1_BLK_TYPE_CFG | SKEIN_T1_FLAG_FINAL)
	#define SKEIN_T1_BLK_TYPE_OUT_FINAL       (SKEIN_T1_BLK_TYPE_OUT | SKEIN_T1_FLAG_FINAL)

	#define SKEIN_VERSION           (1)

	#ifndef SKEIN_ID_STRING_LE      /* allow compile-time personalization */
	#define SKEIN_ID_STRING_LE      (0x33414853)            /* "SHA3" (little-endian)*/
	#endif

	#define SKEIN_MK_64(hi32,lo32)  ((lo32) + (((u64b_t) (hi32)) << 32))
	#define SKEIN_SCHEMA_VER        SKEIN_MK_64(SKEIN_VERSION,SKEIN_ID_STRING_LE)
	#define SKEIN_KS_PARITY         SKEIN_MK_64(0x55555555,0x55555555)

	#define SKEIN_CFG_STR_LEN       (4*8)

	/* bit field definitions in config block treeInfo word */
	#define SKEIN_CFG_TREE_LEAF_SIZE_POS  ( 0)
	#define SKEIN_CFG_TREE_NODE_SIZE_POS  ( 8)
	#define SKEIN_CFG_TREE_MAX_LEVEL_POS  (16)

	#define SKEIN_CFG_TREE_LEAF_SIZE_MSK  ((u64b_t) 0xFF) << SKEIN_CFG_TREE_LEAF_SIZE_POS)
	#define SKEIN_CFG_TREE_NODE_SIZE_MSK  ((u64b_t) 0xFF) << SKEIN_CFG_TREE_NODE_SIZE_POS)
	#define SKEIN_CFG_TREE_MAX_LEVEL_MSK  ((u64b_t) 0xFF) << SKEIN_CFG_TREE_MAX_LEVEL_POS)

	#define SKEIN_CFG_TREE_INFO_SEQUENTIAL (0) /* use as treeInfo in InitExt() call for sequential processing */
	#define SKEIN_CFG_TREE_INFO(leaf,node,maxLevel) ((u64b_t) ((leaf) | ((node) << 8) | ((maxLevel) << 16)))

	/*
	**   Skein macros for getting/setting tweak words, etc.
	**   These are useful for partial input bytes, hash tree init/update, etc.
	**/
	#define Skein_Get_Tweak(ctxPtr,TWK_NUM)         ((ctxPtr)->h.T[TWK_NUM])
	#define Skein_Set_Tweak(ctxPtr,TWK_NUM,tVal)    {(ctxPtr)->h.T[TWK_NUM] = (tVal);}

	#define Skein_Get_T0(ctxPtr)    Skein_Get_Tweak(ctxPtr,0)
	#define Skein_Get_T1(ctxPtr)    Skein_Get_Tweak(ctxPtr,1)
	#define Skein_Set_T0(ctxPtr,T0) Skein_Set_Tweak(ctxPtr,0,T0)
	#define Skein_Set_T1(ctxPtr,T1) Skein_Set_Tweak(ctxPtr,1,T1)

	/* set both tweak words at once */
	#define Skein_Set_T0_T1(ctxPtr,T0,T1)           \
	{                                           \
	Skein_Set_T0(ctxPtr,(T0));                  \
	Skein_Set_T1(ctxPtr,(T1));                  \
	}

	#define Skein_Set_Type(ctxPtr,BLK_TYPE)         \
	Skein_Set_T1(ctxPtr,SKEIN_T1_BLK_TYPE_##BLK_TYPE)

	/* set up for starting with a new type: h.T[0]=0; h.T[1] = NEW_TYPE; h.bCnt=0; */
	#define Skein_Start_New_Type(ctxPtr,BLK_TYPE)   \
	{ Skein_Set_T0_T1(ctxPtr,0,SKEIN_T1_FLAG_FIRST | SKEIN_T1_BLK_TYPE_##BLK_TYPE); (ctxPtr)->h.bCnt=0; }

	#define Skein_Clear_First_Flag(hdr)      { (hdr).T[1] &= ~SKEIN_T1_FLAG_FIRST;       }
	#define Skein_Set_Bit_Pad_Flag(hdr)      { (hdr).T[1] |=  SKEIN_T1_FLAG_BIT_PAD;     }

	#define Skein_Set_Tree_Level(hdr,height) { (hdr).T[1] |= SKEIN_T1_TREE_LEVEL(height);}

	/*****************************************************************
	** "Internal" Skein definitions for debugging and error checking
	******************************************************************/
	#define Skein_Show_Block(bits,ctx,X,blkPtr,wPtr,ksEvenPtr,ksOddPtr)
	#define Skein_Show_Round(bits,ctx,r,X)
	#define Skein_Show_R_Ptr(bits,ctx,r,X_ptr)
	#define Skein_Show_Final(bits,ctx,cnt,outPtr)
	#define Skein_Show_Key(bits,ctx,key,keyBytes)


	#define Skein_Assert(x,retCode)/* default: ignore all Asserts, for performance */
	#define Skein_assert(x)


	/*****************************************************************
	** Skein block function constants (shared across Ref and Opt code)
	******************************************************************/
	enum    
	{   
		/* Skein_256 round rotation constants */
		R_256_0_0= 5, R_256_0_1=56,
		R_256_1_0=36, R_256_1_1=28,
		R_256_2_0=13, R_256_2_1=46,
		R_256_3_0=58, R_256_3_1=44,
		R_256_4_0=26, R_256_4_1=20,
		R_256_5_0=53, R_256_5_1=35,
		R_256_6_0=11, R_256_6_1=42,
		R_256_7_0=59, R_256_7_1=50,

		/* Skein_512 round rotation constants */
		R_512_0_0=38, R_512_0_1=30, R_512_0_2=50, R_512_0_3=53,
		R_512_1_0=48, R_512_1_1=20, R_512_1_2=43, R_512_1_3=31,
		R_512_2_0=34, R_512_2_1=14, R_512_2_2=15, R_512_2_3=27,
		R_512_3_0=26, R_512_3_1=12, R_512_3_2=58, R_512_3_3= 7,
		R_512_4_0=33, R_512_4_1=49, R_512_4_2= 8, R_512_4_3=42,
		R_512_5_0=39, R_512_5_1=27, R_512_5_2=41, R_512_5_3=14,
		R_512_6_0=29, R_512_6_1=26, R_512_6_2=11, R_512_6_3= 9,
		R_512_7_0=33, R_512_7_1=51, R_512_7_2=39, R_512_7_3=35,

		/* Skein1024 round rotation constants */
		R1024_0_0=55, R1024_0_1=43, R1024_0_2=37, R1024_0_3=40, R1024_0_4=16, R1024_0_5=22, R1024_0_6=38, R1024_0_7=12,
		R1024_1_0=25, R1024_1_1=25, R1024_1_2=46, R1024_1_3=13, R1024_1_4=14, R1024_1_5=13, R1024_1_6=52, R1024_1_7=57,
		R1024_2_0=33, R1024_2_1= 8, R1024_2_2=18, R1024_2_3=57, R1024_2_4=21, R1024_2_5=12, R1024_2_6=32, R1024_2_7=54,
		R1024_3_0=34, R1024_3_1=43, R1024_3_2=25, R1024_3_3=60, R1024_3_4=44, R1024_3_5= 9, R1024_3_6=59, R1024_3_7=34,
		R1024_4_0=28, R1024_4_1= 7, R1024_4_2=47, R1024_4_3=48, R1024_4_4=51, R1024_4_5= 9, R1024_4_6=35, R1024_4_7=41,
		R1024_5_0=17, R1024_5_1= 6, R1024_5_2=18, R1024_5_3=25, R1024_5_4=43, R1024_5_5=42, R1024_5_6=40, R1024_5_7=15,
		R1024_6_0=58, R1024_6_1= 7, R1024_6_2=32, R1024_6_3=45, R1024_6_4=19, R1024_6_5=18, R1024_6_6= 2, R1024_6_7=56,
		R1024_7_0=47, R1024_7_1=49, R1024_7_2=27, R1024_7_3=58, R1024_7_4=37, R1024_7_5=48, R1024_7_6=53, R1024_7_7=56
	};

#ifndef SKEIN_ROUNDS
#define SKEIN_256_ROUNDS_TOTAL (72)          /* number of rounds for the different block sizes */
#define SKEIN_512_ROUNDS_TOTAL (72)
#define SKEIN1024_ROUNDS_TOTAL (80)
#else                                        /* allow command-line define in range 8*(5..14)   */
#define SKEIN_256_ROUNDS_TOTAL (8*((((SKEIN_ROUNDS/100) + 5) % 10) + 5))
#define SKEIN_512_ROUNDS_TOTAL (8*((((SKEIN_ROUNDS/ 10) + 5) % 10) + 5))
#define SKEIN1024_ROUNDS_TOTAL (8*((((SKEIN_ROUNDS    ) + 5) % 10) + 5))
#endif









	/* 64-bit rotate left */
	u64b_t RotL_64(u64b_t x,uint_t N)
	{
		return (x << (N & 63)) | (x >> ((64-N) & 63));
	}

#define BLK_BITS    (WCNT*64)

	/* macro to perform a key injection (same for all block sizes) */
#define InjectKey(r)                                                \
	for (i=0;i < WCNT;i++)                                          \
	X[i] += ks[((r)+i) % (WCNT+1)];                            \
	X[WCNT-3] += ts[((r)+0) % 3];                                   \
	X[WCNT-2] += ts[((r)+1) % 3];                                   \
	X[WCNT-1] += (r);                    /* avoid slide attacks */  \
	Skein_Show_Round(BLK_BITS,&ctx->h,SKEIN_RND_KEY_INJECT,X);

	void Skein_256_Process_Block(Skein_256_Ctxt_t *ctx,const u08b_t *blkPtr,size_t blkCnt,size_t byteCntAdd)
	{ /* do it in C */
		enum
		{
			WCNT     = SKEIN_256_STATE_WORDS
		};
		size_t  i,r;
		u64b_t  ts[3];                            /* key schedule: tweak */
		u64b_t  ks[WCNT+1];                       /* key schedule: chaining vars */
		u64b_t  X [WCNT];                         /* local copy of context vars */
		u64b_t  w [WCNT];                         /* local copy of input block */

		Skein_assert(blkCnt != 0);                /* never call with blkCnt == 0! */
		do  {
			/* this implementation only supports 2**64 input bytes (no carry out here) */
			ctx->h.T[0] += byteCntAdd;            /* update processed length */

			/* precompute the key schedule for this block */
			ks[WCNT] = SKEIN_KS_PARITY;
			for (i=0;i < WCNT; i++)
			{
				ks[i]     = ctx->X[i];
				ks[WCNT] ^= ctx->X[i];            /* compute overall parity */
			}
			ts[0] = ctx->h.T[0];
			ts[1] = ctx->h.T[1];
			ts[2] = ts[0] ^ ts[1];

			Skein_Get64_LSB_First(w,blkPtr,WCNT); /* get input block in little-endian format */
			Skein_Show_Block(BLK_BITS,&ctx->h,ctx->X,blkPtr,w,ks,ts);
			for (i=0;i < WCNT; i++)               /* do the first full key injection */
			{
				X[i]  = w[i] + ks[i];
			}
			X[WCNT-3] += ts[0];
			X[WCNT-2] += ts[1];

			Skein_Show_Round(BLK_BITS,&ctx->h,SKEIN_RND_KEY_INITIAL,X);     /* show starting state values */

			for (r=1;r <= SKEIN_256_ROUNDS_TOTAL/8; r++)
			{ /* unroll 8 rounds */
				X[0] += X[1]; X[1] = RotL_64(X[1],R_256_0_0); X[1] ^= X[0];
				X[2] += X[3]; X[3] = RotL_64(X[3],R_256_0_1); X[3] ^= X[2];  Skein_Show_Round(BLK_BITS,&ctx->h,8*r-7,X);

				X[0] += X[3]; X[3] = RotL_64(X[3],R_256_1_0); X[3] ^= X[0];
				X[2] += X[1]; X[1] = RotL_64(X[1],R_256_1_1); X[1] ^= X[2];  Skein_Show_Round(BLK_BITS,&ctx->h,8*r-6,X);

				X[0] += X[1]; X[1] = RotL_64(X[1],R_256_2_0); X[1] ^= X[0];
				X[2] += X[3]; X[3] = RotL_64(X[3],R_256_2_1); X[3] ^= X[2];  Skein_Show_Round(BLK_BITS,&ctx->h,8*r-5,X);

				X[0] += X[3]; X[3] = RotL_64(X[3],R_256_3_0); X[3] ^= X[0];
				X[2] += X[1]; X[1] = RotL_64(X[1],R_256_3_1); X[1] ^= X[2];  Skein_Show_Round(BLK_BITS,&ctx->h,8*r-4,X);
				InjectKey(2*r-1);

				X[0] += X[1]; X[1] = RotL_64(X[1],R_256_4_0); X[1] ^= X[0];
				X[2] += X[3]; X[3] = RotL_64(X[3],R_256_4_1); X[3] ^= X[2];  Skein_Show_Round(BLK_BITS,&ctx->h,8*r-3,X);

				X[0] += X[3]; X[3] = RotL_64(X[3],R_256_5_0); X[3] ^= X[0];
				X[2] += X[1]; X[1] = RotL_64(X[1],R_256_5_1); X[1] ^= X[2];  Skein_Show_Round(BLK_BITS,&ctx->h,8*r-2,X);

				X[0] += X[1]; X[1] = RotL_64(X[1],R_256_6_0); X[1] ^= X[0];
				X[2] += X[3]; X[3] = RotL_64(X[3],R_256_6_1); X[3] ^= X[2];  Skein_Show_Round(BLK_BITS,&ctx->h,8*r-1,X);

				X[0] += X[3]; X[3] = RotL_64(X[3],R_256_7_0); X[3] ^= X[0];
				X[2] += X[1]; X[1] = RotL_64(X[1],R_256_7_1); X[1] ^= X[2];  Skein_Show_Round(BLK_BITS,&ctx->h,8*r  ,X);
				InjectKey(2*r);
			}
			/* do the final "feedforward" xor, update context chaining vars */
			for (i=0;i < WCNT;i++)
				ctx->X[i] = X[i] ^ w[i];
			Skein_Show_Round(BLK_BITS,&ctx->h,SKEIN_RND_FEED_FWD,ctx->X);

			Skein_Clear_First_Flag(ctx->h);		/* clear the start bit */
			blkPtr += SKEIN_256_BLOCK_BYTES;
		}
		while (--blkCnt);
	}

#if defined(SKEIN_CODE_SIZE) || defined(SKEIN_PERF)
	size_t Skein_256_Process_Block_CodeSize(void)
	{
		return ((u08b_t *) Skein_256_Process_Block_CodeSize) -
			((u08b_t *) Skein_256_Process_Block);
	}
	uint_t Skein_256_Unroll_Cnt(void)
	{
		return 1;
	}
#endif

	void Skein_512_Process_Block(Skein_512_Ctxt_t *ctx,const u08b_t *blkPtr,size_t blkCnt,size_t byteCntAdd)
	{ /* do it in C */
		enum
		{
			WCNT = SKEIN_512_STATE_WORDS
		};

		size_t  i,r;
		u64b_t  ts[3];                            /* key schedule: tweak */
		u64b_t  ks[WCNT+1];                       /* key schedule: chaining vars */
		u64b_t  X [WCNT];                         /* local copy of vars */
		u64b_t  w [WCNT];                         /* local copy of input block */

		Skein_assert(blkCnt != 0);                /* never call with blkCnt == 0! */
		do  {
			/* this implementation only supports 2**64 input bytes (no carry out here) */
			ctx->h.T[0] += byteCntAdd;            /* update processed length */

			/* precompute the key schedule for this block */
			ks[WCNT] = SKEIN_KS_PARITY;
			for (i=0;i < WCNT; i++)
			{
				ks[i]     = ctx->X[i];
				ks[WCNT] ^= ctx->X[i];            /* compute overall parity */
			}
			ts[0] = ctx->h.T[0];
			ts[1] = ctx->h.T[1];
			ts[2] = ts[0] ^ ts[1];

			Skein_Get64_LSB_First(w,blkPtr,WCNT); /* get input block in little-endian format */
			Skein_Show_Block(BLK_BITS,&ctx->h,ctx->X,blkPtr,w,ks,ts);
			for (i=0;i < WCNT; i++)               /* do the first full key injection */
			{
				X[i]  = w[i] + ks[i];
			}
			X[WCNT-3] += ts[0];
			X[WCNT-2] += ts[1];

			Skein_Show_Round(BLK_BITS,&ctx->h,SKEIN_RND_KEY_INITIAL,X);
			for (r=1;r <= SKEIN_512_ROUNDS_TOTAL/8; r++)
			{ /* unroll 8 rounds */
				X[0] += X[1]; X[1] = RotL_64(X[1],R_512_0_0); X[1] ^= X[0];
				X[2] += X[3]; X[3] = RotL_64(X[3],R_512_0_1); X[3] ^= X[2];
				X[4] += X[5]; X[5] = RotL_64(X[5],R_512_0_2); X[5] ^= X[4];
				X[6] += X[7]; X[7] = RotL_64(X[7],R_512_0_3); X[7] ^= X[6];  Skein_Show_Round(BLK_BITS,&ctx->h,8*r-7,X);

				X[2] += X[1]; X[1] = RotL_64(X[1],R_512_1_0); X[1] ^= X[2];
				X[4] += X[7]; X[7] = RotL_64(X[7],R_512_1_1); X[7] ^= X[4];
				X[6] += X[5]; X[5] = RotL_64(X[5],R_512_1_2); X[5] ^= X[6];
				X[0] += X[3]; X[3] = RotL_64(X[3],R_512_1_3); X[3] ^= X[0];  Skein_Show_Round(BLK_BITS,&ctx->h,8*r-6,X);

				X[4] += X[1]; X[1] = RotL_64(X[1],R_512_2_0); X[1] ^= X[4];
				X[6] += X[3]; X[3] = RotL_64(X[3],R_512_2_1); X[3] ^= X[6];
				X[0] += X[5]; X[5] = RotL_64(X[5],R_512_2_2); X[5] ^= X[0];
				X[2] += X[7]; X[7] = RotL_64(X[7],R_512_2_3); X[7] ^= X[2];  Skein_Show_Round(BLK_BITS,&ctx->h,8*r-5,X);

				X[6] += X[1]; X[1] = RotL_64(X[1],R_512_3_0); X[1] ^= X[6];
				X[0] += X[7]; X[7] = RotL_64(X[7],R_512_3_1); X[7] ^= X[0];
				X[2] += X[5]; X[5] = RotL_64(X[5],R_512_3_2); X[5] ^= X[2];
				X[4] += X[3]; X[3] = RotL_64(X[3],R_512_3_3); X[3] ^= X[4];  Skein_Show_Round(BLK_BITS,&ctx->h,8*r-4,X);
				InjectKey(2*r-1);

				X[0] += X[1]; X[1] = RotL_64(X[1],R_512_4_0); X[1] ^= X[0];
				X[2] += X[3]; X[3] = RotL_64(X[3],R_512_4_1); X[3] ^= X[2];
				X[4] += X[5]; X[5] = RotL_64(X[5],R_512_4_2); X[5] ^= X[4];
				X[6] += X[7]; X[7] = RotL_64(X[7],R_512_4_3); X[7] ^= X[6];  Skein_Show_Round(BLK_BITS,&ctx->h,8*r-3,X);

				X[2] += X[1]; X[1] = RotL_64(X[1],R_512_5_0); X[1] ^= X[2];
				X[4] += X[7]; X[7] = RotL_64(X[7],R_512_5_1); X[7] ^= X[4];
				X[6] += X[5]; X[5] = RotL_64(X[5],R_512_5_2); X[5] ^= X[6];
				X[0] += X[3]; X[3] = RotL_64(X[3],R_512_5_3); X[3] ^= X[0];  Skein_Show_Round(BLK_BITS,&ctx->h,8*r-2,X);

				X[4] += X[1]; X[1] = RotL_64(X[1],R_512_6_0); X[1] ^= X[4];
				X[6] += X[3]; X[3] = RotL_64(X[3],R_512_6_1); X[3] ^= X[6];
				X[0] += X[5]; X[5] = RotL_64(X[5],R_512_6_2); X[5] ^= X[0];
				X[2] += X[7]; X[7] = RotL_64(X[7],R_512_6_3); X[7] ^= X[2];  Skein_Show_Round(BLK_BITS,&ctx->h,8*r-1,X);

				X[6] += X[1]; X[1] = RotL_64(X[1],R_512_7_0); X[1] ^= X[6];
				X[0] += X[7]; X[7] = RotL_64(X[7],R_512_7_1); X[7] ^= X[0];
				X[2] += X[5]; X[5] = RotL_64(X[5],R_512_7_2); X[5] ^= X[2];
				X[4] += X[3]; X[3] = RotL_64(X[3],R_512_7_3); X[3] ^= X[4];  Skein_Show_Round(BLK_BITS,&ctx->h,8*r  ,X);
				InjectKey(2*r);
			}
			/* do the final "feedforward" xor, update context chaining vars */
			for (i=0;i < WCNT;i++)
				ctx->X[i] = X[i] ^ w[i];
			Skein_Show_Round(BLK_BITS,&ctx->h,SKEIN_RND_FEED_FWD,ctx->X);

			Skein_Clear_First_Flag(ctx->h);		/* clear the start bit */
			blkPtr += SKEIN_512_BLOCK_BYTES;
		}
		while (--blkCnt);
	}

#if defined(SKEIN_CODE_SIZE) || defined(SKEIN_PERF)
	size_t Skein_512_Process_Block_CodeSize(void)
	{
		return ((u08b_t *) Skein_512_Process_Block_CodeSize) -
			((u08b_t *) Skein_512_Process_Block);
	}
	uint_t Skein_512_Unroll_Cnt(void)
	{
		return 1;
	}
#endif

	void Skein1024_Process_Block(Skein1024_Ctxt_t *ctx,const u08b_t *blkPtr,size_t blkCnt,size_t byteCntAdd)
	{ /* do it in C */
		enum
		{
			WCNT = SKEIN1024_STATE_WORDS
		};

		size_t  i,r;
		u64b_t  ts[3];                            /* key schedule: tweak */
		u64b_t  ks[WCNT+1];                       /* key schedule: chaining vars */
		u64b_t  X [WCNT];                         /* local copy of vars */
		u64b_t  w [WCNT];                         /* local copy of input block */

		Skein_assert(blkCnt != 0);                /* never call with blkCnt == 0! */
		do  {
			/* this implementation only supports 2**64 input bytes (no carry out here) */
			ctx->h.T[0] += byteCntAdd;            /* update processed length */

			/* precompute the key schedule for this block */
			ks[WCNT] = SKEIN_KS_PARITY;
			for (i=0;i < WCNT; i++)
			{
				ks[i]     = ctx->X[i];
				ks[WCNT] ^= ctx->X[i];            /* compute overall parity */
			}
			ts[0] = ctx->h.T[0];
			ts[1] = ctx->h.T[1];
			ts[2] = ts[0] ^ ts[1];

			Skein_Get64_LSB_First(w,blkPtr,WCNT); /* get input block in little-endian format */
			Skein_Show_Block(BLK_BITS,&ctx->h,ctx->X,blkPtr,w,ks,ts);
			for (i=0;i < WCNT; i++)               /* do the first full key injection */
			{
				X[i]  = w[i] + ks[i];
			}
			X[WCNT-3] += ts[0];
			X[WCNT-2] += ts[1];

			Skein_Show_Round(BLK_BITS,&ctx->h,SKEIN_RND_KEY_INITIAL,X);   /* show starting state values */
			for (r=1;r <= SKEIN1024_ROUNDS_TOTAL/8; r++)
			{ /* unroll 8 rounds */
				X[ 0] += X[ 1]; X[ 1] = RotL_64(X[ 1],R1024_0_0); X[ 1] ^= X[ 0];
				X[ 2] += X[ 3]; X[ 3] = RotL_64(X[ 3],R1024_0_1); X[ 3] ^= X[ 2];
				X[ 4] += X[ 5]; X[ 5] = RotL_64(X[ 5],R1024_0_2); X[ 5] ^= X[ 4];
				X[ 6] += X[ 7]; X[ 7] = RotL_64(X[ 7],R1024_0_3); X[ 7] ^= X[ 6];
				X[ 8] += X[ 9]; X[ 9] = RotL_64(X[ 9],R1024_0_4); X[ 9] ^= X[ 8];
				X[10] += X[11]; X[11] = RotL_64(X[11],R1024_0_5); X[11] ^= X[10];
				X[12] += X[13]; X[13] = RotL_64(X[13],R1024_0_6); X[13] ^= X[12];
				X[14] += X[15]; X[15] = RotL_64(X[15],R1024_0_7); X[15] ^= X[14];    Skein_Show_Round(BLK_BITS,&ctx->h,8*r-7,X);

				X[ 0] += X[ 9]; X[ 9] = RotL_64(X[ 9],R1024_1_0); X[ 9] ^= X[ 0];
				X[ 2] += X[13]; X[13] = RotL_64(X[13],R1024_1_1); X[13] ^= X[ 2];
				X[ 6] += X[11]; X[11] = RotL_64(X[11],R1024_1_2); X[11] ^= X[ 6];
				X[ 4] += X[15]; X[15] = RotL_64(X[15],R1024_1_3); X[15] ^= X[ 4];
				X[10] += X[ 7]; X[ 7] = RotL_64(X[ 7],R1024_1_4); X[ 7] ^= X[10];
				X[12] += X[ 3]; X[ 3] = RotL_64(X[ 3],R1024_1_5); X[ 3] ^= X[12];
				X[14] += X[ 5]; X[ 5] = RotL_64(X[ 5],R1024_1_6); X[ 5] ^= X[14];
				X[ 8] += X[ 1]; X[ 1] = RotL_64(X[ 1],R1024_1_7); X[ 1] ^= X[ 8];    Skein_Show_Round(BLK_BITS,&ctx->h,8*r-6,X);

				X[ 0] += X[ 7]; X[ 7] = RotL_64(X[ 7],R1024_2_0); X[ 7] ^= X[ 0];
				X[ 2] += X[ 5]; X[ 5] = RotL_64(X[ 5],R1024_2_1); X[ 5] ^= X[ 2];
				X[ 4] += X[ 3]; X[ 3] = RotL_64(X[ 3],R1024_2_2); X[ 3] ^= X[ 4];
				X[ 6] += X[ 1]; X[ 1] = RotL_64(X[ 1],R1024_2_3); X[ 1] ^= X[ 6];
				X[12] += X[15]; X[15] = RotL_64(X[15],R1024_2_4); X[15] ^= X[12];
				X[14] += X[13]; X[13] = RotL_64(X[13],R1024_2_5); X[13] ^= X[14];
				X[ 8] += X[11]; X[11] = RotL_64(X[11],R1024_2_6); X[11] ^= X[ 8];
				X[10] += X[ 9]; X[ 9] = RotL_64(X[ 9],R1024_2_7); X[ 9] ^= X[10];    Skein_Show_Round(BLK_BITS,&ctx->h,8*r-5,X);

				X[ 0] += X[15]; X[15] = RotL_64(X[15],R1024_3_0); X[15] ^= X[ 0];
				X[ 2] += X[11]; X[11] = RotL_64(X[11],R1024_3_1); X[11] ^= X[ 2];
				X[ 6] += X[13]; X[13] = RotL_64(X[13],R1024_3_2); X[13] ^= X[ 6];
				X[ 4] += X[ 9]; X[ 9] = RotL_64(X[ 9],R1024_3_3); X[ 9] ^= X[ 4];
				X[14] += X[ 1]; X[ 1] = RotL_64(X[ 1],R1024_3_4); X[ 1] ^= X[14];
				X[ 8] += X[ 5]; X[ 5] = RotL_64(X[ 5],R1024_3_5); X[ 5] ^= X[ 8];
				X[10] += X[ 3]; X[ 3] = RotL_64(X[ 3],R1024_3_6); X[ 3] ^= X[10];
				X[12] += X[ 7]; X[ 7] = RotL_64(X[ 7],R1024_3_7); X[ 7] ^= X[12];    Skein_Show_Round(BLK_BITS,&ctx->h,8*r-4,X);
				InjectKey(2*r-1);

				X[ 0] += X[ 1]; X[ 1] = RotL_64(X[ 1],R1024_4_0); X[ 1] ^= X[ 0];
				X[ 2] += X[ 3]; X[ 3] = RotL_64(X[ 3],R1024_4_1); X[ 3] ^= X[ 2];
				X[ 4] += X[ 5]; X[ 5] = RotL_64(X[ 5],R1024_4_2); X[ 5] ^= X[ 4];
				X[ 6] += X[ 7]; X[ 7] = RotL_64(X[ 7],R1024_4_3); X[ 7] ^= X[ 6];
				X[ 8] += X[ 9]; X[ 9] = RotL_64(X[ 9],R1024_4_4); X[ 9] ^= X[ 8];
				X[10] += X[11]; X[11] = RotL_64(X[11],R1024_4_5); X[11] ^= X[10];
				X[12] += X[13]; X[13] = RotL_64(X[13],R1024_4_6); X[13] ^= X[12];
				X[14] += X[15]; X[15] = RotL_64(X[15],R1024_4_7); X[15] ^= X[14];    Skein_Show_Round(BLK_BITS,&ctx->h,8*r-3,X);

				X[ 0] += X[ 9]; X[ 9] = RotL_64(X[ 9],R1024_5_0); X[ 9] ^= X[ 0];
				X[ 2] += X[13]; X[13] = RotL_64(X[13],R1024_5_1); X[13] ^= X[ 2];
				X[ 6] += X[11]; X[11] = RotL_64(X[11],R1024_5_2); X[11] ^= X[ 6];
				X[ 4] += X[15]; X[15] = RotL_64(X[15],R1024_5_3); X[15] ^= X[ 4];
				X[10] += X[ 7]; X[ 7] = RotL_64(X[ 7],R1024_5_4); X[ 7] ^= X[10];
				X[12] += X[ 3]; X[ 3] = RotL_64(X[ 3],R1024_5_5); X[ 3] ^= X[12];
				X[14] += X[ 5]; X[ 5] = RotL_64(X[ 5],R1024_5_6); X[ 5] ^= X[14];
				X[ 8] += X[ 1]; X[ 1] = RotL_64(X[ 1],R1024_5_7); X[ 1] ^= X[ 8];    Skein_Show_Round(BLK_BITS,&ctx->h,8*r-2,X);

				X[ 0] += X[ 7]; X[ 7] = RotL_64(X[ 7],R1024_6_0); X[ 7] ^= X[ 0];
				X[ 2] += X[ 5]; X[ 5] = RotL_64(X[ 5],R1024_6_1); X[ 5] ^= X[ 2];
				X[ 4] += X[ 3]; X[ 3] = RotL_64(X[ 3],R1024_6_2); X[ 3] ^= X[ 4];
				X[ 6] += X[ 1]; X[ 1] = RotL_64(X[ 1],R1024_6_3); X[ 1] ^= X[ 6];
				X[12] += X[15]; X[15] = RotL_64(X[15],R1024_6_4); X[15] ^= X[12];
				X[14] += X[13]; X[13] = RotL_64(X[13],R1024_6_5); X[13] ^= X[14];
				X[ 8] += X[11]; X[11] = RotL_64(X[11],R1024_6_6); X[11] ^= X[ 8];
				X[10] += X[ 9]; X[ 9] = RotL_64(X[ 9],R1024_6_7); X[ 9] ^= X[10];    Skein_Show_Round(BLK_BITS,&ctx->h,8*r-1,X);

				X[ 0] += X[15]; X[15] = RotL_64(X[15],R1024_7_0); X[15] ^= X[ 0];
				X[ 2] += X[11]; X[11] = RotL_64(X[11],R1024_7_1); X[11] ^= X[ 2];
				X[ 6] += X[13]; X[13] = RotL_64(X[13],R1024_7_2); X[13] ^= X[ 6];
				X[ 4] += X[ 9]; X[ 9] = RotL_64(X[ 9],R1024_7_3); X[ 9] ^= X[ 4];
				X[14] += X[ 1]; X[ 1] = RotL_64(X[ 1],R1024_7_4); X[ 1] ^= X[14];
				X[ 8] += X[ 5]; X[ 5] = RotL_64(X[ 5],R1024_7_5); X[ 5] ^= X[ 8];
				X[10] += X[ 3]; X[ 3] = RotL_64(X[ 3],R1024_7_6); X[ 3] ^= X[10];
				X[12] += X[ 7]; X[ 7] = RotL_64(X[ 7],R1024_7_7); X[ 7] ^= X[12];    Skein_Show_Round(BLK_BITS,&ctx->h,8*r  ,X);
				InjectKey(2*r);
			}
			/* do the final "feedforward" xor, update context chaining vars */
			for (i=0;i<WCNT;i++)
				ctx->X[i] = X[i] ^ w[i];
			Skein_Show_Round(BLK_BITS,&ctx->h,SKEIN_RND_FEED_FWD,ctx->X);

			Skein_Clear_First_Flag(ctx->h);		/* clear the start bit */
			blkPtr += SKEIN1024_BLOCK_BYTES;
		}
		while (--blkCnt);
	}

#if defined(SKEIN_CODE_SIZE) || defined(SKEIN_PERF)
	size_t Skein1024_Process_Block_CodeSize(void)
	{
		return ((u08b_t *) Skein1024_Process_Block_CodeSize) -
			((u08b_t *) Skein1024_Process_Block);
	}
	uint_t Skein1024_Unroll_Cnt(void)
	{
		return 1;
	}
#endif





















	/*****************************************************************/
	/* External function to process blkCnt (nonzero) full block(s) of data. */
	void    Skein_256_Process_Block(Skein_256_Ctxt_t *ctx,const u08b_t *blkPtr,size_t blkCnt,size_t byteCntAdd);
	void    Skein_512_Process_Block(Skein_512_Ctxt_t *ctx,const u08b_t *blkPtr,size_t blkCnt,size_t byteCntAdd);
	void    Skein1024_Process_Block(Skein1024_Ctxt_t *ctx,const u08b_t *blkPtr,size_t blkCnt,size_t byteCntAdd);

	/*****************************************************************/
	/*     Portable (i.e., slow) endianness conversion functions     */
	u64b_t Skein_Swap64(u64b_t w64)
	{    /* instantiate the function body here */
		static const u64b_t ONE = 1;              /* use this to check endianness */

		/* figure out endianness "on-the-fly" */
		if (1 == ((u08b_t *) & ONE)[0])
			return w64;                           /* little-endian is fast */
		else
			return  (( w64       & 0xFF) << 56) | /*    big-endian is slow */
			(((w64 >> 8) & 0xFF) << 48) |
			(((w64 >>16) & 0xFF) << 40) |
			(((w64 >>24) & 0xFF) << 32) |
			(((w64 >>32) & 0xFF) << 24) |
			(((w64 >>40) & 0xFF) << 16) |
			(((w64 >>48) & 0xFF) <<  8) |
			(((w64 >>56) & 0xFF)      ) ;
	}

	void    Skein_Put64_LSB_First(u08b_t *dst,const u64b_t *src,size_t bCnt)
	{ /* this version is fully portable (big-endian or little-endian), but slow */
		size_t n;

		for (n=0;n<bCnt;n++)
			dst[n] = (u08b_t) (src[n>>3] >> (8*(n&7)));
	}

	void    Skein_Get64_LSB_First(u64b_t *dst,const u08b_t *src,size_t wCnt)
	{ /* this version is fully portable (big-endian or little-endian), but slow */
		size_t n;

		for (n=0;n<8*wCnt;n+=8)
			dst[n/8] = (((u64b_t) src[n  ])      ) +
			(((u64b_t) src[n+1]) <<  8) +
			(((u64b_t) src[n+2]) << 16) +
			(((u64b_t) src[n+3]) << 24) +
			(((u64b_t) src[n+4]) << 32) +
			(((u64b_t) src[n+5]) << 40) +
			(((u64b_t) src[n+6]) << 48) +
			(((u64b_t) src[n+7]) << 56) ;
	}

	/*****************************************************************/
	/*     256-bit Skein                                             */
	/*****************************************************************/

	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	/* init the context for a straight hashing operation */
	int Skein_256_Init(Skein_256_Ctxt_t *ctx, size_t hashBitLen)
	{
		union
		{
			u08b_t  b[SKEIN_256_STATE_BYTES];
			u64b_t  w[SKEIN_256_STATE_WORDS];
		} cfg;                                  /* config block */

		Skein_Assert(hashBitLen > 0,SKEIN_BAD_HASHLEN);

		/* build/process config block for hashing */
		ctx->h.hashBitLen = hashBitLen;             /* output hash byte count */
		Skein_Start_New_Type(ctx,CFG_FINAL);        /* set tweaks: T0=0; T1=CFG | FINAL */

		memset(&cfg.w,0,sizeof(cfg.w));             /* pre-pad cfg.w[] with zeroes */
		cfg.w[0] = Skein_Swap64(SKEIN_SCHEMA_VER);  /* set the schema, version */
		cfg.w[1] = Skein_Swap64(hashBitLen);        /* hash result length in bits */
		cfg.w[2] = Skein_Swap64(SKEIN_CFG_TREE_INFO_SEQUENTIAL);

		/* compute the initial chaining values from config block */
		memset(ctx->X,0,sizeof(ctx->X));            /* zero the chaining variables */
		Skein_256_Process_Block(ctx,cfg.b,1,SKEIN_CFG_STR_LEN);

		/* The chaining vars ctx->X are now initialized for the given hashBitLen. */
		/* Set up to process the data message portion of the hash (default) */
		Skein_Start_New_Type(ctx,MSG);              /* T0=0, T1= MSG type, h.bCnt=0 */

		return SKEIN_SUCCESS;
	}

	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	/* init the context for a MAC and/or tree hash operation */
	/* [identical to Skein_256_Init() when keyBytes == 0 && treeInfo == SKEIN_CFG_TREE_INFO_SEQUENTIAL] */
	int Skein_256_InitExt(Skein_256_Ctxt_t *ctx,size_t hashBitLen,u64b_t treeInfo, const u08b_t *key, size_t keyBytes)
	{
		uint_t i;
		union
		{
			u08b_t  b[SKEIN_256_STATE_BYTES];
			u64b_t  w[SKEIN_256_STATE_WORDS];
		} cfg;                                  /* config block */

		Skein_Assert(hashBitLen > 0,SKEIN_BAD_HASHLEN);
		Skein_Assert(keyBytes == 0 || key != NULL,SKEIN_FAIL);

		/* compute the initial chaining values ctx->X[], based on key */
		if (keyBytes == 0)                          /* is there a key? */
		{                                   
			memset(ctx->X,0,sizeof(ctx->X));        /* no key: use all zeroes as key for config block */
		}
		else                                        /* here to pre-process a key */
		{
			Skein_assert(sizeof(cfg.b) >= sizeof(ctx->X));
			/* do a mini-Init right here */
			ctx->h.hashBitLen=8*sizeof(ctx->X);     /* set output hash bit count = state size */
			Skein_Start_New_Type(ctx,KEY);          /* set tweaks: T0 = 0; T1 = KEY type */
			memset(ctx->X,0,sizeof(ctx->X));        /* zero the initial chaining variables */
			Skein_256_Update(ctx,key,keyBytes);     /* hash the key */
			Skein_256_Final_Pad(ctx,cfg.b);         /* put result into cfg.b[] */
			memcpy(ctx->X,cfg.b,sizeof(cfg.b));     /* copy over into ctx->X[] */
			for (i=0;i<SKEIN_256_STATE_WORDS;i++)   /* convert key bytes to context words */
				ctx->X[i] = Skein_Swap64(ctx->X[i]);
		}

		/* build/process the config block, type == CONFIG (could be precomputed for each key) */
		ctx->h.hashBitLen = hashBitLen;             /* output hash bit count */
		Skein_Start_New_Type(ctx,CFG_FINAL);

		memset(&cfg.w,0,sizeof(cfg.w));             /* pre-pad cfg.w[] with zeroes */
		cfg.w[0] = Skein_Swap64(SKEIN_SCHEMA_VER);
		cfg.w[1] = Skein_Swap64(hashBitLen);        /* hash result length in bits */
		cfg.w[2] = Skein_Swap64(treeInfo);          /* tree hash config info (or SKEIN_CFG_TREE_INFO_SEQUENTIAL) */

		Skein_Show_Key(256,&ctx->h,key,keyBytes);

		/* compute the initial chaining values from config block */
		Skein_256_Process_Block(ctx,cfg.b,1,SKEIN_CFG_STR_LEN);

		/* The chaining vars ctx->X are now initialized */
		/* Set up to process the data message portion of the hash */
		Skein_Start_New_Type(ctx,MSG);              /* T0=0, T1= MSG type, h.bCnt=0 */

		return SKEIN_SUCCESS;
	}

	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	/* process the input bytes */
	int Skein_256_Update(Skein_256_Ctxt_t *ctx, const u08b_t *msg, size_t msgByteCnt)
	{
		size_t n;

		Skein_Assert(ctx->h.bCnt <= SKEIN_256_BLOCK_BYTES,SKEIN_FAIL);     /* catch uninitialized context */

		/* process full blocks, if any */
		if (msgByteCnt + ctx->h.bCnt > SKEIN_256_BLOCK_BYTES)
		{
			if (ctx->h.bCnt)                              /* finish up any buffered message data */
			{
				n = SKEIN_256_BLOCK_BYTES - ctx->h.bCnt;  /* # bytes free in buffer b[] */
				if (n)
				{
					Skein_assert(n < msgByteCnt);         /* check on our logic here */
					memcpy(&ctx->b[ctx->h.bCnt],msg,n);
					msgByteCnt  -= n;
					msg         += n;
					ctx->h.bCnt += n;
				}
				Skein_assert(ctx->h.bCnt == SKEIN_256_BLOCK_BYTES);
				Skein_256_Process_Block(ctx,ctx->b,1,SKEIN_256_BLOCK_BYTES);
				ctx->h.bCnt = 0;
			}
			/* now process any remaining full blocks, directly from input message data */
			if (msgByteCnt > SKEIN_256_BLOCK_BYTES)
			{
				n = (msgByteCnt-1) / SKEIN_256_BLOCK_BYTES;   /* number of full blocks to process */
				Skein_256_Process_Block(ctx,msg,n,SKEIN_256_BLOCK_BYTES);
				msgByteCnt -= n * SKEIN_256_BLOCK_BYTES;
				msg        += n * SKEIN_256_BLOCK_BYTES;
			}
			Skein_assert(ctx->h.bCnt == 0);
		}

		/* copy any remaining source message data bytes into b[] */
		if (msgByteCnt)
		{
			Skein_assert(msgByteCnt + ctx->h.bCnt <= SKEIN_256_BLOCK_BYTES);
			memcpy(&ctx->b[ctx->h.bCnt],msg,msgByteCnt);
			ctx->h.bCnt += msgByteCnt;
		}

		return SKEIN_SUCCESS;
	}

	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	/* finalize the hash computation and output the result */
	int Skein_256_Final(Skein_256_Ctxt_t *ctx, u08b_t *hashVal)
	{
		size_t i,n,byteCnt;
		u64b_t X[SKEIN_256_STATE_WORDS];
		Skein_Assert(ctx->h.bCnt <= SKEIN_256_BLOCK_BYTES,SKEIN_FAIL);    /* catch uninitialized context */

		ctx->h.T[1] |= SKEIN_T1_FLAG_FINAL;        /* tag as the final block */
		if (ctx->h.bCnt < SKEIN_256_BLOCK_BYTES)   /* zero pad b[] if necessary */
			memset(&ctx->b[ctx->h.bCnt],0,SKEIN_256_BLOCK_BYTES - ctx->h.bCnt);
		Skein_256_Process_Block(ctx,ctx->b,1,ctx->h.bCnt);    /* process the final block */

		/* now output the result */
		byteCnt = (ctx->h.hashBitLen + 7) >> 3;    /* total number of output bytes */

		/* run Threefish in "counter mode" to generate output */
		memset(ctx->b,0,sizeof(ctx->b));  /* zero out b[], so it can hold the counter */
		memcpy(X,ctx->X,sizeof(X));       /* keep a local copy of counter mode "key" */
		for (i=0;i*SKEIN_256_BLOCK_BYTES < byteCnt;i++)
		{
			((u64b_t *)ctx->b)[0]= Skein_Swap64((u64b_t) i); /* build the counter block */
			Skein_Start_New_Type(ctx,OUT_FINAL);
			Skein_256_Process_Block(ctx,ctx->b,1,sizeof(u64b_t)); /* run "counter mode" */
			n = byteCnt - i*SKEIN_256_BLOCK_BYTES;   /* number of output bytes left to go */
			if (n >= SKEIN_256_BLOCK_BYTES)
				n  = SKEIN_256_BLOCK_BYTES;
			Skein_Put64_LSB_First(hashVal+i*SKEIN_256_BLOCK_BYTES,ctx->X,n);   /* "output" the ctr mode bytes */
			Skein_Show_Final(256,&ctx->h,n,hashVal+i*SKEIN_256_BLOCK_BYTES);
			memcpy(ctx->X,X,sizeof(X));   /* restore the counter mode key for next time */
		}
		return SKEIN_SUCCESS;
	}

#if defined(SKEIN_CODE_SIZE) || defined(SKEIN_PERF)
	size_t Skein_256_API_CodeSize(void)
	{
		return ((u08b_t *) Skein_256_API_CodeSize) -
			((u08b_t *) Skein_256_Init);
	}
#endif

	/*****************************************************************/
	/*     512-bit Skein                                             */
	/*****************************************************************/

	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	/* init the context for a straight hashing operation */
	int Skein_512_Init(Skein_512_Ctxt_t *ctx, size_t hashBitLen)
	{
		union
		{
			u08b_t  b[SKEIN_512_STATE_BYTES];
			u64b_t  w[SKEIN_512_STATE_WORDS];
		} cfg;                                  /* config block */

		Skein_Assert(hashBitLen > 0,SKEIN_BAD_HASHLEN);

		/* build/process config block for hashing */
		ctx->h.hashBitLen = hashBitLen;             /* output hash byte count */
		Skein_Start_New_Type(ctx,CFG_FINAL);        /* set tweaks: T0=0; T1=CFG | FINAL */

		memset(&cfg.w,0,sizeof(cfg.w));             /* pre-pad cfg.w[] with zeroes */
		cfg.w[0] = Skein_Swap64(SKEIN_SCHEMA_VER);  /* set the schema, version */
		cfg.w[1] = Skein_Swap64(hashBitLen);        /* hash result length in bits */
		cfg.w[2] = Skein_Swap64(SKEIN_CFG_TREE_INFO_SEQUENTIAL);

		/* compute the initial chaining values from config block */
		memset(ctx->X,0,sizeof(ctx->X));            /* zero the chaining variables */
		Skein_512_Process_Block(ctx,cfg.b,1,SKEIN_CFG_STR_LEN);

		/* The chaining vars ctx->X are now initialized for the given hashBitLen. */
		/* Set up to process the data message portion of the hash (default) */
		Skein_Start_New_Type(ctx,MSG);              /* T0=0, T1= MSG type, h.bCnt=0 */

		return SKEIN_SUCCESS;
	}

	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	/* init the context for a MAC and/or tree hash operation */
	/* [identical to Skein_512_Init() when keyBytes == 0 && treeInfo == SKEIN_CFG_TREE_INFO_SEQUENTIAL] */
	int Skein_512_InitExt(Skein_512_Ctxt_t *ctx,size_t hashBitLen,u64b_t treeInfo, const u08b_t *key, size_t keyBytes)
	{
		uint_t i;
		union
		{
			u08b_t  b[SKEIN_512_STATE_BYTES];
			u64b_t  w[SKEIN_512_STATE_WORDS];
		} cfg;                                  /* config block */

		Skein_Assert(hashBitLen > 0,SKEIN_BAD_HASHLEN);
		Skein_Assert(keyBytes == 0 || key != NULL,SKEIN_FAIL);

		/* compute the initial chaining values ctx->X[], based on key */
		if (keyBytes == 0)                          /* is there a key? */
		{                                   
			memset(ctx->X,0,sizeof(ctx->X));        /* no key: use all zeroes as key for config block */
		}
		else                                        /* here to pre-process a key */
		{
			Skein_assert(sizeof(cfg.b) >= sizeof(ctx->X));
			/* do a mini-Init right here */
			ctx->h.hashBitLen=8*sizeof(ctx->X);     /* set output hash bit count = state size */
			Skein_Start_New_Type(ctx,KEY);          /* set tweaks: T0 = 0; T1 = KEY type */
			memset(ctx->X,0,sizeof(ctx->X));        /* zero the initial chaining variables */
			Skein_512_Update(ctx,key,keyBytes);     /* hash the key */
			Skein_512_Final_Pad(ctx,cfg.b);         /* put result into cfg.b[] */
			memcpy(ctx->X,cfg.b,sizeof(cfg.b));     /* copy over into ctx->X[] */
			for (i=0;i<SKEIN_512_STATE_WORDS;i++)   /* convert key bytes to context words */
				ctx->X[i] = Skein_Swap64(ctx->X[i]);
		}

		/* build/process the config block, type == CONFIG (could be precomputed for each key) */
		ctx->h.hashBitLen = hashBitLen;             /* output hash bit count */
		Skein_Start_New_Type(ctx,CFG_FINAL);

		memset(&cfg.w,0,sizeof(cfg.w));             /* pre-pad cfg.w[] with zeroes */
		cfg.w[0] = Skein_Swap64(SKEIN_SCHEMA_VER);
		cfg.w[1] = Skein_Swap64(hashBitLen);        /* hash result length in bits */
		cfg.w[2] = Skein_Swap64(treeInfo);          /* tree hash config info (or SKEIN_CFG_TREE_INFO_SEQUENTIAL) */

		Skein_Show_Key(512,&ctx->h,key,keyBytes);

		/* compute the initial chaining values from config block */
		Skein_512_Process_Block(ctx,cfg.b,1,SKEIN_CFG_STR_LEN);

		/* The chaining vars ctx->X are now initialized */
		/* Set up to process the data message portion of the hash */
		Skein_Start_New_Type(ctx,MSG);              /* T0=0, T1= MSG type, h.bCnt=0 */

		return SKEIN_SUCCESS;
	}

	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	/* process the input bytes */
	int Skein_512_Update(Skein_512_Ctxt_t *ctx, const u08b_t *msg, size_t msgByteCnt)
	{
		size_t n;

		Skein_Assert(ctx->h.bCnt <= SKEIN_512_BLOCK_BYTES,SKEIN_FAIL);     /* catch uninitialized context */

		/* process full blocks, if any */
		if (msgByteCnt + ctx->h.bCnt > SKEIN_512_BLOCK_BYTES)
		{
			if (ctx->h.bCnt)                              /* finish up any buffered message data */
			{
				n = SKEIN_512_BLOCK_BYTES - ctx->h.bCnt;  /* # bytes free in buffer b[] */
				if (n)
				{
					Skein_assert(n < msgByteCnt);         /* check on our logic here */
					memcpy(&ctx->b[ctx->h.bCnt],msg,n);
					msgByteCnt  -= n;
					msg         += n;
					ctx->h.bCnt += n;
				}
				Skein_assert(ctx->h.bCnt == SKEIN_512_BLOCK_BYTES);
				Skein_512_Process_Block(ctx,ctx->b,1,SKEIN_512_BLOCK_BYTES);
				ctx->h.bCnt = 0;
			}
			/* now process any remaining full blocks, directly from input message data */
			if (msgByteCnt > SKEIN_512_BLOCK_BYTES)
			{
				n = (msgByteCnt-1) / SKEIN_512_BLOCK_BYTES;   /* number of full blocks to process */
				Skein_512_Process_Block(ctx,msg,n,SKEIN_512_BLOCK_BYTES);
				msgByteCnt -= n * SKEIN_512_BLOCK_BYTES;
				msg        += n * SKEIN_512_BLOCK_BYTES;
			}
			Skein_assert(ctx->h.bCnt == 0);
		}

		/* copy any remaining source message data bytes into b[] */
		if (msgByteCnt)
		{
			Skein_assert(msgByteCnt + ctx->h.bCnt <= SKEIN_512_BLOCK_BYTES);
			memcpy(&ctx->b[ctx->h.bCnt],msg,msgByteCnt);
			ctx->h.bCnt += msgByteCnt;
		}

		return SKEIN_SUCCESS;
	}

	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	/* finalize the hash computation and output the result */
	int Skein_512_Final(Skein_512_Ctxt_t *ctx, u08b_t *hashVal)
	{
		size_t i,n,byteCnt;
		u64b_t X[SKEIN_512_STATE_WORDS];
		Skein_Assert(ctx->h.bCnt <= SKEIN_512_BLOCK_BYTES,SKEIN_FAIL);    /* catch uninitialized context */

		ctx->h.T[1] |= SKEIN_T1_FLAG_FINAL;                 /* tag as the final block */
		if (ctx->h.bCnt < SKEIN_512_BLOCK_BYTES)   /* zero pad b[] if necessary */
			memset(&ctx->b[ctx->h.bCnt],0,SKEIN_512_BLOCK_BYTES - ctx->h.bCnt);

		Skein_512_Process_Block(ctx,ctx->b,1,ctx->h.bCnt);  /* process the final block */

		/* now output the result */
		byteCnt = (ctx->h.hashBitLen + 7) >> 3;             /* total number of output bytes */

		/* run Threefish in "counter mode" to generate more output */
		memset(ctx->b,0,sizeof(ctx->b));  /* zero out b[], so it can hold the counter */
		memcpy(X,ctx->X,sizeof(X));       /* keep a local copy of counter mode "key" */
		for (i=0;i*SKEIN_512_BLOCK_BYTES < byteCnt;i++)
		{
			((u64b_t *)ctx->b)[0]= Skein_Swap64((u64b_t) i); /* build the counter block */
			Skein_Start_New_Type(ctx,OUT_FINAL);
			Skein_512_Process_Block(ctx,ctx->b,1,sizeof(u64b_t)); /* run "counter mode" */
			n = byteCnt - i*SKEIN_512_BLOCK_BYTES;   /* number of output bytes left to go */
			if (n >= SKEIN_512_BLOCK_BYTES)
				n  = SKEIN_512_BLOCK_BYTES;
			Skein_Put64_LSB_First(hashVal+i*SKEIN_512_BLOCK_BYTES,ctx->X,n);   /* "output" the ctr mode bytes */
			Skein_Show_Final(512,&ctx->h,n,hashVal+i*SKEIN_512_BLOCK_BYTES);
			memcpy(ctx->X,X,sizeof(X));   /* restore the counter mode key for next time */
		}

		return SKEIN_SUCCESS;
	}

#if defined(SKEIN_CODE_SIZE) || defined(SKEIN_PERF)
	size_t Skein_512_API_CodeSize(void)
	{
		return ((u08b_t *) Skein_512_API_CodeSize) -
			((u08b_t *) Skein_512_Init);
	}
#endif

	/*****************************************************************/
	/*    1024-bit Skein                                             */
	/*****************************************************************/

	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	/* init the context for a straight hashing operation */
	int Skein1024_Init(Skein1024_Ctxt_t *ctx, size_t hashBitLen)
	{
		union
		{
			u08b_t  b[SKEIN1024_STATE_BYTES];
			u64b_t  w[SKEIN1024_STATE_WORDS];
		} cfg;                                  /* config block */

		Skein_Assert(hashBitLen > 0,SKEIN_BAD_HASHLEN);

		/* build/process config block for hashing */
		ctx->h.hashBitLen = hashBitLen;             /* output hash byte count */
		Skein_Start_New_Type(ctx,CFG_FINAL);        /* set tweaks: T0=0; T1=CFG | FINAL */

		memset(&cfg.w,0,sizeof(cfg.w));             /* pre-pad cfg.w[] with zeroes */
		cfg.w[0] = Skein_Swap64(SKEIN_SCHEMA_VER);  /* set the schema, version */
		cfg.w[1] = Skein_Swap64(hashBitLen);        /* hash result length in bits */
		cfg.w[2] = Skein_Swap64(SKEIN_CFG_TREE_INFO_SEQUENTIAL);

		/* compute the initial chaining values from config block */
		memset(ctx->X,0,sizeof(ctx->X));            /* zero the chaining variables */
		Skein1024_Process_Block(ctx,cfg.b,1,SKEIN_CFG_STR_LEN);

		/* The chaining vars ctx->X are now initialized for the given hashBitLen. */
		/* Set up to process the data message portion of the hash (default) */
		Skein_Start_New_Type(ctx,MSG);              /* T0=0, T1= MSG type, h.bCnt=0 */

		return SKEIN_SUCCESS;
	}

	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	/* init the context for a MAC and/or tree hash operation */
	/* [identical to Skein1024_Init() when keyBytes == 0 && treeInfo == SKEIN_CFG_TREE_INFO_SEQUENTIAL] */
	int Skein1024_InitExt(Skein1024_Ctxt_t *ctx,size_t hashBitLen,u64b_t treeInfo, const u08b_t *key, size_t keyBytes)
	{
		uint_t i;
		union
		{
			u08b_t  b[SKEIN1024_STATE_BYTES];
			u64b_t  w[SKEIN1024_STATE_WORDS];
		} cfg;                                  /* config block */

		Skein_Assert(hashBitLen > 0,SKEIN_BAD_HASHLEN);
		Skein_Assert(keyBytes == 0 || key != NULL,SKEIN_FAIL);

		/* compute the initial chaining values ctx->X[], based on key */
		if (keyBytes == 0)                          /* is there a key? */
		{                                   
			memset(ctx->X,0,sizeof(ctx->X));        /* no key: use all zeroes as key for config block */
		}
		else                                        /* here to pre-process a key */
		{
			Skein_assert(sizeof(cfg.b) >= sizeof(ctx->X));
			/* do a mini-Init right here */
			ctx->h.hashBitLen=8*sizeof(ctx->X);     /* set output hash bit count = state size */
			Skein_Start_New_Type(ctx,KEY);          /* set tweaks: T0 = 0; T1 = KEY type */
			memset(ctx->X,0,sizeof(ctx->X));        /* zero the initial chaining variables */
			Skein1024_Update(ctx,key,keyBytes);     /* hash the key */
			Skein1024_Final_Pad(ctx,cfg.b);         /* put result into cfg.b[] */
			memcpy(ctx->X,cfg.b,sizeof(cfg.b));     /* copy over into ctx->X[] */
			for (i=0;i<SKEIN1024_STATE_WORDS;i++)   /* convert key bytes to context words */
				ctx->X[i] = Skein_Swap64(ctx->X[i]);
		}

		/* build/process the config block, type == CONFIG (could be precomputed for each key) */
		ctx->h.hashBitLen = hashBitLen;             /* output hash bit count */
		Skein_Start_New_Type(ctx,CFG_FINAL);

		memset(&cfg.w,0,sizeof(cfg.w));             /* pre-pad cfg.w[] with zeroes */
		cfg.w[0] = Skein_Swap64(SKEIN_SCHEMA_VER);
		cfg.w[1] = Skein_Swap64(hashBitLen);        /* hash result length in bits */
		cfg.w[2] = Skein_Swap64(treeInfo);          /* tree hash config info (or SKEIN_CFG_TREE_INFO_SEQUENTIAL) */

		Skein_Show_Key(1024,&ctx->h,key,keyBytes);

		/* compute the initial chaining values from config block */
		Skein1024_Process_Block(ctx,cfg.b,1,SKEIN_CFG_STR_LEN);

		/* The chaining vars ctx->X are now initialized */
		/* Set up to process the data message portion of the hash */
		Skein_Start_New_Type(ctx,MSG);              /* T0=0, T1= MSG type, h.bCnt=0 */

		return SKEIN_SUCCESS;
	}

	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	/* process the input bytes */
	int Skein1024_Update(Skein1024_Ctxt_t *ctx, const u08b_t *msg, size_t msgByteCnt)
	{
		size_t n;

		Skein_Assert(ctx->h.bCnt <= SKEIN1024_BLOCK_BYTES,SKEIN_FAIL);     /* catch uninitialized context */

		/* process full blocks, if any */
		if (msgByteCnt + ctx->h.bCnt > SKEIN1024_BLOCK_BYTES)
		{
			if (ctx->h.bCnt)                              /* finish up any buffered message data */
			{
				n = SKEIN1024_BLOCK_BYTES - ctx->h.bCnt;  /* # bytes free in buffer b[] */
				if (n)
				{
					Skein_assert(n < msgByteCnt);         /* check on our logic here */
					memcpy(&ctx->b[ctx->h.bCnt],msg,n);
					msgByteCnt  -= n;
					msg         += n;
					ctx->h.bCnt += n;
				}
				Skein_assert(ctx->h.bCnt == SKEIN1024_BLOCK_BYTES);
				Skein1024_Process_Block(ctx,ctx->b,1,SKEIN1024_BLOCK_BYTES);
				ctx->h.bCnt = 0;
			}
			/* now process any remaining full blocks, directly from input message data */
			if (msgByteCnt > SKEIN1024_BLOCK_BYTES)
			{
				n = (msgByteCnt-1) / SKEIN1024_BLOCK_BYTES;   /* number of full blocks to process */
				Skein1024_Process_Block(ctx,msg,n,SKEIN1024_BLOCK_BYTES);
				msgByteCnt -= n * SKEIN1024_BLOCK_BYTES;
				msg        += n * SKEIN1024_BLOCK_BYTES;
			}
			Skein_assert(ctx->h.bCnt == 0);
		}

		/* copy any remaining source message data bytes into b[] */
		if (msgByteCnt)
		{
			Skein_assert(msgByteCnt + ctx->h.bCnt <= SKEIN1024_BLOCK_BYTES);
			memcpy(&ctx->b[ctx->h.bCnt],msg,msgByteCnt);
			ctx->h.bCnt += msgByteCnt;
		}

		return SKEIN_SUCCESS;
	}

	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	/* finalize the hash computation and output the result */
	int Skein1024_Final(Skein1024_Ctxt_t *ctx, u08b_t *hashVal)
	{
		size_t i,n,byteCnt;
		u64b_t X[SKEIN1024_STATE_WORDS];
		Skein_Assert(ctx->h.bCnt <= SKEIN1024_BLOCK_BYTES,SKEIN_FAIL);    /* catch uninitialized context */

		ctx->h.T[1] |= SKEIN_T1_FLAG_FINAL;                 /* tag as the final block */
		if (ctx->h.bCnt < SKEIN1024_BLOCK_BYTES)   /* zero pad b[] if necessary */
			memset(&ctx->b[ctx->h.bCnt],0,SKEIN1024_BLOCK_BYTES - ctx->h.bCnt);

		Skein1024_Process_Block(ctx,ctx->b,1,ctx->h.bCnt);  /* process the final block */

		/* now output the result */
		byteCnt = (ctx->h.hashBitLen + 7) >> 3;    /* total number of output bytes */

		/* run Threefish in "counter mode" to generate output */
		memset(ctx->b,0,sizeof(ctx->b));  /* zero out b[], so it can hold the counter */
		memcpy(X,ctx->X,sizeof(X));       /* keep a local copy of counter mode "key" */
		for (i=0;i*SKEIN1024_BLOCK_BYTES < byteCnt;i++)
		{
			((u64b_t *)ctx->b)[0]= Skein_Swap64((u64b_t) i); /* build the counter block */
			Skein_Start_New_Type(ctx,OUT_FINAL);
			Skein1024_Process_Block(ctx,ctx->b,1,sizeof(u64b_t)); /* run "counter mode" */
			n = byteCnt - i*SKEIN1024_BLOCK_BYTES;   /* number of output bytes left to go */
			if (n >= SKEIN1024_BLOCK_BYTES)
				n  = SKEIN1024_BLOCK_BYTES;
			Skein_Put64_LSB_First(hashVal+i*SKEIN1024_BLOCK_BYTES,ctx->X,n);   /* "output" the ctr mode bytes */
			Skein_Show_Final(1024,&ctx->h,n,hashVal+i*SKEIN1024_BLOCK_BYTES);
			memcpy(ctx->X,X,sizeof(X));   /* restore the counter mode key for next time */
		}
		return SKEIN_SUCCESS;
	}

#if defined(SKEIN_CODE_SIZE) || defined(SKEIN_PERF)
	size_t Skein1024_API_CodeSize(void)
	{
		return ((u08b_t *) Skein1024_API_CodeSize) -
			((u08b_t *) Skein1024_Init);
	}
#endif

#if SKEIN_TREE_HASH
	/**************** Functions to support tree hashing ***************/
	/* (this code is identical for Optimized and Reference versions)  */

	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	/* finalize the hash computation and output the block, no OUTPUT stage */
	int Skein_256_Final_Pad(Skein_256_Ctxt_t *ctx, u08b_t *hashVal)
	{
		Skein_Assert(ctx->h.bCnt <= SKEIN_256_BLOCK_BYTES,SKEIN_FAIL);    /* catch uninitialized context */

		ctx->h.T[1] |= SKEIN_T1_FLAG_FINAL;        /* tag as the final block */
		if (ctx->h.bCnt < SKEIN_256_BLOCK_BYTES)   /* zero pad b[] if necessary */
			memset(&ctx->b[ctx->h.bCnt],0,SKEIN_256_BLOCK_BYTES - ctx->h.bCnt);
		Skein_256_Process_Block(ctx,ctx->b,1,ctx->h.bCnt);    /* process the final block */

		Skein_Put64_LSB_First(hashVal,ctx->X,SKEIN_256_BLOCK_BYTES);   /* "output" the state bytes */

		return SKEIN_SUCCESS;
	}

	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	/* finalize the hash computation and output the block, no OUTPUT stage */
	int Skein_512_Final_Pad(Skein_512_Ctxt_t *ctx, u08b_t *hashVal)
	{
		Skein_Assert(ctx->h.bCnt <= SKEIN_512_BLOCK_BYTES,SKEIN_FAIL);    /* catch uninitialized context */

		ctx->h.T[1] |= SKEIN_T1_FLAG_FINAL;        /* tag as the final block */
		if (ctx->h.bCnt < SKEIN_512_BLOCK_BYTES)   /* zero pad b[] if necessary */
			memset(&ctx->b[ctx->h.bCnt],0,SKEIN_512_BLOCK_BYTES - ctx->h.bCnt);
		Skein_512_Process_Block(ctx,ctx->b,1,ctx->h.bCnt);    /* process the final block */

		Skein_Put64_LSB_First(hashVal,ctx->X,SKEIN_512_BLOCK_BYTES);   /* "output" the state bytes */

		return SKEIN_SUCCESS;
	}

	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	/* finalize the hash computation and output the block, no OUTPUT stage */
	int Skein1024_Final_Pad(Skein1024_Ctxt_t *ctx, u08b_t *hashVal)
	{
		Skein_Assert(ctx->h.bCnt <= SKEIN1024_BLOCK_BYTES,SKEIN_FAIL);    /* catch uninitialized context */

		ctx->h.T[1] |= SKEIN_T1_FLAG_FINAL;        /* tag as the final block */
		if (ctx->h.bCnt < SKEIN1024_BLOCK_BYTES)   /* zero pad b[] if necessary */
			memset(&ctx->b[ctx->h.bCnt],0,SKEIN1024_BLOCK_BYTES - ctx->h.bCnt);
		Skein1024_Process_Block(ctx,ctx->b,1,ctx->h.bCnt);    /* process the final block */

		Skein_Put64_LSB_First(hashVal,ctx->X,SKEIN1024_BLOCK_BYTES);   /* "output" the state bytes */

		return SKEIN_SUCCESS;
	}

	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	/* just do the OUTPUT stage                                       */
	int Skein_256_Output(Skein_256_Ctxt_t *ctx, u08b_t *hashVal)
	{
		size_t i,n,byteCnt;
		u64b_t X[SKEIN_256_STATE_WORDS];
		Skein_Assert(ctx->h.bCnt <= SKEIN_256_BLOCK_BYTES,SKEIN_FAIL);    /* catch uninitialized context */

		/* now output the result */
		byteCnt = (ctx->h.hashBitLen + 7) >> 3;    /* total number of output bytes */

		/* run Threefish in "counter mode" to generate output */
		memset(ctx->b,0,sizeof(ctx->b));  /* zero out b[], so it can hold the counter */
		memcpy(X,ctx->X,sizeof(X));       /* keep a local copy of counter mode "key" */
		for (i=0;i*SKEIN_256_BLOCK_BYTES < byteCnt;i++)
		{
			((u64b_t *)ctx->b)[0]= Skein_Swap64((u64b_t) i); /* build the counter block */
			Skein_Start_New_Type(ctx,OUT_FINAL);
			Skein_256_Process_Block(ctx,ctx->b,1,sizeof(u64b_t)); /* run "counter mode" */
			n = byteCnt - i*SKEIN_256_BLOCK_BYTES;   /* number of output bytes left to go */
			if (n >= SKEIN_256_BLOCK_BYTES)
				n  = SKEIN_256_BLOCK_BYTES;
			Skein_Put64_LSB_First(hashVal+i*SKEIN_256_BLOCK_BYTES,ctx->X,n);   /* "output" the ctr mode bytes */
			Skein_Show_Final(256,&ctx->h,n,hashVal+i*SKEIN_256_BLOCK_BYTES);
			memcpy(ctx->X,X,sizeof(X));   /* restore the counter mode key for next time */
		}
		return SKEIN_SUCCESS;
	}

	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	/* just do the OUTPUT stage                                       */
	int Skein_512_Output(Skein_512_Ctxt_t *ctx, u08b_t *hashVal)
	{
		size_t i,n,byteCnt;
		u64b_t X[SKEIN_512_STATE_WORDS];
		Skein_Assert(ctx->h.bCnt <= SKEIN_512_BLOCK_BYTES,SKEIN_FAIL);    /* catch uninitialized context */

		/* now output the result */
		byteCnt = (ctx->h.hashBitLen + 7) >> 3;    /* total number of output bytes */

		/* run Threefish in "counter mode" to generate output */
		memset(ctx->b,0,sizeof(ctx->b));  /* zero out b[], so it can hold the counter */
		memcpy(X,ctx->X,sizeof(X));       /* keep a local copy of counter mode "key" */
		for (i=0;i*SKEIN_512_BLOCK_BYTES < byteCnt;i++)
		{
			((u64b_t *)ctx->b)[0]= Skein_Swap64((u64b_t) i); /* build the counter block */
			Skein_Start_New_Type(ctx,OUT_FINAL);
			Skein_512_Process_Block(ctx,ctx->b,1,sizeof(u64b_t)); /* run "counter mode" */
			n = byteCnt - i*SKEIN_512_BLOCK_BYTES;   /* number of output bytes left to go */
			if (n >= SKEIN_512_BLOCK_BYTES)
				n  = SKEIN_512_BLOCK_BYTES;
			Skein_Put64_LSB_First(hashVal+i*SKEIN_512_BLOCK_BYTES,ctx->X,n);   /* "output" the ctr mode bytes */
			Skein_Show_Final(256,&ctx->h,n,hashVal+i*SKEIN_512_BLOCK_BYTES);
			memcpy(ctx->X,X,sizeof(X));   /* restore the counter mode key for next time */
		}
		return SKEIN_SUCCESS;
	}

	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	/* just do the OUTPUT stage                                       */
	int Skein1024_Output(Skein1024_Ctxt_t *ctx, u08b_t *hashVal)
	{
		size_t i,n,byteCnt;
		u64b_t X[SKEIN1024_STATE_WORDS];
		Skein_Assert(ctx->h.bCnt <= SKEIN1024_BLOCK_BYTES,SKEIN_FAIL);    /* catch uninitialized context */

		/* now output the result */
		byteCnt = (ctx->h.hashBitLen + 7) >> 3;    /* total number of output bytes */

		/* run Threefish in "counter mode" to generate output */
		memset(ctx->b,0,sizeof(ctx->b));  /* zero out b[], so it can hold the counter */
		memcpy(X,ctx->X,sizeof(X));       /* keep a local copy of counter mode "key" */
		for (i=0;i*SKEIN1024_BLOCK_BYTES < byteCnt;i++)
		{
			((u64b_t *)ctx->b)[0]= Skein_Swap64((u64b_t) i); /* build the counter block */
			Skein_Start_New_Type(ctx,OUT_FINAL);
			Skein1024_Process_Block(ctx,ctx->b,1,sizeof(u64b_t)); /* run "counter mode" */
			n = byteCnt - i*SKEIN1024_BLOCK_BYTES;   /* number of output bytes left to go */
			if (n >= SKEIN1024_BLOCK_BYTES)
				n  = SKEIN1024_BLOCK_BYTES;
			Skein_Put64_LSB_First(hashVal+i*SKEIN1024_BLOCK_BYTES,ctx->X,n);   /* "output" the ctr mode bytes */
			Skein_Show_Final(256,&ctx->h,n,hashVal+i*SKEIN1024_BLOCK_BYTES);
			memcpy(ctx->X,X,sizeof(X));   /* restore the counter mode key for next time */
		}
		return SKEIN_SUCCESS;
	}
#endif



}



namespace xcore
{
	xdigest_engine_skein256::xdigest_engine_skein256()
	{
		reset2();
	}

	///@name Updating
	void	xdigest_engine_skein256::reset()
	{
		reset2();
	}

	void	xdigest_engine_skein256::update(void const* inBuffer, u32 inLength)
	{
		if (mState == OPEN)
			skein_hash::Skein_256_Update((skein_hash::Skein_256_Ctxt_t*)&mCtx, (skein_hash::u08b_t*)inBuffer, inLength);
	}

	void	xdigest_engine_skein256::digest(xbyte* digest)
	{
		if (mState == OPEN)
		{
			mState = CLOSED;
			skein_hash::Skein_256_Final((skein_hash::Skein_256_Ctxt_t*)&mCtx, digest);
		}
	}

	bool	xdigest_engine_skein256::digest(xskein256& hash)
	{
		if (mState == OPEN)
		{
			mState = CLOSED;
			skein_hash::Skein_256_Final((skein_hash::Skein_256_Ctxt_t*)&mCtx, hash.data());
			return true;
		}
		return false;
	}

	void	xdigest_engine_skein256::reset2()
	{
		mState = OPEN;
		skein_hash::Skein_256_Init((skein_hash::Skein_256_Ctxt_t*)&mCtx, 256);
	}


	xdigest_engine_skein512::xdigest_engine_skein512()
	{
		reset2();
	}

	///@name Updating
	void	xdigest_engine_skein512::reset()
	{
		reset2();
	}

	void	xdigest_engine_skein512::update(void const* inBuffer, u32 inLength)
	{
		if (mState == OPEN)
			skein_hash::Skein_512_Update((skein_hash::Skein_512_Ctxt_t*)&mCtx, (skein_hash::u08b_t*)inBuffer, inLength);
	}

	void	xdigest_engine_skein512::digest(xbyte* digest)
	{
		if (mState == OPEN)
		{
			mState = CLOSED;
			skein_hash::Skein_512_Final((skein_hash::Skein_512_Ctxt_t*)&mCtx, digest);
		}
	}

	bool	xdigest_engine_skein512::digest(xskein512& hash)
	{
		if (mState == OPEN)
		{
			mState = CLOSED;
			skein_hash::Skein_512_Final((skein_hash::Skein_512_Ctxt_t*)&mCtx, hash.data());
			return true;
		}
		return false;
	}

	void	xdigest_engine_skein512::reset2()
	{
		mState = OPEN;
		skein_hash::Skein_512_Init((skein_hash::Skein_512_Ctxt_t*)&mCtx, 512);
	}



	/**
	 *	Utilities
	 */
	xskein256	x_skein256Hash(void const* inBuffer, s32 inLength)
	{
		skein_hash::Skein_256_Ctxt_t ctx;
		skein_hash::Skein_256_Init(&ctx, 256);
		skein_hash::Skein_256_Update(&ctx, (skein_hash::u08b_t*)inBuffer, inLength);
		xskein256 hash;
		skein_hash::Skein_256_Final(&ctx, hash.data());
		return hash;
	}

	xskein512	x_skein512Hash(void const* inBuffer, s32 inLength)
	{
		skein_hash::Skein_512_Ctxt_t ctx;
		skein_hash::Skein_512_Init(&ctx, 512);
		skein_hash::Skein_512_Update(&ctx, (skein_hash::u08b_t*)inBuffer, inLength);
		xskein512 hash;
		skein_hash::Skein_512_Final(&ctx, hash.data());
		return hash;
	}

}
