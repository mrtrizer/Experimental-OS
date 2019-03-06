#include "fatcontroller.h"

#include <iostream>
#include <iterator>
#include <fstream>
#include <iomanip>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <math.h>
#include <vector>
#include <string>
#include <strings.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "file.h"
#include "dir.h"
#include "tools.h"

using namespace std;

#define TRUE_STRUCTURE 16

FatController::FatController(DriveController * controller) //создает драйвера файловой системы
{
    this->driveController = controller;
    char * buff = controller->readSector(0); //читаем в буфер нулевой сектор
    bootStruct = (BootStruct *) buff;
    //проверка компилятора.
    if (TRUE_STRUCTURE != (intptr_t) &(bootStruct->BPB_NumFATs) - (intptr_t)bootStruct)
    {
        cout << "BAD STUCTURE. See compiler's directives." << endl;
        throw bad_structure();
    }
    fatInit(buff); //Инициализация файловой системы. Определение всех параметров.
}

//Инициализация файловой системы. Определение всех параметров.
void FatController::fatInit(char * buff)
{
    uint32_t FATSz;
    uint16_t TotSec;
    uint32_t DataSec;
    uint32_t RootDirSectors;
    uint32_t CountOfClusters;
    bootStruct = (BootStruct *) buff;

    //Определение размера корневой дирректории
    RootDirSectors = ((bootStruct->BPB_RootEntCnt * 32) + (bootStruct->BPB_BytsPerSec - 1)) / bootStruct->BPB_BytsPerSec;


    if(bootStruct->BPB_FATSz16 != 0)
        FATSz = bootStruct->BPB_FATSz16;
    else
        FATSz = bootStruct->fatTile.fat32.BPB_FATSz32;

    if(bootStruct->BPB_TotSec16 != 0)
        TotSec = bootStruct->BPB_TotSec16;
    else
        TotSec = bootStruct->BPB_TotSec32;

    //Вычисление количества кластеров
    DataSec = TotSec - (bootStruct->BPB_RsvdSecCnt + (bootStruct->BPB_NumFATs * FATSz) + RootDirSectors);
    CountOfClusters = DataSec / bootStruct->BPB_SecPerClus;

    //Определение типа fat
    if(CountOfClusters < 4085)
        fatType = FAT12_T;
    else
    {
        if(CountOfClusters < 65525)
            fatType = FAT16_T;
        else
            fatType = FAT32_T;
    }

    //Остальные вычисления
    clusterCount = CountOfClusters; //кол-во кластеров
    if ((fatType == FAT12_T) || (fatType == FAT16_T))
        firstRootSector = bootStruct->BPB_RsvdSecCnt + (bootStruct->BPB_NumFATs * FATSz);
    else
        throw fat32_unsupported();
    firstClasterSector = bootStruct->BPB_RsvdSecCnt + (bootStruct->BPB_NumFATs * FATSz) + RootDirSectors; //начальный кластер
    sectorSize = bootStruct->BPB_BytsPerSec; //размер сектора
    driveController->setSectorSize(sectorSize);
    clusterSize = bootStruct->BPB_SecPerClus; //размер кластера
    reservedSectCount = bootStruct->BPB_RsvdSecCnt; //количество секторов в Reserved region

}

//Читает данные кластера
int FatController::readCluster(FatCluster * cluster)
{
    if (cluster->data != 0)
        return 1; //Уже прочитан
    if (cluster->index > clusterCount)
        return 2; //Выход за границы
    cluster->data = driveController->readSector(firstClasterSector + (cluster->index - 2) * clusterSize);
    return 0;
}

//Получить указатель на файл по пути
File * FatController::getFile(string path)
{
    string fileName;
    for (int i = path.length() - 1; i > 0; i--)
        if (path[i] == '/')
        {
            fileName = path.c_str() + i + 1;
            path.erase(path.begin() + i + 1,path.end());
            break;
        }
    Dir * dir = getDir(path);
    //Затем в каталоге найти файл Dir::searhFile(ИМЯ);
    File * file = new File(*(dir->searchFile(fileName,0xFF)));
    return file;
    //return 0;
}

//Получить дирректорию по пути
Dir * FatController::getDir(string path)
{
    int i, j;
    string fileName;
    string *name;
    char * dirBuff;
    name = (string*)malloc(sizeof(string)*sizeof(path));
    if (path == "/") //Заглушка для корневой дирректории
        return getRootDir();
    //Разделить строку на составляющие и прочитать корневую дирректорию
    Dir * currentDir = getRootDir();
    currentDir->files.erase(currentDir->files.begin()); //Убирает первый, незначимый файл в корне
    i = 1;
    while (i != path.length())  //пока не прошли всю строку
    {
        if (path[i] != '/') //если нет признака разделения каталогов
        {
            fileName += path[i]; //складываем символы
            i++;
            continue;
        }
        File * file = currentDir->searchFile(fileName,0x10);
        if (file == NULL) //получаем слово, если такого нет, то
        {
            cout << "\nDirrectory not found. Dirrectory: " << fileName << endl;
            return 0;
        }
        try
        {
            file->open();
        }
        catch (File::bad_fat_item)
        {
            cout << "\nFile open error. File: " << fileName << endl;
            return 0;
        }

        dirBuff = new char[file->sectorCount * sectorSize];
        file->read(0,file->sectorCount * sectorSize,dirBuff);
        //УТЕЧКА ПАМЯТИ!!!!!!! Надо придумать механизм, вроде сохранеия дерева каталогов
        currentDir  = new Dir(this,dirBuff,file->sectorCount * sectorSize);
        delete []dirBuff;
        fileName.clear();
        i++;

    } //последнее name[j] это и будет полученное имя файла в пройденных каталогах
    return currentDir;
}

//Получить корневую дирректорию +
Dir * FatController::getRootDir()
{
    if ((fatType == FAT12_T) || (fatType == FAT16_T))
    {
        //УТЕЧКА
        return new Dir(this, driveController->readSectors(firstRootSector,32), 32 * sectorSize);
    }


}

//Выдает смещение относительно начала Fat таблицы -
uint32_t FatController::getFatOffset(uint32_t n)
{
    uint32_t FATSz;
    uint32_t FATOffset;

    if(bootStruct->BPB_FATSz16 != 0)
        FATSz = bootStruct->BPB_FATSz16;
    else
        FATSz = bootStruct->fatTile.fat32.BPB_FATSz32;

    n -=2; //первые две позиции зарезервированы (вроде)
    //ВНИМАНИЕ! ЕСЛИ ФАЙЛЫ ЧИТАЮТСЯ КРИВО!
    //Это сомнительный код, не знаю, почему так. но на том образе дискеты, который есть у меня есть,
    //указатели идут через один. Есть мимолетное предположение, что я просто не так сдвигаю.
    n *= 2;

    switch (fatType)
    {
    case FAT12_T:
        FATOffset = n + (n / 2);
        break;
    case FAT16_T:
        FATOffset = n * 2;
        break;
    case FAT32_T:
        FATOffset = n * 4;
        break;
    }

    return FATOffset + 2;
}

//Устанавливает значение в таблицу фат (на будущее, если вдруг понадобится запись) -
void FatController::setFatValue(uint32_t n, uint32_t value)
{

}

//Определение типа записи в фат-таблице для fat12
FatController::FATNode FatController::fat12Type(uint16_t valueFat12)
{
    if (valueFat12 == 0x0000)
        return FATNode(FREE);
    if ((valueFat12 > 0x0000) && (valueFat12 < clusterCount) && (valueFat12 < 0x0FF0))
        return FATNode(NEXT,valueFat12);
    if ((valueFat12 >= 0x0FF0) && (valueFat12 <= 0x0FF6))
        return FATNode(RESERVED);
    if (valueFat12 == 0x0FF7)
        return FATNode(BAD);
    if (valueFat12 == 0x0FFF)
        return FATNode(END);
}

//Определение типа записи в фат-таблице для fat16
FatController::FATNode FatController::fat16Type(uint16_t valueFat16)
{
    if (valueFat16 == 0x0000)
        return FATNode(FREE);
    if ((valueFat16 > 0x0000) && (valueFat16 < clusterCount) && (valueFat16 < 0xFFF0))
        return FATNode(NEXT,valueFat16);
    if ((valueFat16 >= 0xFFF0) && (valueFat16 <= 0xFFF6))
        return FATNode(RESERVED);
    if (valueFat16 == 0xFFF7)
        return FATNode(BAD);
    if (valueFat16 == 0xFFFF)
        return FATNode(END);
}

//Определение типа записи в фат-таблице для fat32
FatController::FATNode FatController::fat32Type(uint32_t valueFat32)
{
    if (valueFat32 == 0x00000000)
        return FATNode(FREE);
    if ((valueFat32 > 0x00000000) && (valueFat32 < clusterCount) && (valueFat32 < 0x0FFFFFF0))
        return FATNode(NEXT,valueFat32);
    if ((valueFat32 >= 0x0FFFFFF0) && (valueFat32 <= 0x0FFFFFF6))
        return FATNode(RESERVED);
    if (valueFat32 == 0x0FFFFFF7)
        return FATNode(BAD);
    if (valueFat32 == 0x0FFFFFFF)
        return FATNode(END);
}

//Получить значение из фат таблицы в общем для fat12 fat16 и fat32 виде -
FatController::FATNode FatController::getFatValue(uint32_t n)
{
    uint32_t offsetFatT = getFatOffset(n);

    //Рассчет сектора и смещение внутри сектора
    uint32_t sector = reservedSectCount + (offsetFatT / sectorSize);
    uint32_t offset = offsetFatT % sectorSize;

    char * buff = driveController->readSector(sector);

    switch (fatType)
    {
    case FAT12_T:
    {
        uint16_t valueFat12;
        valueFat12 = *((uint16_t *) (buff + offset));
        if((n % 2))
            valueFat12 = (valueFat12 >> 4) & 0x0FFF;
        else
            valueFat12 = valueFat12 & 0x0FFF;

        return fat12Type(valueFat12);
    }
    break;
    case FAT16_T:
    {
        uint16_t valueFat16;
        valueFat16 = *((uint16_t *) buff  + offset);
        return fat16Type(valueFat16);
    }
    break;
    case FAT32_T:
    {
        uint32_t valueFat32;
        valueFat32 = (*((uint32_t *) buff + offset)) & 0x0FFFFFFF;
        return fat32Type(valueFat32);
    }
    break;
    }
    return FATNode();
}

//Отладочная функция
#ifdef FAT_CONTROLLER_DEBUG
void FatController::printDebugInfo()
{
    //printDump((unsigned char*)buff,512);
    uint32_t pos = 0;
    cout << endl << "Fat offset for " << pos << ": " << getFatOffset(pos) << endl; //смещение от начала
    cout << endl << "Fat value at pos " << pos << ": " << getFatValue(pos).type << hex
         << " " << getFatValue(pos).nextCluster << endl;
    Dir * dir = getRootDir();
    vector<File*> fileList = dir->getFileList();
    File * file = fileList[1];
    cout << file->size;
    file->open();
    char * fileBuff = new char[file->sectorCount * sectorSize];
    fileList[1]->read(0,100,fileBuff);
    Tools::printDump((unsigned char *)fileBuff,100);

    for (int i = 0; i < fileList.size(); i++)
    {
        cout << fileList[i]->shortName << endl;
    }
    delete []fileBuff;
}
#endif
