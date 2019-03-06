#ifndef _STRUCTURE_H
#define _STRUCTURE_H

#include "config.h"
#include "types.h"

typedef struct ElfHeader
{
	uint32 num;
	uint32 size;
	uint32 addr;
	uint32 shndx;
} ElfHeader;

typedef struct MultibootInfo
{
	uint32 flags8;
	uint32 memLower;
	uint32 memUpper;
	uint32 bootDevice;
	uint32 cmdLine;
	uint32 modCount;
	uint32 modAddr;
	ElfHeader elfHeader;
	uint32 mmap_length;
	uint32 mmap_addr;
} MultibootInfo;

#endif
