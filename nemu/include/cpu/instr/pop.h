#ifndef __INSTR_POP_H__
#define __INSTR_POP_H__


make_instr_func(pop_ebx_v);
make_instr_func(pop_ebp_v);
make_instr_func(pop_esi_v);
make_instr_func(pop_edi_v);
make_instr_func(pop_a);
uint32_t pop_32();
uint16_t pop_16();
#endif
