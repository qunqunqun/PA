#include "cpu/instr.h"
#include <stdio.h>
make_instr_func(lea){
	int len=1;
	OPERAND src;
	OPERAND dest;
	src.data_size=data_size;
	dest.data_size=data_size;
	len = len + modrm_r_rm(eip+1,&dest,&src);
	dest.val = src.addr;
	operand_write(&dest);
	//printf("%d\n",len);
	return len;
}
