#include "memory/mmu/cache.h"
#include "memory/memory.h"
#include <stdlib.h>
#include <time.h>

CacheLine Cache[128][8];
static bool suc = false;

static uint32_t hw_mem_read(paddr_t paddr, size_t len)
{
	uint32_t ret = 0;
	memcpy(&ret, hw_mem + paddr, len);
	return ret;
}//调试用

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

int not_exist(uint32_t as, uint32_t t, paddr_t paddr) {
    uint32_t l;
    srand((unsigned)time(NULL));
    int ran = rand() % 8;
    l = (paddr / 64) << 6;
    for (int i = 0; i < 8; i++) {
        if(!Cache[as][i].vabit) {
            memcpy(Cache[as][i].data, hw_mem + l, 64);
            Cache[as][i].vabit = true;
            Cache[as][i].tag = t;
            //printf("%d", i);
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
    //printf("%d", len);
    if (len <= 64 - caddr) {
        uint32_t loc = locate_cache(as, t);
        if (suc) {
            memcpy(Cache[as][loc].data + caddr, &data, len);
            } 
    } else if(len > 64 - caddr){
        
        uint32_t lres = data % (64 - caddr);
        uint32_t rres = data >> (64 - caddr);
	    uint32_t loc = locate_cache(as, t);
	    
	    if(suc) {
	        memcpy(Cache[as][loc].data + caddr, &lres, 64 - caddr);
	    }
	    
	    uint32_t ias = ((paddr + 64 - caddr) >> 6) % 128;
	    uint32_t it = (paddr + 64 - caddr) >> 13;
	    uint32_t icaddr = (paddr + 64 -caddr) % 64;
	    //printf("ias = %ud, it = %ud", ias, it);
	    uint32_t iloc = locate_cache(ias, it);
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
	    uint32_t loc = locate_cache(as, t);
	    if(suc) {
	        memcpy(&res, Cache[as][loc].data + caddr, len);
	        //assert(res == hw_mem_read(paddr, len));
	    } else{
	        uint32_t rloc = not_exist(as, t, paddr);
	        //printf("%d\n", rloc);
	        memcpy(&res, Cache[as][rloc].data + caddr, len);
	        //printf("%d\n", res);
	        //printf("%d\n", hw_mem_read(paddr, len));
	        assert(res == hw_mem_read(paddr, len));
	    }
	} else if(len > 64 - caddr) {
	    uint32_t lres;
	    uint32_t rres;
	    uint32_t loc = locate_cache(as, t);
	    if(suc) {
	        memcpy(&lres, Cache[as][loc].data + caddr, 64 - caddr);
	    } else{
	         uint32_t rloc = not_exist(as, t, paddr);
	         memcpy(&lres, Cache[as][rloc].data + caddr, 64 - caddr);
	    }
	    uint32_t ias = ((paddr + 64 - caddr) >> 6) % 128;
	    uint32_t it = (paddr + 64 - caddr) >> 13;
	    uint32_t icaddr = (paddr + 64 -caddr) % 64;
	    uint32_t iloc = locate_cache(ias, it);
	    if(suc) {
	        memcpy(&rres, Cache[ias][iloc].data + icaddr, len - 64 + caddr);
	    } else{
	        uint32_t irloc =not_exist(ias, it, paddr + 64 - caddr);
	        memcpy(&rres, Cache[ias][irloc].data + icaddr, len - 64 + caddr);
	    }
	    res = (rres << (64 - caddr)) + lres;
	    //assert(res == hw_mem_read(paddr, len));
	}
	return res;
}

