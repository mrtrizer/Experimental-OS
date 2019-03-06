#include "irq.h"
#include "common.h"

//Interrupt controllers defines
#define PIC_MASER	0x20
#define PIC_SLAVE	0xA0
#define PIC_MASTER_COMMAND	PIC_MASER
#define PIC_MASTER_DATA	(PIC_MASTER + 1)
#define PIC_SLAVE_COMMAND	PIC_SLAVE
#define PIC_SLAVE_DATA	(PIC_SLAVE + 1)

//End of interrupt cmd code
#define PIC_EOI	0x20

//ICW1 flags
#define ICW1_ICW4	0x01
#define ICW1_SINGLE	0x02
#define ICW1_INTERVAL4	0x04
#define ICW1_LEVEL	0x08
#define ICW1_INIT	0x10

//ICW4 flags 
#define ICW4_8086	0x01
#define ICW4_AUTO	0x02
#define ICW4_BUF_SLAVE	0x08
#define ICW4_BUF_MASTER	0x0C
#define ICW4_SFNM	0x10

void PIC_remap(uint32 offsetMaster, uint32 offsetSlave)
{
	uint8 maskMaster, maskSlave; 
	maskMaster = inb(PIC_MASTER_DATA);
	maskSlave = inb(PIC_SLAVE_DATA);
	
	uint8 flags = ICW1_INIT + ICW1_ICW4;

	outb(PIC_MASTER_COMMAND, flags);
	outb(PIC_MASTER_DATA, offsetMaster);
	outb(PIC_MASTER_DATA, 4);
	outb(PIC_MASTER_DATA, ICW4_8086);
	outb(PIC_SLAVE_COMMAND, flags);
	outb(PIC_SLAVE_DATA, offsetSlave);
	outb(PIC_SLAVE_DATA, 2);
	outb(PIC_SLAVE_DATA, ICW4_8086);

	outb(PIC_MASTER_DATA, maskMaster);
	outb(PIC_SLAVE_DATA, maskSlave);
} 
