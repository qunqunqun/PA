#include "cpu/instr.h"
#include <stdio.h>
#include "cpu/intr.h"
make_instr_func(int_ib)
{
	OPERAND imm;
	imm.data_size = 8;
	imm.sreg = SREG_CS;
	imm.addr = cpu.eip + 1;
	imm.type = OPR_IMM;

	operand_read(&imm);
	//printf("imm:%x\n",imm.val);
	raise_sw_intr(imm.val);
	print_asm_1("int","0x",1,&imm);

	return 0;
}
