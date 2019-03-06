#ifndef DRIVECONTROLLER_H
#define DRIVECONTROLLER_H

#include <cinttypes>

class DriveController
{
public:
    virtual ~DriveController() = default;
    virtual char * readSector(uint32_t n) = 0;
    virtual char * readSectors(uint32_t start, uint32_t length) = 0;
    virtual void setSectorSize(unsigned size){sectorSize = size;}
protected:
    unsigned sectorSize = 512;
};

#endif // DRIVECONTROLLER_H
