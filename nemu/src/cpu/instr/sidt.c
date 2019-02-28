#include "cpu/instr.h"
#include <stdio.h>
make_instr_func(lidt)
{
	OPERAND mem;
	int len = modrm_rm( eip + 1 , &mem);
	//printf("addr:%x\n",mem.addr);
	cpu.idtr.limit = laddr_read(mem.addr, 2);
	cpu.idtr.base = laddr_read(mem.addr + 2, 4);
	//printf("limit:%x,base:%x,mem.val:%x\n",cpu.idtr.limit,cpu.gdtr.base,mem.val);	
	return len + 1;
}

