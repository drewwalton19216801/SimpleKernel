#include <simple/reboot.h>
#include <simple/common.h>

static long no_idt[2] = {0, 0};

static inline void kb_wait(void)
{
	int i;

	for (i=0; i<0x10000; i++)
		if ((inb(0x64) & 0x02) == 0)
			break;
}

void hard_reset_now(void)
{
	int i, j;
	
	asm volatile ("sti");
	*((unsigned short *)0x472) = 0x1234;
	for (;;) {
		for (i=0; i<100; i++) {
			kb_wait();
			for(j = 0; j < 100000; j++)
			outb(0xfe, 0x64);
		}
		asm volatile ("\tlidt %0": "=m" (no_idt));
	}
}
