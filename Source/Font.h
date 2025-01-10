#ifndef APPLICAZIONE_H_INCLUDED
#define APPLICAZIONE_H_INCLUDED

#include "ARMLib/Common/ComDef.h"

typedef struct
{
	const uint8_t *data;
	uint16_t width;
	uint16_t height;
	uint8_t dataSize;
} tImage;
typedef struct
{
	long int code;
	const tImage *image;
} tChar;
typedef struct
{
	int length;
	const tChar *chars;
} tFont;


extern const tFont Font;

#endif /* APPLICAZIONE_H_INCLUDED */
