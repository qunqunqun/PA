#include"cpu/instr.h"

make_instr_func(cli)
{
	cpu.eflags.IF = 0;//clear interrupt Flag
	return 1;
}






