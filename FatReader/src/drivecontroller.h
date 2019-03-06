#ifndef DRIVECONTROLLER_H
#define DRIVECONTROLLER_H

class DriveController
{
public:
    DriveController(){sectorSize = 512;}
    virtual char * readSector(int n) = 0;
    virtual char * readSectors(int start, int length) = 0;
    virtual void setSectorSize(int size){sectorSize = size;}
protected:
    int sectorSize;
};

#endif // DRIVECONTROLLER_H
