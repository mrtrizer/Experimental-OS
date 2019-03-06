#ifndef FATCLUSTER_H
#define FATCLUSTER_H

#include <inttypes.h>

class FatCluster
{
public:
    FatCluster(uint32_t index);
    char * data;
    int index;
};

#endif // FATCLUSTER_H
