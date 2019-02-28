#include "cpu/instr.h"
#include <stdio.h>

make_instr_func(lgdt)
{
	OPERAND rm;
	int len = modrm_rm( eip + 1 , &rm);
	rm.data_size = 16;
	operand_read(&rm);
	cpu.gdtr.limit = rm.val;

	rm.addr += 2;
	rm.data_size = 32;
	operand_read(&rm);
	cpu.gdtr.base = rm.val;
	//printf("limit:%x,base:%x,mem.val:%x\n",cpu.gdtr.limit,cpu.gdtr.base,mem.val);
	return len+1;
}

