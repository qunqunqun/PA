#include "cpu/instr.h"
#include <stdio.h>
make_instr_func(ret_near)
{
	OPERAND	im;
	im.type = OPR_MEM;
	im.data_size = data_size;
	im.addr = cpu.esp;
	//im.sreg = SREG_SS;
	operand_read(&im);
	cpu.esp += data_size/8;
	cpu.eip = im.val;
	//printf("%x\n",im.val);
	return 0;
}
make_instr_func(ret_near_w)
{
	OPERAND rm;
	rm.type = OPR_IMM;
	rm.data_size = data_size;
	rm.addr = cpu.esp;
	operand_read(&rm);

	OPERAND imm;
	imm.type = OPR_IMM;
	imm.data_size = 16;
	imm.addr = eip + 1;
	operand_read(&imm);
	cpu.esp += 4 + imm.val;
	cpu.eip = rm.val;
	return 0;
}
make_instr_func(iret)
{
	cpu.eip = pop_32();
	cpu.cs.val = sign_ext(pop_16(),32);
	cpu.eflags.val = pop_32();
	return 0;
}
