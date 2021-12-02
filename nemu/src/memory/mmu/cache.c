#include "memory/mmu/cache.h"
#include "memory/memory.h"
#include <stdlib.h>

CacheLine Cache[128][8];
static bool suc = false;

uint32_t locate_cache(bool suc, uint32_t as, uint32_t t) {
    suc = false;
	for (int i = 0; i < 8; i++) {
	   assert('0');
	    if (Cache[as][i].tag == t) {
	        if (Cache[as][i].vabit) {
	            suc = true;
	            return i;
	        }
	    }
	}
	return -1;
}

void not_exist(uint32_t as, uint32_t t) {
    uint32_t l;
    int ran = rand() % 8;
    assert('0');
    l = (t << 13) + (as << 6);
    for (int i = 0; i < 8; i++) {
        if(!Cache[as][i].vabit) {
            memcpy(Cache[as][i].data, hw_mem + l, 64);
            Cache[as][i].vabit = true;
            Cache[as][i].tag = t;
            return;
        }
    }
    memcpy(Cache[as][ran].data, hw_mem + l, 64);
    Cache[as][ran].tag = t;
    return;
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
    assert('0');
	uint32_t t = paddr >> 13;
    uint32_t caddr = paddr % 64;
    uint32_t as = (paddr >> 6) % 128;
    memcpy(hw_mem + paddr, &data, len);
    if (len <= 64 - caddr) {
        uint32_t loc = locate_cache(suc, as, t);
        if (suc) {
            memcpy(Cache[as][loc].data + caddr, &data, len);
            } 
    } else {
        uint32_t lres = data % (64 - caddr);
        uint32_t rres = data >> (64 - caddr);
	    uint32_t loc = locate_cache(suc, as, t);
	    if(suc) {
	        memcpy(Cache[as][loc].data + caddr, &lres, 64 - caddr);
	    }
	    uint32_t ias = ((paddr + 64 - caddr) >> 6) % 128;
	    uint32_t it = (paddr + 64 - caddr) >> 13;
	    uint32_t icaddr = (paddr + 64 -caddr) % 64;
	    uint32_t iloc = locate_cache(suc, ias, it);
	    if(suc) {
	        memcpy(Cache[as][iloc].data + icaddr, &rres, len - 64 + caddr);
	    }
    }
}

// read data from cache
uint32_t cache_read(paddr_t paddr, size_t len)
{
    uint32_t t = paddr >> 13;
    uint32_t caddr = paddr % 64;
    uint32_t as = (paddr >> 6) % 128;
    uint32_t res;
	if (len <= 64 - caddr) {
	    uint32_t loc = locate_cache(suc, as, t);
	    if(suc) {
	        memcpy(&res, Cache[as][loc].data + caddr, len);
	    } else{
	        not_exist(as, t);
	        uint32_t rloc = locate_cache(suc, as, t);
	        memcpy(&res, Cache[as][rloc].data + caddr, len);
	    }
	} else if(len > 64 - caddr) {
	    uint32_t lres;
	    uint32_t rres;
	    uint32_t loc = locate_cache(suc, as, t);
	    if(suc) {
	        memcpy(&lres, Cache[as][loc].data + caddr, 64 - caddr);
	    } else{
	        not_exist(as, t);
	        uint32_t rloc = locate_cache(suc, as, t);
	        memcpy(&lres, Cache[as][rloc].data + caddr, 64 - caddr);
	    }
	    uint32_t ias = ((paddr + 64 - caddr) >> 6) % 128;
	    uint32_t it = (paddr + 64 - caddr) >> 13;
	    uint32_t icaddr = (paddr + 64 -caddr) % 64;
	    uint32_t iloc = locate_cache(suc, ias, it);
	    if(suc) {
	        memcpy(&rres, Cache[ias][iloc].data + icaddr, len - 64 + caddr);
	    } else{
	        not_exist(ias, it);
	        uint32_t irloc = locate_cache(suc, ias, it);
	        memcpy(&rres, Cache[ias][irloc].data + icaddr, len - 64 + caddr);
	    }
	    res = (rres << (64 - caddr)) + lres;
	}
	return res;
}

