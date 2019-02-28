#include "cpu/instr.h"
#include <stdio.h>

make_instr_func(incv)
{
	int len=1;
	OPERAND rm;
	rm.data_size=data_size;
	len += modrm_rm( eip+1 , &rm);
	operand_read(&rm);
	rm.val = alu_add(1,rm.val);
	operand_write(&rm);
	return len;
}
make_instr_func(inc_edx)
{
	cpu.edx = alu_add(1,cpu.edx);
	return 1;
}
make_instr_func(inc_eax)
{
	cpu.eax = alu_add(1,cpu.eax);
	return 1;
}
make_instr_func(inc_edi)
{
	cpu.edi = alu_add(1,cpu.edi);
	return 1;
}
make_instr_func(inc_esi)
{
	cpu.esi = alu_add(1,cpu.esi);
	return 1;
}



