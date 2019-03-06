#ifndef VDRIVECONTROLLER_H
#define VDRIVECONTROLLER_H

#include "drivecontroller.h"

#include <cstddef>

class VDriveController: public DriveController
{
public:
    VDriveController(char * buff, size_t size);
    char * readSector(uint32_t n);
    char * readSectors(uint32_t start, uint32_t length);

private:
    char * buff;
    size_t buffSize;
};

#endif // DRIVECONTROLLER_H
