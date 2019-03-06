#ifndef TOOLS_H
#define TOOLS_H

#include <string>

class Tools
{
public:
    static int printDump(unsigned char * start, int size);
    static std::string split(std::string & strIn,char symb);
private:
    Tools(){}
    Tools(Tools &){}
};

#endif // TOOLS_H
