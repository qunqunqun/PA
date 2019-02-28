#include "cpu/instr.h"
#include <stdio.h>
make_instr_func(jmp_near) {
        OPERAND rel;
        rel.type = OPR_IMM;
	rel.sreg = SREG_CS;
        rel.data_size = data_size;
        rel.addr = eip + 1;

        operand_read(&rel);

	int offset = sign_ext(rel.val, data_size);
	//print_asm_1("jmp", "", 1+data_size/8,&rel);
	cpu.eip += offset;

        return 1 + data_size / 8;
}
make_instr_func(jmp_short) {
	OPERAND rel;
	rel.type=OPR_IMM;
	rel.sreg=SREG_CS;
	rel.data_size=8;
	rel.addr=eip+1;

	operand_read(&rel);

	int offset = sign_ext(rel.val,8);
	//print_asm_1("jmp", "", 2,&rel);
	cpu.eip+=offset;

	return 2;
}
make_instr_func(jmp_far){
	OPERAND rm;
	rm.type = OPR_MEM;
	rm.sreg = SREG_CS;
	rm.data_size = data_size;
	//printf("jmpdata_size:%d\n",data_size);
	modrm_rm(eip+1,&rm);
	operand_read(&rm);
	//printf("rm%x\n",rm.val);
	if(data_size==16)
	{	
		cpu.eip = sign_ext( rm.val, 16);
	}
	else{
		cpu.eip = rm.val;
	}
	return 0;
}

make_instr_func(ljmp)
{
	OPERAND dest;
	dest.data_size = data_size;
	dest.type = OPR_IMM;
	dest.sreg = SREG_CS;
	dest.addr = eip + 1 ;
	operand_read(&dest);
	if(data_size==16)
	{
		cpu.eip = dest.val & 0xffff;
	}
	else
	{
		cpu.eip = dest.val;
	}
	OPERAND dest2;
	dest2.type = OPR_IMM;
	dest2.data_size = 16;
	dest2.addr = eip + 5;
	dest2.sreg = SREG_CS;
	operand_read(&dest2);
	cpu.cs.val = dest2.val;
	//printf("dest1:%x,dest2:%x\n",dest.val,dest2.val);
	load_sreg(SREG_CS);
	//cpu.eip = dest.val;
	return 0;
}
