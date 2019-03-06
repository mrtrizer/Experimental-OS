#ifndef _IRQ_H
#define _IRQ_H

#include "types.h"

void remapPIC(uint32 offsetMaster, uint32 offsetSlave);
void initIRQ(uint32 vector, void (* callback) ());

#endif
