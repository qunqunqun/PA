#include "cpu/cpu.h"
#include <stdlib.h>
uint32_t alu_add(uint32_t src, uint32_t dest) {
	uint32_t temp=dest;
	dest=dest+src;
	cpu.eflags.ZF=(dest==0);
	cpu.eflags.SF=((dest>>31)==1);
	cpu.eflags.CF=((dest<src)||(dest<temp));
//	printf("jinwei%d\n",cpu.eflags.CF);
	cpu.eflags.OF=((((src>>31)==0)&&(temp>>31==0)&&(dest>>31==1))||((src>>31==1)&&(temp>>31==1)&&(dest>>31==0)));
	int count=0;
	uint32_t pf=(dest&(0xff));
	while(pf!=0)
	{
	  count+=(pf&0x1);
	  pf=pf>>1;
	}
	cpu.eflags.PF=((count%2)==0);
	//assert(0);
	return dest;
}

uint32_t alu_adc(uint32_t src, uint32_t dest) {
	uint64_t result2=(uint64_t)src+(uint64_t)dest;
	//int cf=((result<src)||(result<dest));
	result2=result2+cpu.eflags.CF;
	//cpu.eflags.CF=cf;
	//printf("%d\n",cpu.eflags.CF);
	cpu.eflags.CF=((result2>>32)!=0);
	uint32_t result=result2;
	cpu.eflags.ZF=(result==0);
	cpu.eflags.SF=(result>>31==1);
	cpu.eflags.OF=(((src>>31==0)&&(dest>>31==0)&&(result>>31==1))||((src>>31==1)&&(dest>>31==1)&&(result>>31==0)));
	int count=0;
	uint32_t pf=(result&0xff);
	while(pf!=0)
	{
		count+=(pf&0x1);
		pf=pf>>1;
	}
	cpu.eflags.PF=((count%2)==0);
	return result;
}


uint32_t alu_sub(uint32_t src, uint32_t dest) {
	uint32_t result=dest-src;
	cpu.eflags.ZF=(result==0);
	cpu.eflags.SF=(result>>31==1);
	cpu.eflags.CF=(dest<src);
	//cpu.eflags.OF=(((src>>31==1)&&(dest>>31==0)&&(result>>31==1))||((src>>31==0)&&(dest>>31==1)&&(result>>31==0)));
	uint32_t temp=dest+1+(~src);
	uint32_t x_neg=src>>31;
	uint32_t y_neg=dest>>31;
	uint32_t d_neg=temp>>31;
	cpu.eflags.OF=(~(y_neg^~x_neg)&(y_neg^d_neg));
	int count=0;
	uint32_t pf=(result&0xff);
	while(pf!=0)
	{
		count+=(pf&0x1);
		pf=pf>>1;
	}	
	cpu.eflags.PF=(count%2==0);
	//printf("%d%d%d\n",cpu.eflags.ZF,cpu.eflags.CF,cpu.eflags.OF);
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//assert(0);
	return result;
}

uint32_t alu_sbb(uint32_t src, uint32_t dest) {
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	uint32_t result=dest-src;
	result=result-cpu.eflags.CF;
	cpu.eflags.ZF=(result==0);
	cpu.eflags.SF=(result>>31==1);
	uint32_t temp=dest+~(src+cpu.eflags.CF)+1;
	uint32_t x_neg=src>>31;
	uint32_t y_neg=dest>>31;
	uint32_t d_neg=temp>>31;
	cpu.eflags.OF=(~(y_neg^~x_neg)&(y_neg^d_neg));

	//cpu.eflags.OF=(((src>>31==0)&&(dest>>31==1)&&(result>>31==0))||((src>>31==1)&&(dest>>31==0)&&(result>>31==1)));
	//uint32_t temp=dest-cpu.eflags.CF;
	//int CF=(((temp>>31==0)&&(dest>>31==1))||((dest>>31==0)&&(temp>>31==1)));
	cpu.eflags.CF=((dest<cpu.eflags.CF)||(dest<src)||(dest-cpu.eflags.CF<src)||(dest-src<cpu.eflags.CF));
	int count=0;
	uint32_t pf=(result&0xff);
	while(pf!=0)
	{
		count+=(pf&0x1);
		pf=pf>>1;
	}
	cpu.eflags.PF=(count%2==0);
	//assert(0);
	return result;
}


uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size) {
	uint64_t result=(uint64_t)src*(uint64_t)dest;
	cpu.eflags.CF = ((result>>data_size)!=0);
	cpu.eflags.OF = ((result>>data_size)!=0);
	return result;
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size) {
	uint64_t result=(uint64_t)dest*(uint64_t)src;
	cpu.eflags.CF=((result>>data_size)!=0);
	cpu.eflags.OF=((result>>data_size)!=0);
	return result;
}

uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size) {
	if(src==0) exit(0);
	uint32_t result=dest/src;
	return result;
}

int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size) {
	if(src==0) exit(0);
	uint32_t result=dest/src;
	return result;
}

uint32_t alu_mod(uint64_t src, uint64_t dest) {
	if(src==0) exit(0);
	uint32_t result=dest%src;
	return result;
}

int32_t alu_imod(int64_t src, int64_t dest) {
	if(src==0) exit(0);
	uint32_t result=dest%src;
	return result;
}

uint32_t alu_and(uint32_t src, uint32_t dest) {
	uint32_t result=(src&dest);
	cpu.eflags.ZF=(result==0);
	cpu.eflags.SF=(result>>31==1);
	cpu.eflags.OF=0;
	cpu.eflags.CF=0;
	int count=0;
	uint32_t pf=(result&0xff);
	while(pf!=0)
	{
		count+=(pf&0x1);
		pf=pf>>1;
	}
	cpu.eflags.PF=(count%2==0);
	return result;
}

uint32_t alu_xor(uint32_t src, uint32_t dest) {
	uint32_t result=(src^dest);
	//printf("%x,%x,%x\n",src,dest,result);
	cpu.eflags.ZF=(result==0);
	cpu.eflags.SF=(result>>31==1);
	cpu.eflags.CF=0;
	cpu.eflags.OF=0;
	int count=0;
	uint32_t pf=(result&0xff);
	while(pf!=0)
	{
		count+=(pf&0x1);
		pf=pf>>1;
	}
	cpu.eflags.PF=(count%2==0);
	return result;
}

uint32_t alu_or(uint32_t src, uint32_t dest) {
	uint32_t result=(src|dest);
	//printf("%d,%d\n",src,dest);
	cpu.eflags.CF=0;
	cpu.eflags.OF=0;
	cpu.eflags.ZF=(result==0);
	//printf("%d\n",cpu.eflags.ZF);
	cpu.eflags.SF=(result>>31==1);
	int count=0;
	uint32_t pf=(result&0xff);
	while(pf!=0)
	{
		count+=(pf&0x1);
		pf=pf>>1;
	}
	cpu.eflags.PF=(count%2==0);
	return result;
}

uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size) {
	uint32_t o=0;
	uint32_t low=0;
	uint32_t high=0;
	if(data_size!=32)
	{
		high=((dest>>data_size)<<data_size);
	}
	for(int i=0;i<data_size;i++)
	{
		o=o<<1;
		o+=0x1;
	}
	low=o&dest;
	uint32_t flag1=((low>>(data_size-src))&0x1);
	uint32_t flag2=(low>>(data_size-1));
	low=low<<src;
	low=low&o;
	uint32_t flag3=(low>>(data_size-1));
	cpu.eflags.ZF=(low==0);
	cpu.eflags.SF=(low>>(data_size-1)==1);
	cpu.eflags.OF=(flag2==flag3);
	cpu.eflags.CF=(flag1!=0);
	int count=0;
	uint32_t pf=(low&0xff);
	while(pf!=0)
	{
		count+=(pf&0x1);
		pf=pf>>1;
	}
	cpu.eflags.PF=(count%2==0);
	uint32_t result=high|low;
	return result;
}

uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size) {
	uint32_t high=0;
	uint32_t low=0;
	uint32_t o=0;
	for(int i=0;i<data_size;i++)
	{
		o=o<<1;
		o+=0x1;
	}
	low=dest&o;
	high=dest&(~o);
	uint32_t flag1=(low>>(data_size-1));
	uint32_t flag2=(low>>(src-1)&0x1);
	low=low>>src;
	cpu.eflags.ZF=(low==0);
	cpu.eflags.SF=(low>>(data_size-1)==1);
	cpu.eflags.CF=(flag2!=0);
	cpu.eflags.OF=(flag1!=(low>>(data_size-1)));
	int count=0;
	uint32_t pf=(low&0xff);
	while(pf!=0)
	{
		count+=(pf&0x1);
		pf=pf>>1;
	}
	cpu.eflags.PF=(count%2==0);
	uint32_t result=high|low;
	return result;
	
}

uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size) {
	uint32_t high=0;
	uint32_t low=0;
	uint32_t o=0;
	for(int i=0;i<data_size;i++)
	{
		o=o<<1;
		o=o+0x1;
	}
	low=dest&o;
	high=dest&(~o);
	uint32_t flag1=(low>>(src-1)&0x1);
	uint32_t flag2=(low>>(data_size-1));
	uint32_t temp=0;
	if(flag2==1)
	{for(int i=0;i<(data_size-src);i++)
	{
		temp=temp<<1;
		temp+=0x1;
	}
	temp=(~temp)&o;
	}
	low=(low>>src);
	low=(low|temp);
	cpu.eflags.ZF=(low==0);
	cpu.eflags.SF=(low>>(data_size-1)==1);
	cpu.eflags.OF=(flag2!=(low>>(data_size-1)));
	cpu.eflags.CF=(flag1!=0);
	int count=0;
	uint32_t pf=(low&0xff);
	while(pf!=0)
	{
		count+=(pf&0x1);
		pf=pf>>1;
	}
	cpu.eflags.PF=(count%2==0);
	uint32_t result=high|low;
	return result;
}

uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size) {
	uint32_t high=0;
	uint32_t low=0;
	uint32_t o=0;
	for(int i=0;i<data_size;i++)
	{
		o=o<<1;
		o+=0x1;
	}
	low=dest&o;
	high=dest&(~o);
	uint32_t flag1=(low>>(data_size-1));
	uint32_t flag2=((low>>(data_size-src))&0x1);
	low=low<<src;
	low=low&o;
	cpu.eflags.ZF=(low==0);
	cpu.eflags.SF=(low>>(data_size-1));
	cpu.eflags.CF=(flag2!=0);
	cpu.eflags.OF=(flag1!=(low>>(data_size-1)));
	int count=0;
	uint32_t pf=(low&0xff);
	while(pf!=0)
	{
		count+=(pf&0x1);
		pf=pf>>1;
	}
	cpu.eflags.PF=(count%2==0);
	uint32_t result=high|low;
	return result;
}
