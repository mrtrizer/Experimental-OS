#include "file.h"
#include "fatcontroller.h"

using namespace std;

File::File(FatController * fatController, uint8_t attr, uint32_t firstCluster, uint32_t size)
{
    this->fatController = fatController;
    clusters = 0;
    this->attr = *((Attr *)&attr);
    this->firstCluster = firstCluster;
    this->size = size;
    this->sectorCount = this->size / fatController->sectorSize
            + (this->size % fatController->sectorSize)?0:1; //Округляем в большую сторону
}

File::~File()
{
    if (clusters != 0)
        close();
}

void File::setShortName(string shortName)
{
    this->shortName = shortName.substr(0,8); //считываем первые 8 символов
    this->shortName = this->shortName.erase(this->shortName.find_last_not_of(" \n\r\t")+1);  //остальные удаляем
}

void File::setFullName(string fullName)
{
    this->fullName = fullName;
}

//Чтение цепочек кластеров
//При открытии файла читаются цепочки кластеров, для того, что бы затем иметь быстрый доступ к ним.
int File::open()
{
    if (clusters != 0)
        return 1; //Уже открыт
    clusters = new vector<FatCluster *>;
    if (firstCluster == 0)
        return 2; //Пустой файл
    uint32_t clusterN = firstCluster;
    FatCluster * currentCluster = new FatCluster(clusterN);
    sectorCount = 1;

    while (1) //Читаем цепочку кластеров
    {
        fatController->readCluster(currentCluster);
        FatController::FATNode fatEntrie = fatController->getFatValue(clusterN); //описание эл.в фат таблице
        clusters->push_back(currentCluster);
        if (fatEntrie.type == FatController::NEXT)
        {
            clusterN = fatEntrie.nextCluster;
            currentCluster = new FatCluster(clusterN);
        }
        else
            if (fatEntrie.type == FatController::END)
                break;
            else
                throw bad_fat_item(); //Not NEXT and not END.
        sectorCount++;
    }
    return 0;
}

int File::close()
{
    if (clusters == 0)
        return 1;
    for (int i = 0; i < clusters->size(); i++)
        delete (*clusters)[i];
    delete clusters;
    clusters = 0;
}

//Чтение файла. С определенного смещения.
int File::read(uint32_t offset, uint32_t n, char * buff)
{
    if (clusters == 0)
        return 1; //Не открыт
    if (clusters->size() == 0)
        return 2; //Пустой файл
    uint32_t clusterSize = fatController->clusterSize *
                                fatController->sectorSize;
    uint32_t clusterStart = firstCluster;
    uint32_t clusterCount = (n > clusterSize)? (n / clusterSize): (1);
    uint32_t currentOffset = offset % clusterSize;
    uint32_t currentCluster = findCluster(clusterStart);
    uint32_t currentBuffOffset = 0;
    while (clusterCount > 1)
    {
        memcpy(buff + currentBuffOffset,(*clusters)[currentCluster]->data + currentOffset,
               clusterSize - currentOffset);
        currentOffset = 0;
        clusterCount--;
        n -= clusterSize - currentOffset;
        currentBuffOffset += clusterSize;
    }
    memcpy(buff + currentBuffOffset, (*clusters)[currentCluster]->data + currentOffset,
           (n == clusterSize)?clusterSize:(n % clusterSize));
    return 0;
}

int File::findCluster(int n)
{
    for (int i = 0; i < clusters->size(); i++)
        if ((*clusters)[i]->index == n)
            return i;
    return -1;
}
