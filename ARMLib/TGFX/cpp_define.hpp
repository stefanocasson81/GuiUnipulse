/*
 * cpp_define.hpp
 *
 *  Created on: 14 lug 2021
 *      Author: stefanoc
 */

#ifndef TARGET_CPP_DEFINE_HPP_
#define TARGET_CPP_DEFINE_HPP_

/**************************************************************************************************
* INCLUDE
*************************************************************************************************/

#include <machine/_default_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#if 0

typedef signed char        S8;
typedef signed short       S16;
typedef signed int         S32;
typedef signed long long   S64;
typedef unsigned char      U8;
typedef unsigned short     U16;
typedef unsigned int       U32;
typedef unsigned long long U64;

#endif


typedef unsigned short     td_UNION_WORD_BIT;
typedef unsigned long      bit;
typedef __uint8_t          BOOLEAN;
typedef __uint8_t          CHAR;             /* Char  8 bit                                        */
typedef __uint8_t          U8;            /* Unsigned  8 bit quantity                           */
typedef __int8_t           S8;            /* Signed    8 bit quantity                           */
typedef __uint16_t         U16;           /* Unsigned 16 bit quantity                           */
typedef __int16_t          S16;           /* Signed   16 bit quantity                           */
typedef __uint32_t         U32;           /* Unsigned 32 bit quantity                           */
typedef __int32_t          S32;           /* Signed   32 bit quantity                           */
//typedef signed   long long INT64S;        /* Signed   64 bit quantity                           */
//typedef unsigned long long INT64U;        /* Unsigned 64 bit quantity                           */
typedef float              FP32;             /* Single precision floating point                    */
typedef double             FP64;             /* Double precision floating point                    */





#define SETBIT(a, bit)      (a |= (1<<(bit)))
#define RESBIT(a, bit)      (a &= (~(1<<(bit))))
#define XORBIT(a, bit)      (a ^= (1<<(bit)))
#define CHKBIT(a, bit)      (a & (1<<(bit)))

// byte ordering on the Cortex-M4 core, they are organised as little endian
#define LOBYTE(w)       ( *( (uint8_t *) (&(w)) + 0) )
#define HIBYTE(w)       ( *( (uint8_t *) (&(w)) + 1) )

// word ordering for the Cortex-M3 core for a 32 bit variable w
#define LOWORD(w)       ( *( (uint16_t *) (&(w)) + 0))
#define HIWORD(w)       ( *( (uint16_t *) (&(w)) + 1))

// byte ordering for the Cortex-M3 core for a 32 bit variable w
#define LOWORDLOBYTE(w) ( *( (uint8_t *) (&(w)) + 0))
#define LOWORDHIBYTE(w) ( *( (uint8_t *) (&(w)) + 1))
#define HIWORDLOBYTE(w) ( *( (uint8_t *) (&(w)) + 2))
#define HIWORDHIBYTE(w) ( *( (uint8_t *) (&(w)) + 3))

#define DOWORD(h,l)     (unsigned short)(((unsigned short)(h)<<8)|((unsigned char)(l)))
#define DOLONG(h,l)     (unsigned long)(((unsigned long)(h)<<16)|((unsigned short)(l)))


#define DOUBLE_BYTE_TO_WORD(HI,LO)           ((U16)( ( ((U16)HI)<<8) | ((U16)LO) ))
#define DOUBLE_BYTE_TO_WORD_POINT(HI,LO)     (U16)((((U16) *(HI))<<8) | ((U16) *(LO)))
#define QUADRUPLE_BYTE_TO_U32(MMSB,MSB,LSB,LLSB)           ((U32)( (((U32)MMSB)<<24) | (((U32)MSB)<<16) | (((U32)LSB)<<8) | (((U32)LLSB))))

// set the related flag (only flag word surounded by atomic instructions in case of interruption)
#define SET_FLAG(a, mask)        {_atomic_(0); a |= mask; _endatomic_();}

// reset the related flag (only flag word surounded by atomic instructions in case of interruption)
#define RESET_FLAG(a, mask)      {_atomic_(0); a &= ~(mask); _endatomic_();}

#define TEST_FLAG(a, mask)       ((bit) ((a) & (mask) ))             // check a flag (only flag word)
#if (0)
#define TEST_BIT(a, b)           TEST_FLAG ((a), (1 << (b)))
#define SET_BIT(a, b)            SET_FLAG  ((a), (1 << (b)))
#define RESET_BIT(a, b)          RESET_FLAG((a), (1 << (b)))
#define TEST_MASK(a, b)          (((a) & (1 << (b))) != 0)
#endif

#define SETBIT(a, bit)      (a |= (1<<(bit)))
#define RESBIT(a, bit)      (a &= (~(1<<(bit))))
#define XORBIT(a, bit)      (a ^= (1<<(bit)))
#define CHKBIT(a, bit)      (a & (1<<(bit)))

#define MAX(a, b)           (((a) > (b)) ? (a) : (b))           // computes the maximal value of 2 values
#define MIN(a, b)           (((a) < (b)) ? (a) : (b))           // computes the minimal value of 2 values
#define ABS(a)              (((long) (a) < 0) ? (-(a)) : (a))   // computes the absolute value of a variable without calling a function

#define CHECK_VALUE_MAX_MIN_DEFAULT(a,max,min,defaultVal)               ( ( ((a) > (max)) || ((a) < (min)) ) ? (a = defaultVal) : (a=a))


#ifdef __cplusplus
}
#endif
#endif /* TARGET_CPP_DEFINE_HPP_ */
