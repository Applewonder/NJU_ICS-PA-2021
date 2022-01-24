#include "common.h"
#include "memory.h"
#include <string.h>

#define VMEM_ADDR 0xa0000
#define SCR_SIZE (320 * 200)
#define NR_PT ((SCR_SIZE + PT_SIZE - 1) / PT_SIZE) // number of page tables to cover the vmem

PDE *get_updir();
//PTE uptable[NR_PT] align_to_page;

void create_video_mapping()
{
	PTE ptable[1024] align_to_page;
	/* TODO: create an identical mapping from virtual memory area
	 * [0xa0000, 0xa0000 + SCR_SIZE) to physical memeory area
	 * [0xa0000, 0xa0000 + SCR_SIZE) for user program. You may define
	 * some page tables to create this mapping.
	 */
	PDE *updir = (PDE *)va_to_pa(get_updir());
	//updir[0].val = make_pde(va_to_pa(uptable));
	//for(uint32_t addrv = 0xa0; addrv < 0xb0; addrv++) {
		//uptable[addrv].val = make_pde(0xa0 << 12);
	//}
	int count = 0;
	for(int i = 0xa0; count <= 0xf; ++count, ++i){
		ptable[0xa0 + count].val = make_pde(i << 12);
	}
	updir[0].val = make_pde(va_to_pa(ptable));
}

void video_mapping_write_test()
{
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for (i = 0; i < SCR_SIZE / 4; i++)
	{
		buf[i] = i;
	}
}

void video_mapping_read_test()
{
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for (i = 0; i < SCR_SIZE / 4; i++)
	{
		assert(buf[i] == i);
	}
}

void video_mapping_clear()
{
	memset((void *)VMEM_ADDR, 0, SCR_SIZE);
}
