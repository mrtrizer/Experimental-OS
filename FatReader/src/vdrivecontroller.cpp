#include "vdrivecontroller.h"

VDriveController::VDriveController(char * buff, size_t size)
{
    sectorSize = 512;
    this->buff = buff;
    buffSize = size;
}

char * VDriveController::readSector(uint32_t n)
{
    uint32_t offset = n * sectorSize;
    return buff + offset;
}

char * VDriveController::readSectors(uint32_t start, uint32_t)
{
    uint32_t offset = start * sectorSize;
    return buff + offset;
}
