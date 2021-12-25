#include "cpu/cpu.h"
#include "memory/memory.h"

// translate from linear address to physical address
paddr_t page_translate(laddr_t laddr)
{
 #ifndef TLB_ENABLED
// 	uint32_t dir = laddr >> 22;
// 	uint32_t page = (laddr >> 12) % 1024;
// 	uint32_t offset = laddr % 4096;
// 	uint32_t m = (cpu.cr3.pdbr << 12) + 4*dir;
// 	PDE t;
// // 	printf("laddr = -------->%x\n", laddr);
// // 	fflush(stdout);
// // 	printf("page = -------->%x\n", page);
// // 	fflush(stdout);
// // 	printf("m = -------->%x\n", m);
// // 	fflush(stdout);
// // 	printf("offset-------->%x\n", offset);
// // 	fflush(stdout);
	
// 	memcpy(&t.val, hw_mem + m, 4); //这里报seg fault
// 	//assert(0);//这里加断点
// 	assert(t.present);
// 	PTE l;
	
// 	m = (t.page_frame << 12) + 4*page;
	
// // 	printf("m = -------->%x\n", m);
// // 	fflush(stdout);
// // 	printf("t.page_frame-------->%x\n", t.page_frame);
// // 	fflush(stdout);
// 	memcpy(&l.val, hw_mem + m, 4);

// 	//assert(0);
// // 	printf("-------->%d", l.present);
// // 	assert(l.present == 1);
//  //	assert(0);
// 	paddr_t result = (l.page_frame << 12) + offset;
// 	return result;
    // printf("------>%x\n", laddr);
    // fflush(stdout);
    // printf("------>%d\n", cpu.cr3.pdbr);;
    // fflush(stdout);
    
    uint32_t a = (uint32_t)hw_mem + (cpu.cr3.pdbr<<12) + 4*(laddr >> 22);
    
    PDE* b = (PDE*) a;
   // assert(b->present == 1);
    a = (uint32_t)hw_mem + 4*((laddr >> 12)&0x3ff) + (b -> page_frame << 12);
    PTE* res = (PTE*)a;
    //assert(res->present == 1);
    return (laddr&0xfff) + (res->page_frame << 12);
#else
	return tlb_read(laddr) | (laddr & PAGE_MASK);
#endif
}
