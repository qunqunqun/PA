#include "cpu/instr.h"
#include "device/port_io.h"


make_instr_func(out_a2d_b)
{
	pio_write( cpu.gpr[2]._16 , 1 , cpu.gpr[0]._8[0]);
	return 1;
}
