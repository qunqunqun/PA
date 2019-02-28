#include "cpu/cpu.h"
#include "memory/memory.h"
#include <stdio.h>
// return the linear address from the virtual address and segment selector
uint32_t segment_translate(uint32_t offset, uint8_t sreg) {
	/* TODO: perform segment translation from virtual address to linear address
	 * by reading the invisible part of the segment register 'sreg'
	 */
	//printf("sreg:%d\n",sreg);
	
	return cpu.segReg[sreg].base + offset;
}

// load the invisible part of a segment register
void load_sreg(uint8_t sreg) {
	/* TODO: load the invisibile part of the segment register 'sreg' by reading the GDT.
	 * The visible part of 'sreg' should be assigned by mov or ljmp already.
	 */
	//printf("%x\n",cpu.segReg[sreg].index);
	laddr_t addr = cpu.gdtr.base + sizeof(SegDesc) * cpu.segReg[sreg].index;
	SegDesc segdesc;
	segdesc.val[0] = laddr_read(addr , 4);
	segdesc.val[1] = laddr_read(addr + 4,4);
	cpu.segReg[sreg].base = segdesc.base_15_0 | (segdesc.base_23_16<<16) | (segdesc.base_31_24<<24);
	cpu.segReg[sreg].limit = segdesc.limit_15_0 | (segdesc.limit_19_16<<16);
	cpu.segReg[sreg].privilege_level = segdesc.privilege_level;
	cpu.segReg[sreg].soft_use = segdesc.soft_use;
	cpu.segReg[sreg].type = segdesc.type;
	assert(cpu.segReg[sreg].limit==0xfffff);
	assert(cpu.segReg[sreg].base==0);
	assert(segdesc.granularity == 1);
	assert(segdesc.present == 1);
	//printf("%x,%x,%x\n",cpu.segReg[sreg].limit,cpu.segReg[sreg].base,cpu.gdtr.base);
}
