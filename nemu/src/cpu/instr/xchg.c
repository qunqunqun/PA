#include "cpu/instr.h"

make_instr_func(xchg_r)
{
	OPERAND rm;
	rm.data_size = data_size;
	rm.type = OPR_REG;
	rm.addr = opcode & 0x7;
	operand_read(&rm);
	if(data_size == 32)
	{
		uint32_t temp = rm.val;
		rm.val = cpu.eax;
		cpu.eax = temp;
	}
	else if(data_size == 16)
	{
		uint16_t temp = rm.val;
		rm.val = cpu.gpr[0]._16;
		cpu.gpr[0]._16 = temp;
	}
	operand_write(&rm);
	return 1;
}





