#ifndef __FIND_SYMBOL_H__
#define __FIND_SYMBOL_H__

void load_elf_tables(char * exec_file);
uint32_t look_up_fun_symtab(char *sym,bool *success);
uint32_t look_up_symtab(char *sym,bool *success);

#endif
