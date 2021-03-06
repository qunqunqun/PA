#include "cpu/instr.h"
#include <stdio.h>
static void instr_execute_2op()
{
	operand_read(&opr_src);
	operand_read(&opr_dest);
	uint32_t src = sign_ext(opr_src.val,data_size);
	uint32_t dest = sign_ext(opr_dest.val,data_size);
	uint32_t res = alu_and(src,dest);
	cpu.eflags.OF=0;
	cpu.eflags.CF=0;
	//printf("%d\n",cpu.eflags.ZF);
	if(opr_src.data_size == 8 || opr_dest.data_size ==8)
	{
		cpu.eflags.SF = (res & 0x80) ? 1 : 0;
	}
}

make_instr_impl_2op(test,r,rm,v)
make_instr_impl_2op(test,r,rm,b)
make_instr_impl_2op(test,i,rm,v)
make_instr_impl_2op(test,i,a,v)
make_instr_impl_2op(test,i,a,b)
make_instr_impl_2op(test,i,rm,b)
