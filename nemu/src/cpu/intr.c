#include "cpu/intr.h"
#include "cpu/instr.h"
#include "memory/memory.h"

void push_eflags()
{
	cpu.esp = cpu.esp - 4;
	OPERAND rmm;
	rmm.data_size = 32;
	rmm.sreg = SREG_SS;//ss duan
	rmm.addr = cpu.esp;
	rmm.type = OPR_MEM;
	rmm.val = cpu.eflags.val;
	operand_write(&rmm);
}

void push_cs()
{
	cpu.esp = cpu.esp - 4;
	OPERAND rmm;
	rmm.data_size = 32;
	rmm.sreg = SREG_SS;//ss duan
	rmm.addr = cpu.esp;
	rmm.type = OPR_MEM;
	rmm.val = cpu.cs.val;
	operand_write(&rmm);
}

void push_eip()
{
	cpu.esp = cpu.esp - 4;
	OPERAND rmm;
	rmm.data_size = 32;
	rmm.sreg = SREG_SS;//ss duan
	rmm.addr = cpu.esp;
	rmm.type = OPR_MEM;
	rmm.val = cpu.eip;
	operand_write(&rmm);
}

void raise_intr(uint8_t intr_no) {
#ifdef IA32_INTR
	//printf("intr_no:%x\n",intr_no);
	//push eflags cs rip
	push_eflags();
	push_cs();
	push_eip();
	//find the IDTR base
	//uint32_t limit = cpu.idtr.limit;
	uint32_t base = cpu.idtr.base;
	//printf("imm.limit:0x%x,imm.base:0x%x\n",limit,base);
	//find idt
	GateDesc gatedesc;
	uint32_t addr = base;
	addr = addr + sizeof(GateDesc) * intr_no;
	gatedesc.val[0] = laddr_read(addr,4);
	gatedesc.val[1] = laddr_read(addr+4,4);
	//printf("G1:%x,G2:%x\n",gatedesc.val[0],gatedesc.val[1]);
	assert(gatedesc.present == 1);
	//load cs
	cpu.cs.val = gatedesc.selector;
	//printf("cpu.cs.val:%x\n",cpu.cs.val);
	load_sreg(SREG_CS);
	//clear IF if interrupt
	//printf("stop3\n");
	if(gatedesc.type == 0xe)
	{
		cpu.eflags.IF = 0;
	}
	//find the eip
	//find the offset
	uint32_t offset = ((gatedesc.offset_31_16<<16)|(gatedesc.offset_15_0));
	cpu.eip = offset;
	//printf("cpu.eip:0x%x\n",cpu.eip);
	//printf("Please implement raise_intr()");
	//assert(0);
#endif
}

void raise_sw_intr(uint8_t intr_no) {
	// return address is the next instruction
	cpu.eip += 2;
	raise_intr(intr_no);
}
