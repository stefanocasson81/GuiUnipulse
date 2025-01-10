#ifndef CRC_H_INCLUDED
#define CRC_H_INCLUDED

//===================================================================== INCLUDES
#include "ARMLib/Common/ComDef.h"

//====================================================================== DEFINES

//============================================================= GLOBAL VARIABLES

//============================================================ GLOBAL PROTOTYPES

uint32_t crc_CalcCrc32(uint8_t *ptrBuff, uint32_t len, uint32_t crc32Partenza);
uint16_t crc_CalcCrc16(uint8_t *ptrBuff, uint32_t len, uint16_t crc16Partenza);


//============================================================= INLINE FUNCTIONS

#endif /* CRC_H_INCLUDED */


