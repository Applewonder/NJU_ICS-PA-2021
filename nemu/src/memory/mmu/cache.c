#include "memory/mmu/cache.h"
#include "memory/memory.h"
#include <stdlib.h>
#include <time.h>

CacheLine Cache[128][8];
static bool suc = false;

uint32_t locate_cache(uint32_t as, uint32_t t) {
    suc = false;
	for (int i = 0; i < 8; i++) {
	    if (Cache[as][i].tag == t) {
	        if (Cache[as][i].vabit) {
	            suc = true;
	            return i;
	        }
	    }
	}
	return -1;
}

int not_exist(uint32_t as, uint32_t t, paddr_t paddr, uint32_t caddr, size_t len) {
    uint32_t l;
    srand((unsigned)time(NULL));
    int ran = rand() % 8;
    l = (paddr / 64) << 6;
    for (int i = 0; i < 8; i++) {
        if(!Cache[as][i].vabit) {
            memcpy(Cache[as][i].data, hw_mem + l, 64);
            Cache[as][i].vabit = true;
            Cache[as][i].tag = t;
            return i;
        }
    }
    memcpy(Cache[as][ran].data, hw_mem + l, 64);
    Cache[as][ran].tag = t;
    return ran;
}

// init the cache
void init_cache()
{
    
	for (int i = 0; i < 128; i++) {
	    for (int j = 0; j < 8; j++) {
	        Cache[i][j].vabit = false;
	    }
	}
}

// write data to cache
void cache_write(paddr_t paddr, size_t len, uint32_t data)
{
    
	uint32_t t = paddr >> 13;
    uint32_t caddr = paddr % 64;
    uint32_t as = (paddr >> 6) % 128;
    memcpy(hw_mem + paddr, &data, len);
    if (len <= 64 - caddr) {
        uint32_t loc = locate_cache(as, t);
        if (suc) {
            memcpy(Cache[as][loc].data + caddr, &data, len);
            } 
    } else if(len > 64 - caddr){
        
        uint32_t rres = data >> (64 - caddr)*8;
        uint32_t lres = data  - (rres << (64 - caddr)*8);
	    uint32_t loc = locate_cache(as, t);
	    if(suc) {
	        memcpy(Cache[as][loc].data + caddr, &lres, 64 - caddr);
	    }
	    uint32_t ias = ((paddr + 64 - caddr) >> 6) % 128;
	    uint32_t it = (paddr + 64 - caddr) >> 13;
	    uint32_t icaddr = (paddr + 64 -caddr) % 64;
	    uint32_t iloc = locate_cache(ias, it);
	    if(suc) {
	        memcpy(Cache[ias][iloc].data + icaddr, &rres, len - 64 + caddr);
	    }
    }
}

// read data from cache
uint32_t cache_read(paddr_t paddr, size_t len)
{
    uint32_t t = paddr >> 13;
    uint32_t caddr = paddr % 64;
    uint32_t as = (paddr >> 6) % 128;
    uint32_t res = 0;
	if (len <= 64 - caddr) {
	    uint32_t loc = locate_cache(as, t);
	    if(suc) {
	        memcpy(&res, Cache[as][loc].data + caddr, len);
	    } else{
	        uint32_t rloc = not_exist(as, t, paddr, caddr, len);
	        memcpy(&res, Cache[as][rloc].data + caddr, len);
	    }
	} else if(len > 64 - caddr) {
	    uint32_t lres = 0;
	    uint32_t rres = 0;
	    uint32_t loc = locate_cache(as, t);
	    if(suc) {
	         memcpy(&lres, Cache[as][loc].data + caddr, 64 - caddr);
	    } else{
	         uint32_t rloc = not_exist(as, t, paddr, caddr, len);
	         memcpy(&lres, Cache[as][rloc].data + caddr, 64 - caddr);
	    }
	    uint32_t ias = ((paddr + 64 - caddr) >> 6) % 128;
	    uint32_t it = (paddr + 64 - caddr) >> 13;
	    uint32_t icaddr = (paddr + 64 -caddr) % 64;
	    uint32_t iloc = locate_cache(ias, it);
	    if(suc) {
	        memcpy(&rres, Cache[ias][iloc].data + icaddr, len - 64 + caddr);
	    } else{
	        uint32_t irloc = not_exist(ias, it, paddr + 64 - caddr, icaddr, len - 64 + caddr);
	        memcpy(&rres, Cache[ias][irloc].data + icaddr, len - 64 + caddr);
	    }
	    res = (rres << (64 - caddr)*8) + lres;
	}
	return res;
}
