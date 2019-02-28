#include "cpu/cpu.h"
#include "memory/memory.h"
#include <stdio.h>
#include <memory.h>
#include "memory/cache.h"
#include "nemu.h"

// translate from linear address to physical address
paddr_t page_translate(laddr_t laddr) {
#ifndef TLB_ENABLED
	//printf("\nPlease implement page_translate()\n");
	//assert(0);
	//bu chong
	//printf("cpu.eip:%x\n",cpu.eip);
	uint32_t dir = (laddr >> 22); //high 10
	uint32_t page = ((laddr << 10) >> 22);//mid 10
	uint32_t offset = ((laddr <<20) >> 20);//low 12
	//printf("dir:%x,page:%x,offset:%x\n",dir,page,offset);
	uint32_t pdbr = cpu.cr3.PDBR;
	//printf("PDBR:%x\n",pdbr);
	uint32_t mulu = (dir * 4) + (pdbr << 12);
	PDE pde;
	pde.val = paddr_read(mulu,4);
	if(pde.present !=1)
	{
		printf("mulu:%x\n,cpu.eip=%x\n",mulu,cpu.eip);
	}
	assert(pde.present == 1);

	//printf("PDE:%x\n",pde.val);
	uint32_t base = (pde.val & 0xfffff000) + page * 4;
	//printf("base:%x\n",base);
	PTE pte;
	pte.val = paddr_read( base , 4 );

	assert(pte.present == 1);

//	printf("PTE:%x\n",pte.val);
	paddr_t paddr  = ((pte.val & 0xfffff000) | offset);
	//printf("paddr:%x\n",paddr);
	return paddr;
#else	
	return tlb_read(laddr) | (laddr & PAGE_MASK);
#endif
}
