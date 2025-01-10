#ifndef COMDEF_H_INCLUDED
#define COMDEF_H_INCLUDED

//===================================================================== INCLUDES
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "String.h"
#include <stdbool.h>
#include "ctype.h"

#if _KEILIDE_
	#ifdef RTE_DEVICE_FRAMEWORK_CUBE_MX
	 #include "MX_Device.h"
	#else
	 #include "RTE_Components.h"
	 #include "RTE_Device.h"
	#endif
#else
	#if defined(_STM32F4XX_)
		#include "stm32f4xx_hal.h"
	#endif
	#if defined(_STM32F7XX_)
		#include "stm32f7xx_hal.h"
	#endif
	#if defined(_STM32G4XX_)
		#include "stm32g4xx_hal.h"
	#endif
	#if defined(_STM32L0XX_)
		#include "stm32l0xx_hal.h"
	#endif
	#if defined(_STM32H7XX_)
		#include "stm32h7xx_hal.h"
	#endif
#endif

//====================================================================== DEFINES

#ifndef _ON_
	#define _ON_ 		0x01
#endif /* ON */
#ifndef _OFF_
	#define _OFF_		0x00
#endif /* OFF */
#ifndef _EOR_
	#define _EOR_		0x02
#endif

#ifndef _ENABLED_
	#define _ENABLED_	1
#endif
#ifndef _DISABLED_
	#define _DISABLED_	0
#endif

#ifndef _NULL_
	#define _NULL_ 	0
#endif
		   
#ifndef _DELETE_
	#define _DELETE_(arg)					if(arg){delete(arg);arg=_NULL_;}
#endif
		   
#ifndef _MIN_
   #define _MIN_(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef _MAX_
   #define _MAX_(a, b) ((a) > (b) ? (a) : (b))
#endif
#ifndef _ABS_
	#define _ABS_(a) ((a) < 0 ? -(a) : (a))
#endif

#define _LIMIT_(val,min,max)				if(val>max){val=max;}else if(val<min){val=min;}

#ifndef _MALLOC_
	#if _LIBSUPPORT_FREERTOS == _ENABLED_	//niknik da sistemare
		//extern void *pvPortMalloc( size_t xWantedSize );// PRIVILEGED_FUNCTION;
		#include "FreeRTOS.h"
		#define _MALLOC_(a) pvPortMalloc(a)
	#else
		#define _MALLOC_(a) malloc(a)
	#endif
#endif
#ifndef _CALLOC_
	#if _LIBSUPPORT_FREERTOS == _ENABLED_	//niknik da sistemare
		//extern void *pvPortMalloc( size_t xWantedSize );// PRIVILEGED_FUNCTION;
		#include "FreeRTOS.h"
		#define _CALLOC_(a) pvPortCalloc(a)
	#else
		#define _CALLOC_(a) calloc(a,1)
	#endif
#endif
#ifndef _FREE_
	#if _LIBSUPPORT_FREERTOS == _ENABLED_
		#include "FreeRTOS.h"
		#define _FREE_(arg)					 	if(arg){vPortFree(arg);arg=_NULL_;}
	#else
		#define _FREE_(arg)					 	if(arg){free(arg);arg=_NULL_;}
	#endif
#endif

#ifndef _PORTSETOUT_		//**************************************************
	#if defined(XMC1404_F064x0200)
		#define _PORTSETOUT_(port,mask)				port->OMR = mask
	#elif defined(_STM32F4XX_) || defined(_STM32F7XX_) || defined(_STM32G4XX_) || defined(_STM32H7XX_) || defined(_STM32L0XX_)
		#define _PORTSETOUT_(port,mask)				port->BSRR = mask
	#else
		#error "ComDef.h : Microprocessore NON supportato. Implementarlo!!!"
	#endif
#endif
#ifndef _PORTRESETOUT_		//**************************************************
	#if defined(XMC1404_F064x0200)
		#define _PORTRESETOUT_(port,mask)			port->OMR = (((uint32_t)mask)<<16)
	#elif defined(_STM32G4XX_) || defined(_STM32L0XX_)
		#define _PORTRESETOUT_(port,mask)			port->BRR = (((uint32_t)mask))
	#elif defined(_STM32F4XX_) || defined(_STM32F7XX_) || defined(_STM32H7XX_)
		#define _PORTRESETOUT_(port,mask)			port->BSRR = (((uint32_t)mask)<<16)
	#else
		#error "ComDef.h : Microprocessore NON supportato. Implementarlo!!!"
	#endif
#endif
#ifndef _PORTEOROUT_		//**************************************************
	#if defined(XMC1404_F064x0200)
		#define _PORTEOROUT_(port,mask)				port->OMR = (mask|(((uint32_t)mask)<<16))
	#elif defined(_STM32F4XX_) || defined(_STM32F7XX_) || defined(_STM32G4XX_) || defined(_STM32H7XX_) || defined(_STM32L0XX_)
		#define _PORTEOROUT_(port,mask)				if(port->ODR & mask){_PORTRESETOUT_(port,mask);}else{_PORTSETOUT_(port,mask);}
	#else
		#error "ComDef.h : Microprocessore NON supportato. Implementarlo!!!"
	#endif
#endif
#ifndef _PORTGETIN_
	#if defined(XMC1404_F064x0200)
		#define _PORTGETIN_(port,mask)				(port->IN & mask)
	#elif defined(_STM32F4XX_) || defined(_STM32F7XX_) || defined(_STM32G4XX_) || defined(_STM32H7XX_) || defined(_STM32L0XX_)
		#define _PORTGETIN_(port,mask)				(port->IDR & mask)
	#else
		#error "ComDef.h : Microprocessore NON supportato. Implementarlo!!!"
	#endif
#endif
		 
#ifndef	_NELEMENTS_
	#define _NELEMENTS_(arg)				(sizeof(arg) / sizeof(arg[0]))
#endif

#ifndef _LIMIT_
	#define _LIMIT_(arg,min,max)				(if(arg<min){arg=min}elif(arg>max){arg=max})
#endif


	
#define _SQRT2_				1.414213f
#define _SQRT3_ 			1.732050807569f

#define _GRADIGIRO_			360
#define _PI_ 				3.141592654		// Pigreco
#define _DUEPI_ 			6.283185307		// 2*Pigreco
#define _PIMEZZI_			1.570796327		// Pigreco diviso 2
#define _TREMEZZIPI_		4.712388980		// due terzi pigreco

#define _STATUS_OK_   0 	// Operation succeeded 
#define _STATUS_FAIL_ -1 	// Unspecified error

#define _B0_  	0x00000001
#define _B1_ 	0x00000002
#define _B2_	0x00000004
#define _B3_ 	0x00000008
#define _B4_ 	0x00000010
#define _B5_ 	0x00000020
#define _B6_ 	0x00000040
#define _B7_ 	0x00000080
#define _B8_ 	0x00000100
#define _B9_ 	0x00000200
#define _B10_ 	0x00000400
#define _B11_ 	0x00000800
#define _B12_ 	0x00001000
#define _B13_ 	0x00002000
#define _B14_ 	0x00004000
#define _B15_ 	0x00008000
#define _B16_ 	0x00010000
#define _B17_ 	0x00020000
#define _B18_ 	0x00040000
#define _B19_ 	0x00080000
#define _B20_ 	0x00100000
#define _B21_ 	0x00200000
#define _B22_ 	0x00400000
#define _B23_ 	0x00800000
#define _B24_ 	0x01000000
#define _B25_ 	0x02000000
#define _B26_ 	0x04000000
#define _B27_ 	0x08000000
#define _B28_ 	0x10000000
#define _B29_ 	0x20000000
#define _B30_ 	0x40000000
#define _B31_ 	0x80000000

typedef enum
{	// ATTENZIONE TENERE ALLIENATI TIPI e ARRAY
	_TYPEDATA_BOOL_		,
	_TYPEDATA_UINT8_	,
	_TYPEDATA_SINT8_	,
	_TYPEDATA_UINT16_	,
	_TYPEDATA_SINT16_	,
	_TYPEDATA_UINT32_	,
	_TYPEDATA_SINT32_	,
	_TYPEDATA_FLOAT_	,
	_TYPEDATA_DOUBLE_	,
	_TYPEDATA_PTRSTR_	,

	_TYPEDATA_ARRAYOFBOOL_		,
	_TYPEDATA_ARRAYOFUINT8_		,
	_TYPEDATA_ARRAYOFSINT8_		,
	_TYPEDATA_ARRAYOFUINT16_	,
	_TYPEDATA_ARRAYOFSINT16_	,
	_TYPEDATA_ARRAYOFUINT32_	,
	_TYPEDATA_ARRAYOFSINT32_	,
	_TYPEDATA_ARRAYOFFLOAT_		,
	_TYPEDATA_ARRAYOFDOUBLE_	,
	_TYPEDATA_ARRAYOFPTRSTR_	,

	_TYPEDATA_MAX_
}_TypeData_e;

//============================================================= GLOBAL VARIABLES

//============================================================ GLOBAL PROTOTYPES

//============================================================= INLINE FUNCTIONS

#endif /* COMDEF_H_INCLUDED */
