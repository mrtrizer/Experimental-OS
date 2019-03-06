#ifndef FILE_H
#define FILE_H

#include <string>
#include <inttypes.h>
#include <vector>
#include "fatcluster.h"

class FatController;

class File
{
    friend class Dir;
    friend class FatController;
public:
    void setShortName(std::string shortName);
    void setFullName(std::string fullName);
    int open();
    int close();
    int read(uint32_t offset, uint32_t n, char * buff);

    std::string shortName;
    std::string fullName;
    uint32_t firstCluster;
    uint32_t size;
    uint32_t sectorCount;
    std::vector<FatCluster *> * clusters;

    struct Attr
    {
        uint32_t readOnly:1;
        uint32_t hidden:1;
        uint32_t system:1;
        uint32_t volumeId:1;
        uint32_t dirrectory:1;
        uint32_t archive:1;
    } attr;

    //Interrupts classes
    class bad_fat_item{};

private:
    File(FatController * fatController, uint8_t attr, uint32_t firstCluster, uint32_t size);
    ~File();
    FatController * fatController;
    int findCluster(int n);
};

#endif // FILE_H
