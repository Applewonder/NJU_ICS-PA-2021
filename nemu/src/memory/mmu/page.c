#include "cpu/cpu.h"
#include "memory/memory.h"

// translate from linear address to physical address
paddr_t page_translate(laddr_t laddr)
{
#ifndef TLB_ENABLED
	uint32_t dir = laddr >> 22;
	uint32_t page = (laddr >> 12) % 10;
	uint32_t offset = laddr % 12;
	uint32_t m = (cpu.cr0.pdbr << 12) + 8*dir;
	PDE t;
	memcpy(&t, &m, 4);
	assert(t.present == 1);
	PTE l;
	memcpy(&l, (t.page_frame << 12) + 8*page, 4);
	assert(l.present == 1);
	paddr_t result = (l.page_frame << 12) + offset;
	return result;
#else
	return tlb_read(laddr) | (laddr & PAGE_MASK);
#endif
}
