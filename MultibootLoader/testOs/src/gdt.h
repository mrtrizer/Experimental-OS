#ifndef _GTD_H
#define _GTD_H

#include "types.h"

typedef struct gdtTableItem
{
	uint16 limitLow;
	uint16 baseLow;
	uint8 baseMiddle;
	uint8 access;
	uint8 granularity;
	uint8 baseHigh;
}__attribute__((packed));

typedef struct gdtPointer
{
	uint16 limit;
	uint32 base;
} __attrubute__((packed));

void gdtInit();
void gdtSetGate();

#endif


