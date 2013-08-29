// main.c -- Defines the C-code kernel entry point, calls initialisation routines.

#include <simple/multiboot.h>
#include <simple/common.h>

#include <simple/monitor.h>
#include <simple/gdt.h>
#include <simple/idt.h>
#include <simple/timer.h>
#include <simple/elf.h>
#include <simple/pmm.h>
#include <simple/vmm.h>
#include <simple/heap.h>
#include <simple/thread.h>
#include <simple/lock.h>
#include <simple/keyboard.h>
#include <simple/string.h>


elf_t kernel_elf;

spinlock_t lock = SPINLOCK_UNLOCKED;

void append(char * s, char c)
{
	int len = strlen(s);
	s[len] = c;
	s[len+1] = '\0';
}

char cmd[256];
char PanicCmd[] = "panic";
char c;
char *cmdptr;

int kbthread(void *arg)
{
	for (;;) {
		c = keyboard_getchar();
		if (c == '\n') {
			monitor_put(c);
			if (strncmp(cmd, PanicCmd, 1) == 0) {
				panic("Got forced panic!");
			} else {
				printk("Got command: %s", cmd);
				memset(&cmd[0], 0, sizeof(cmd));
				monitor_put(c);
				monitor_write("$ ");
			}
		} else {
			append(cmd, c);
			monitor_put(c);
		}
	}
	return 6;
}

int kernel_main(multiboot_t *mboot_ptr)
{

  monitor_clear();

  init_gdt ();
  init_idt ();

  init_timer (20);
  init_pmm (mboot_ptr->mem_upper);
  init_vmm ();
  init_heap ();

  // Find all the usable areas of memory and inform the physical memory manager about them.
  uint32_t i = mboot_ptr->mmap_addr;
  while (i < mboot_ptr->mmap_addr + mboot_ptr->mmap_length)
  {
    mmap_entry_t *me = (mmap_entry_t*) i;

    // Does this entry specify usable RAM?
    if (me->type == 1)
    {
      uint32_t j;
      // For every page in this entry, add to the free page stack.
      for (j = me->base_addr_low; j < me->base_addr_low+me->length_low; j += 0x1000)
      {
        pmm_free_page (j);
      }
    }

    // The multiboot specification is strange in this respect - the size member does not include "size" itself in its calculations,
    // so we must add sizeof (uint32_t).
    i += me->size + sizeof (uint32_t);
  }
  kernel_elf = elf_from_multiboot (mboot_ptr);
  asm volatile ("sti");
  init_scheduler (init_threading ());

  asm volatile("sti");
  init_keyboard_driver();
  monitor_write("SimpleKernel 0.0.2\n");
  monitor_write("$ ");
  uint32_t *stack = kmalloc(0x400) + 0x3F0;
  thread_t *t = create_thread(&kbthread, (void*)0x567, stack);
  for(;;)
  {
      
  }
  for (;;);
	return 0xdeadbeef;

}
