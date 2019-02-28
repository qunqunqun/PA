#include "cpu/instr.h"
#include <stdio.h>

make_instr_func(pop_ebx_v)
{
	OPERAND rm;
	rm.data_size = data_size;
	rm.type = OPR_MEM;
	rm.addr = cpu.esp;
	rm.sreg = SREG_SS;
	operand_read(&rm);
	cpu.ebx = rm.val;
	cpu.esp += 4;
	return 1;
}
make_instr_func(pop_ebp_v)
{
	OPERAND rm;
	rm.data_size = data_size;
	rm.type = OPR_MEM;
	rm.sreg = SREG_SS;
	rm.addr = cpu.esp;
	operand_read(&rm);
	cpu.ebp = rm.val;
	cpu.esp += 4;
	return 1;
}
make_instr_func(pop_esi_v)
{
	OPERAND rm;
	rm.data_size = data_size;
	rm.type = OPR_MEM;
	rm.addr = cpu.esp;
	rm.sreg = SREG_SS;
	operand_read(&rm);
	cpu.esi = rm.val;
	cpu.esp += 4;
	return 1;
}
make_instr_func(pop_edi_v)
{
	OPERAND rm;
	rm.data_size = data_size;
	rm.type = OPR_MEM;
	rm.addr = cpu.esp;
	rm.sreg = SREG_SS;
	operand_read(&rm);
	cpu.edi = rm.val;
	cpu.esp += 4;
	return 1;
}

uint16_t pop_16()
{
	OPERAND rmm;
	rmm.type = OPR_MEM;
	rmm.addr = cpu.esp;
	rmm.sreg = SREG_SS;
	rmm.data_size = 16;
	
	operand_read(&rmm);
	cpu.esp += 4;
	return rmm.val;
}

uint32_t pop_32()
{
	OPERAND rmm;
	rmm.type = OPR_MEM;
	rmm.addr = cpu.esp;
	rmm.sreg = SREG_SS;
	rmm.data_size = 32;
	
	operand_read(&rmm);
	cpu.esp += 4;
	return rmm.val;
}

make_instr_func(pop_a)
{
	if(data_size == 16)
	{
		for(int i = 7; i >= 0;i--)
		{	
			if(i!=4)
			{
				cpu.gpr[i].val = pop_16();
			}
			else
			{
				cpu.esp += 4;
			}
		}
	}	
	else if(data_size == 32)
	{
		for(int i = 7; i >= 0;i--)
		{
			if(i!=4)
			{
				cpu.gpr[i].val = pop_32();
			}
			else
			{
				cpu.esp += 4;
			}
		}
	}
	return 1;
}
