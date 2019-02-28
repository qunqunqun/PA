#include "cpu/instr.h"

make_instr_func(leave)
{
	cpu.esp=cpu.ebp;//mov ebp esp

	OPERAND	rmm;
	rmm.data_size = data_size;
	rmm.type = OPR_MEM;
	rmm.addr =cpu.esp;
	rmm.sreg = SREG_SS;
	operand_read(&rmm);
	cpu.ebp=rmm.val;
	cpu.esp = cpu.esp+4;
	return 1;
}
