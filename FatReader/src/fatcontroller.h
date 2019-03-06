#ifndef FATCONTROLLER_H
#define FATCONTROLLER_H

#include "fatstructures.h"
#include "vdrivecontroller.h"
#include "fatcluster.h"
#include <string>
#include <vector>

#define FAT_CONTROLLER_DEBUG
#define PROPERTY_GET(property,type,func) type func(){return property;}

class Dir;
class File;

class FatController
{
friend class Dir;
friend class File;

public:
    class bad_structure{};
    class fat32_unsupported{};

    enum FATType {FAT12_T = 12, FAT16_T = 16, FAT32_T = 32};
    enum ClusterType {FREE = 0,BAD = 1, RESERVED = 2, NEXT = 3, END = 4, BAD_TYPE = 0xFF};
    struct FATNode
    {
        ClusterType type;
        uint32_t nextCluster; //Если тип кластера - указатель на следующий кластер
        FATNode(ClusterType t = BAD_TYPE, uint32_t n = 0){type = t; nextCluster = n;}
    };

    FatController(DriveController *controller);
    Dir *getDir(std::string path);
    File * getFile(std::string path);

    PROPERTY_GET(fatType, FATType, getFatType)
    PROPERTY_GET(clusterCount, int, getClusterCount)
    PROPERTY_GET(clusterSize, int, getClusterSize)
    PROPERTY_GET(sectorSize, int, getSectorSize)
    PROPERTY_GET(reservedSectCount, int, getRsrvdSectCount)
    PROPERTY_GET(firstClasterSector, int, getFirstClstrSect)

#ifdef FAT_CONTROLLER_DEBUG
    void printDebugInfo();
#endif

private:
    int readCluster(FatCluster * cluster);
    Dir *getRootDir();
    void fatInit(char *buff);
    FATNode getFatValue(uint32_t n);
    uint32_t getFatOffset(uint32_t n);
    void setFatValue(uint32_t n, uint32_t value);
    FATNode fat12Type(uint16_t valueFat12);
    FATNode fat16Type(uint16_t valueFat16);
    FATNode fat32Type(uint32_t valueFat32);


    BootStruct * bootStruct;
    FATType fatType;
    size_t clusterCount;
    uint32_t firstClasterSector;
    uint32_t sectorSize;
    uint32_t clusterSize;
    uint32_t reservedSectCount;
    DriveController * driveController;
    uint32_t firstRootSector;
};

#endif // FATCONTROLLER_H
