#include"cpu/instr.h"
#include<stdio.h>
static void instr_execute_2op()
{
	operand_read(&opr_src);
	operand_read(&opr_dest);
	//opr_src.val = sign_ext(opr_src.val,data_size);
	//opr_dest.val = sign_ext(opr_dest.val,data_size);
	if((opr_src.data_size==8)&&(opr_dest.data_size==data_size))
	{
		if(((opr_src.val & 0x80)>>7)==1)
		{/*{
			uint32_t temp=0;
			for(int i=0; i<data_size-8;i++)
			{
				temp=temp<<1;
				temp+=1;
			}
			temp=temp<<8;
			opr_src.val = opr_src.val|temp;
		}*/
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
	alu_sub(opr_src.val,opr_dest.val);
	//printf("%d,%d\n",cpu.eflags.ZF,cpu.eflags.CF);
}

make_instr_impl_2op(cmp,i,rm,bv)
make_instr_impl_2op(cmp,r,rm,v)
make_instr_impl_2op(cmp,i,rm,v)
make_instr_impl_2op(cmp,rm,r,v)
make_instr_impl_2op(cmp,i,a,b)
make_instr_impl_2op(cmp,i,rm,b)
make_instr_impl_2op(cmp,rm,r,b)
make_instr_impl_2op(cmp,i,a,v)
make_instr_impl_2op(cmp,r,rm,b)




