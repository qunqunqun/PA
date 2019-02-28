#include "cpu/instr.h"
#include <stdio.h>
static void instr_execute_2op()
{
	operand_read(&opr_src);
	operand_read(&opr_dest);
	if((opr_src.data_size==8)&&(opr_dest.data_size==data_size))
	{
		if(((opr_src.val & 0x80)>>7)==1)
		{
			if(data_size == 16)
			{
				opr_src.val = (uint16_t) opr_src.val + 0xFF00;
			}
			else if(data_size == 32)
			{	
				opr_src.val = (uint32_t) opr_src.val + 0xFFFFFF00;	
			}
		}	
	}
	//printf("%x,%x\n",opr_src.val,opr_dest.val);
	opr_dest.val = alu_and(opr_src.val,opr_dest.val);
	//printf("%x\n",opr_dest.val);
	//printf("%x\n",cpu.esp);
	operand_write(&opr_dest);
	//printf("%x\n",cpu.esp);
}

make_instr_impl_2op(and,i,rm,bv)
make_instr_impl_2op(and,rm,r,b)
make_instr_impl_2op(and,r,rm,v)
make_instr_impl_2op(and,i,rm,v)
make_instr_impl_2op(and,i,rm,b)
make_instr_impl_2op(and,i,a,v)
make_instr_impl_2op(and,rm,r,v)
make_instr_impl_2op(and,i,a,b)

