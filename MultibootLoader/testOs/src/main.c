#include <config.h>
#include <structure.h>
#include <io.h>

int main (unsigned long magic, unsigned long addr)
{
	clr();
	print("Test kernel start.\n");
	printInt(100,16);
	print("\n");
	printInt(100,10);
	print("\n");
	printInt(-100,16);
	print("\n");
	printInt(-100,10);
	print("\n");
	printRegs32();
}
