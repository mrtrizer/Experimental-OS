#ifndef DIR_H
#define DIR_H

#include "file.h"
#include <vector>

class Dir
{
friend class FatController;

public:
    Dir(FatController *fatController, char * buff, uint32_t length);
    ~Dir();
    std::vector<File *> getFileList(){return files;}
    File * searchFile(std::string &name, uint32_t attrMask);

private:
    std::vector<File *> files;
    uint32_t length;
    FatController * fatController;

};

#endif // DIR_H
