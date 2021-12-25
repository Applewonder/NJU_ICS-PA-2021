#include "cpu/cpu.h"
#include "memory/memory.h"

// translate from linear address to physical address
paddr_t page_translate(laddr_t laddr)
{
 #ifndef TLB_ENABLED
    uint32_t a = (uint32_t)hw_mem + (cpu.cr3.pdbr<<12) + 4*(laddr >> 22);
    PDE* b = (PDE*) a;
    assert(b->present == 1);
    a = (uint32_t)hw_mem + 4*((laddr >> 12)&0x3ff) + (b -> page_frame << 12);
    PTE* res = (PTE*)a;
    assert(res->present == 1);
    return (laddr&0xfff) + (res->page_frame << 12);
#else
	return tlb_read(laddr) | (laddr & PAGE_MASK);
#endif
}
