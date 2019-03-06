
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
#include "fatcontroller.h"
#include "vdrivecontroller.h"
#include "dir.h"
#include "file.h"

using namespace std;

int main (int argc, char **argv)
{
    std::string fileName;

    if (argc < 2)
        cout << "No file name! Provide disk image as argument." << std::endl;

    fileName = argv[1];

    cout << "Drive image file name: " << fileName << std::endl;

	std::ifstream f(fileName.data(), std::ios::in | std::ios::binary);

    if(!f )
    {
        cout << "File open error!!!" << endl;
        return 1;
    }

    char * buff;
    ifstream::pos_type size = 0;

    if(f.seekg(0, std::ios::end))
       size = f.tellg();

	if (size == 0)
	{
		cout << "File is empty" << endl;
		return 2;
	}
	
    if(size && f.seekg(0, std::ios::beg))
    {
       buff = new char[size];
       f.read(&buff[0], size);
    }	

    cout<< "File size: " << size << endl;

    VDriveController driveController(buff,size);
    FatController fatController(&driveController);
    Dir* rootDir = fatController.getDir("/DIR2/");
    vector<File*> files = rootDir->getFileList();
    for (int i = 0; i < files.size(); i++)
    {
        if (files[i]->attr.dirrectory)
            cout << files[i]->shortName << " " << "DIR" <<endl;
        else
            cout << files[i]->shortName << " " <<  files[i]->size <<endl;
    }

    //fatController.printDebugInfo();
	return 0;
}
