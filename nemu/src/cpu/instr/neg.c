#include "cpu/instr.h"

make_instr_func(neg_v)
{
	OPERAND r;
	r.data_size = data_size;
	int len = modrm_rm(cpu.eip+1, &r);
	operand_read(&r);
	if(r.val == 0)
	{
		cpu.eflags.CF = 0;
	}
	else
	{
		cpu.eflags.CF = 1;
	}
	r.val = -r.val;
	operand_write(&r);
	return 1 + len;
}



