#include "cpu/instr.h"
#include <stdio.h>

make_instr_func(decv)
{
	int len=1;
	OPERAND rm;
	rm.data_size=data_size;
	len += modrm_rm( eip+1 , &rm);
	operand_read(&rm);
	rm.val = alu_sub(1,rm.val);
	operand_write(&rm);
	return len;
}


make_instr_func(dec_eax)
{
	cpu.eax = alu_sub(1,cpu.eax);
	return 1;
}
make_instr_func(dec_ecx)
{
	cpu.ecx = alu_sub(1,cpu.ecx);
	return 1;
}
make_instr_func(dec_edx)
{
	cpu.edx = alu_sub(1,cpu.edx);
	return 1;
}

