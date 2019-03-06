#ifndef VDRIVECONTROLLER_H
#define VDRIVECONTROLLER_H

#include "drivecontroller.h"

class VDriveController: public DriveController
{
public:
    VDriveController(char * buff, unsigned int size);
    char * readSector(int n);
    char * readSectors(int start, int length);

private:
    char * buff;
    unsigned int buffSize;
};

#endif // DRIVECONTROLLER_H
