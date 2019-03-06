#ifndef FATSTRUCTURES_H
#define FATSTRUCTURES_H

#include <stdint.h>

//Окончание структуры (со смещениея 36) BootSector для FAT16 и FAT12
typedef struct BootStructFAT12_16
{
    uint8_t BS_DrvNum; // 0x40 диск с которым работаем drive
    uint8_t BS_Reserved1; //reserved
    uint8_t BS_BootSig; //signature
    uint32_t BS_VolID;
    uint8_t BS_VolLab[11]; // 0x47 label[11]
    uint8_t BS_FilSysType[8]; //fat_format[8]
} __attribute__((__packed__)) BootStructFAT12_16;

//Окончание структуры (со смещениея 36) BootSector для FAT32
typedef struct BootStructFAT32
{
    uint32_t BPB_FATSz32;
    uint16_t BPB_ExtFlags;
    uint16_t BPB_FSVer;
    uint32_t BPB_RootClus;
    uint16_t BPB_FSInfo;
    uint16_t BPB_BkBootSec;
    uint8_t BPB_Reserved[12];
    uint8_t BS_DrvNum;
    uint8_t BS_Reserved1;
    uint8_t BS_BootSig;
    uint32_t BS_VolID;
    uint8_t BS_VolLab[11];
    uint8_t BS_FilSysType[8];
}  __attribute__((__packed__))  BootStructFAT32;

typedef struct BootStruct //boot сектор
{
    uint8_t BS_jmpBoot[3]; // 0x00 jmp command to boot code
    uint8_t BS_OEMName[8]; // 0x03 OEM Name
    uint16_t BPB_BytsPerSec; //размер сектора sectsize
    uint8_t BPB_SecPerClus; //размер кластера clustsize
    uint16_t BPB_RsvdSecCnt; //ressecs
    uint8_t BPB_NumFATs; //fatcnt
    uint16_t BPB_RootEntCnt; //размер читаемого диска
    uint16_t BPB_TotSec16; //TotSec16
    uint8_t BPB_Media; //Media
    uint16_t BPB_FATSz16; //размер файловой системы FATSz16 fatsize
    uint16_t BPB_SecPerTrk;
    uint16_t BPB_NumHeads; //headcnt
    uint32_t BPB_HiddSec;
    uint32_t BPB_TotSec32 ;
    union
    {
        BootStructFAT12_16 fat12_16;
        BootStructFAT32 fat32;
    } fatTile;
} __attribute__((__packed__)) BootStruct;


typedef struct FatTime
{
  uint16_t sec : 5, min : 6, hour : 5;
}  __attribute__((__packed__))  FatTime;

typedef struct FatDate
{
  uint16_t day : 5, month : 4, year : 7;
}  __attribute__((__packed__))  FatDate;

typedef struct FileStruct
{
  uint8_t Name[11];
  uint8_t Attr;
  uint8_t NTRes;
  uint8_t CrtTimeTenth;
  uint16_t CrtTime;
  uint16_t CrtDate;
  uint16_t LstAccDate;
  uint16_t FstClusHI;
  FatTime WrtTime;
  FatDate WrtDate;
  uint16_t FstClusLO;
  uint32_t FileSize;
}  __attribute__((__packed__))  FileStruct;

#endif // FATSTRUCTURES_H
