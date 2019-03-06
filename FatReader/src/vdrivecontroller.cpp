#include "vdrivecontroller.h"
#include <inttypes.h>

VDriveController::VDriveController(char * buff, unsigned int size) //создает контроллер виртуального диска
{
    sectorSize = 512;
    this->buff = buff;
    buffSize = size;
}

char * VDriveController::readSector(int n) //чтение секторов
{
    uint32_t offset = n * sectorSize;
    return buff + offset; //буфер + смещение
}

char * VDriveController::readSectors(int start, int length)
{
    uint32_t offset = start * sectorSize;
    return buff + offset;
}
