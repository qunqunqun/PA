#include "memory/memory.h"
#include "memory/cache.h"
#include "cpu/cpu.h"
#include <memory.h>
#include <stdlib.h>
#include <time.h>

void init_cache()
{
	//printf("initialize it!\n");
	for(int i=0; i<CACHE_LINE; i++)
	{
		cache[i].valid=false;
		cache[i].tag=0;
		memset(cache[i].data,0,BLOCK_SIZE);
	}
}

uint32_t cache_read(paddr_t paddr,size_t len,CacheLine *cache)
{
	//printf("cpu.eip%x\n",cpu.eip);
	bool hit_it = false;
	uint32_t group=((paddr>>6)&0x7f)*8;
	//printf("%x\n",group);
	uint32_t baddr=paddr&0x3f;
	int i=0;
	for(i=group;i<group+8;i++)
	{
		uint32_t temp_tag = paddr>>13;
		if((cache[i].tag == temp_tag)&&cache[i].valid==true)
		{
			hit_it = true;
			break;
		}
	}
	if(hit_it==true)
	{	
	//	printf("hit!\n");
		uint32_t ret_addr=0;
		if(baddr+len > BLOCK_SIZE)
			memcpy(&ret_addr,hw_mem+paddr,len);//chao guo kuai de nei cun
		else
			memcpy(&ret_addr,cache[i].data+baddr,len);
		return ret_addr;
	}
	else
	{
	//	printf("not hit!\n");
		int insert = 0;
		for(insert = group;insert < group + 8;insert++)
		{
			if(cache[insert].valid==false)
			{
			//	printf("%d\n",insert);
				break;
			}
		}
		if(insert==group+8)
		{
			srand((unsigned)time(NULL));
			insert = group + (rand()%8);
		}
		cache[insert].tag=paddr>>13;
		cache[insert].valid=true;
		paddr_t temp = (paddr>>6)<<6;
		memcpy(cache[insert].data,hw_mem+temp,BLOCK_SIZE);
		//uint32_t ret = 0;
		//memcpy(&ret,hw_mem+paddr,len);
		return cache_read(paddr,len,cache);
	}
}

void cache_write(paddr_t paddr,size_t len, uint32_t data,CacheLine *cache)
{
//	printf("cpu.eip%x\n",cpu.eip);
	bool hit_it=false;
	uint32_t group = ((paddr>>6)&0x7f)*8;
	uint32_t baddr= paddr&0x3f;
	int i=0;
	for(i = group; i < group+8; i++)
	{
		uint32_t temp_tag=paddr>>13;
		if(cache[i].tag == temp_tag)
		{
			if(cache[i].valid == true)
			{
				hit_it=true;
				break;
			}
		}
	}
	if(hit_it)
	{
		//printf("w hit!!\n");
		if(baddr + len > BLOCK_SIZE)
		{
			memcpy(hw_mem + paddr,&data,len);
			cache[i].valid = false;
			paddr_t addr2 = paddr + len;
			uint32_t group_2 = ((addr2 >> 6)&0x7f)*8;
			uint32_t tag_2 = addr2>>13;
			for(int k = group_2 ; k < group_2+8;k++)
			{
				if(cache[k].tag == tag_2)
				{
					if(cache[k].valid == true)
					{
						cache[k].valid = false;
					}
				}
			}
			return;
		}
		else{	
			memcpy(hw_mem + paddr ,&data,len);
			uint32_t temp = (paddr>>6)<<6;
			memcpy(cache[i].data, hw_mem + temp ,BLOCK_SIZE);
		}
	}
	else
	{
		//printf("w not hit!\n");
		memcpy(hw_mem+paddr,&data,len);
	}
}
