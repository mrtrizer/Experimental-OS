#include <io.h>

#define SCREEN_HEIGHT	25
#define SCREEN_WIDTH	80
#define SCREEN_START	0xb8000
#define SCREEN_END	(SCREEN_START+SCREEN_HEIGHT*SCREEN_WIDTH*2)

unsigned char * curPos = (unsigned char *) SCREEN_START;

void clr()
{
	unsigned char * pos;
	for(pos = (unsigned char *)SCREEN_START; pos < (unsigned char *)SCREEN_END;pos++)
	{
		*pos = 0x0;
	}
	curPos = (unsigned char *)SCREEN_START;
}

int print(char * str)
{
	int n = 0;
	while (* str != 0x00)
	{
		n++;
		if (*str == '\n')
		{
			int intPos = (int)(curPos - SCREEN_START);
			int lineN = intPos/(SCREEN_WIDTH * 2) + 1; 
			curPos = (unsigned char *)SCREEN_START + lineN * SCREEN_WIDTH * 2;
			str++;
			continue;
		}
		if (curPos < (unsigned char *)SCREEN_END)
		{
			*((unsigned char*)curPos++) = *(str++);
			*((unsigned char*)curPos++) = 0x07;
		}
		else
			clr();
	}
	return n;
}

int printf(char * str, ...)
{
}

int printInt(long int num, int radix)
{
	char neg = 0;
	if (num < 0)
	{
		neg = 1;
		num = -num;
	}
	int length = num / radix + 1 + neg;
	unsigned char str[length + 1];
	str[length] = 0x00;
	unsigned char * currentChar = str + length - 1;
	unsigned char mod;
	while (num)
	{
		mod = num % radix;
		num = num / radix;
		if (mod > 9)
			*currentChar = mod + 'A';
		else
			*currentChar = mod + '0';
		currentChar--;
	}
	if (neg)
		*currentChar = '-';
	else
		currentChar++;
	return print((char *)currentChar);
}

void printRegs32()
{
	unsigned long reg = 0;
	print("Registers 32: \n");
	__asm("mov %%eax, %0":"=r"(reg));
	print("EAX = ");
	printInt(reg, 16);
	print("\nEAX = ");
	printInt(reg, 10);
	__asm("mov %%ebx, %0":"=r"(reg));
	print("\nEBX = ");
	printInt(reg, 16);
	__asm("mov %%ecx, %0":"=r"(reg));
	print("\nECX = ");
	printInt(reg, 16);
}

