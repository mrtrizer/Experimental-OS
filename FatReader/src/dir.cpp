#include "dir.h"
#include "fatstructures.h"
#include "fatcontroller.h"

using namespace std;

#define ATTR_READ_ONLY 0x01
#define ATTR_HIDDEN 0x02
#define ATTR_SYSTEM 0x04
#define ATTR_VOLUME_ID 0x08
#define ATTR_DIRRECTORY 0x10
#define ATTR_ARCHIVE 0x20
#define ATTR_LONG_NAME (ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID)


Dir::Dir(FatController * fatController, char *buff, uint32_t length)
{
    FileStruct * fileStruct = (FileStruct *)(buff);
    this->fatController = fatController;

    if (fileStruct->Name[0] == 0xF6) //если каталог пуст
        return;

    while (((char *)fileStruct) < (buff + length))
    {
        if(fileStruct->Name[0] == 0)
            break;
        //Если (ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID), то это часть длинного имени, а не файл
        if (fileStruct->Attr != 0x0F)
        {
            File * file = new File(fatController,fileStruct->Attr,
                                   (fileStruct->FstClusHI << 8) | (fileStruct->FstClusLO),fileStruct->FileSize); //№1го кластера и размер
            file->setShortName((char*)fileStruct->Name);
            files.push_back(file); //добавляем элемент в конец вектора
        }
        fileStruct++;
    }
}

Dir::~Dir()
{
    for(size_t i = 0; i < files.size(); i++)
        delete files[i];
}

File * Dir::searchFile(const string & name, uint32_t attrMask)
{
    for(size_t i = 0; i < files.size(); i++)
        if ((files[i]->shortName == name) &&
                (*((uint32_t*)(&files[i]->attr)) & attrMask))
            return files[i];
    return nullptr;
}
