#include "cpu/instr.h"
#include <stdio.h>

make_instr_func(not_rm_v)
{
	int len = 1;
	OPERAND rm;
	rm.data_size = data_size;
	len += modrm_rm(eip+1,&rm);
	operand_read(&rm);
	rm.val = ~rm.val;
	operand_write(&rm);
	return len;
}
