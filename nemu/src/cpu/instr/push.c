#include "cpu/instr.h"
#include <stdio.h>
make_instr_func(push_edi_v)
{
	OPERAND rmm;
	rmm.data_size = data_size;
	rmm.type = OPR_MEM;
	rmm.sreg = SREG_SS;
	rmm.val = cpu.edi;
	cpu.esp = cpu.esp-data_size/8;
	rmm.addr = cpu.esp;
	operand_write(&rmm);
	return 1;
}
make_instr_func(push_ecx_v)
{
	OPERAND rmm;
	rmm.data_size = data_size;
	rmm.type = OPR_MEM;
	rmm.val = cpu.ecx;
	rmm.sreg = SREG_SS;
	cpu.esp = cpu.esp-data_size/8;
	rmm.addr = cpu.esp;
	operand_write(&rmm);
	return 1;
}

make_instr_func(push_esi_v)
{
	OPERAND rmm;
	rmm.data_size = data_size;
	rmm.type = OPR_MEM;
	rmm.val = cpu.esi;
	cpu.esp = cpu.esp-data_size/8;
	rmm.addr = cpu.esp;
	operand_write(&rmm);
	return 1;
}
make_instr_func(push_ebp_v)
{
	OPERAND rmm;
	rmm.data_size = data_size;
	rmm.type = OPR_MEM;
	rmm.val = cpu.ebp;
	rmm.sreg = SREG_SS;
	cpu.esp = cpu.esp-data_size/8;
	rmm.addr = cpu.esp;
	operand_write(&rmm);
	return 1;
}
make_instr_func(push_ebx_v)
{
	OPERAND rmm;
	rmm.data_size=data_size;
	rmm.type=OPR_MEM;
	rmm.val=cpu.ebx;
	rmm.sreg = SREG_SS;
	cpu.esp = cpu.esp-data_size/8;
	rmm.addr = cpu.esp;
	operand_write(&rmm);
	return 1;
}
make_instr_func(push_edx_v)
{
	OPERAND rmm;
	rmm.data_size=data_size;
	rmm.type=OPR_MEM;
	rmm.val=cpu.edx;
	rmm.sreg = SREG_SS;
	cpu.esp = cpu.esp-data_size/8;
	rmm.addr = cpu.esp;
	operand_write(&rmm);
	return 1;
}
make_instr_func(push_eax_v)
{
	OPERAND rmm;
	rmm.data_size=data_size;
	rmm.type=OPR_MEM;
	rmm.val=cpu.eax;
	rmm.sreg = SREG_SS;	
	cpu.esp = cpu.esp-data_size/8;
	rmm.addr = cpu.esp;
	operand_write(&rmm);
	return 1;
}
make_instr_func(push_ev)
{
	int len=1;
	OPERAND rmm;
	rmm.type=OPR_MEM;
	rmm.sreg = SREG_SS;
	rmm.data_size=data_size;
	OPERAND src;
	src.data_size=data_size;
	len += modrm_rm(eip+1,&src);
	operand_read(&src);
	//printf("%x\n",cpu.ecx);
	//printf("%x,%x\n",src.addr,src.val);
	rmm.val=src.val;
	cpu.esp = cpu.esp-data_size/8;
	rmm.addr = cpu.esp;
	operand_write(&rmm);
	//printf("%d\nlen",len);
	return len;
}
make_instr_func(push_ib)
{
	//printf("%d\n",data_size);
	OPERAND imm;
	imm.data_size = 8;
	imm.type=OPR_MEM;
	imm.addr = eip+1;
	operand_read(&imm);
	//printf("%x\n",imm.val);
	imm.data_size=data_size;
	if((imm.val>>7)==1)
	{
		uint32_t temp=0;
		for(int i=0;i<data_size-8;i++)
		{
			temp=temp<<1;
			temp += 1;
		}
		temp=temp<<8;
		imm.val = (imm.val)|(temp);
	}
	//printf("%x\n",imm.val);
	cpu.esp = cpu.esp - data_size/8;
	imm.addr = cpu.esp;
	imm.sreg = SREG_SS;
	operand_write(&imm);
	return 2;
}
make_instr_func(push_iv)
{
	//printf("%d\n",data_size);
	OPERAND imm;
	imm.data_size = data_size;
	imm.type=OPR_MEM;
	imm.addr = eip+1;
	operand_read(&imm);
	//printf("%x\n",imm.val);
	imm.data_size=data_size;
	//printf("%x\n",imm.val);
	cpu.esp = cpu.esp - data_size/8;
	imm.addr = cpu.esp;
	imm.sreg = SREG_SS;
	operand_write(&imm);
	return 1+data_size/8;
}
make_instr_func(push_esp_v)
{
	OPERAND rmm;
	rmm.data_size=data_size;
	rmm.type=OPR_MEM;
	rmm.val=cpu.esp;
	rmm.sreg = SREG_SS;
	cpu.esp = cpu.esp-data_size/8;
	rmm.addr = cpu.esp;
	operand_write(&rmm);
	return 1;
}
void push_32(uint32_t temp)
{
	cpu.esp = cpu.esp-4;
	OPERAND rmm;
	rmm.type = OPR_MEM;
	rmm.sreg = SREG_SS;
	rmm.data_size = 32;
	rmm.addr = cpu.esp;
	rmm.val = temp;
	operand_write(&rmm);
}
void push_16(uint16_t temp)
{
	cpu.esp = cpu.esp-4;
	OPERAND rmm;
	rmm.type = OPR_MEM;
	rmm.sreg = SREG_SS;
	rmm.data_size =	16;
	rmm.addr = cpu.esp;
	rmm.val = temp;
	operand_write(&rmm);
}

make_instr_func(push_a)
{
	uint32_t addr = cpu.esp;
	if(data_size == 16)
	{
		addr = addr & 0xffff;
		for(int i=0;i<8;i++)
		{
			if(i!=4)
			{
				push_16(cpu.gpr[i]._16);
			}
			else
			{
				push_16(addr);
			}
		}
	}
	else if(data_size ==32)
	{
		for(int i=0;i<8;i++)
		{
			if(i!=4)
			{
				push_32(cpu.gpr[i].val);
			}
			else
			{	
				push_32(addr);
			}
		}
	}
	return 1;
}

