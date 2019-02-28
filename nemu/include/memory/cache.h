#define __CACHE_H__


#include "memory/memory.h"
#define BLOCK_SIZE 64
#define CACHE_LINE 1024


typedef struct
{
	bool valid;//vaild wei 
	uint32_t tag;//tag wei
	uint8_t data[BLOCK_SIZE];
} CacheLine;

CacheLine cache[CACHE_LINE];

void init_cache();
uint32_t cache_read(paddr_t paddr,size_t len,CacheLine *cache);
void cache_write(paddr_t paddr,size_t len,uint32_t data,CacheLine *cache);

