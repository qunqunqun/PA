#include "cpu/instr.h"
#include<stdio.h>
make_instr_func(call_near)
{
	//printf("%x\n",eip);
	OPERAND im;
	im.type = OPR_MEM;
	im.data_size = data_size;
	im.val = cpu.eip + data_size/8 + 1;
	cpu.esp = cpu.esp - 4;
	im.addr = cpu.esp;
	im.sreg = SREG_SS;
	operand_write(&im);
	
	OPERAND imm;
	imm.type = OPR_IMM;
	imm.data_size = data_size;
	imm.addr = eip+1;
	imm.sreg = SREG_CS;
	operand_read(&imm);
	int offset = sign_ext(imm.val,data_size);
	cpu.eip += offset;
	//printf("%x\n",imm.val);
	return  data_size/8 + 1;
}
make_instr_func(call_mem)
{
	OPERAND mem;
	mem.type = OPR_MEM;
	//printf("%d\n",520);
	//mem.sreg = SREG_CS;
	mem.data_size = data_size;
	int len = 1;
	len += modrm_rm(eip + 1,&mem);
	operand_read(&mem);
	//push
	OPERAND src;
	src.data_size = data_size;
	src.type = OPR_MEM;
	src.sreg = SREG_SS;
	cpu.esp = cpu.esp - 4;
	src.addr = cpu.esp;
	src.val = cpu.eip + len;
	operand_write(&src);
	//printf("%x\n",mem.val);
	if(data_size==16)
	{
		cpu.eip = cpu.eip>>16;
		cpu.eip = cpu.eip<<16;
		cpu.eip += mem.val;
		cpu.eip = cpu.eip &0x0000FFFF;
	}
	else
	{
		cpu.eip = mem.val;
	}
	return 0;
	
}
