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

#include <simple/cmd/cmds.h>


elf_t kernel_elf;

spinlock_t lock = SPINLOCK_UNLOCKED;

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
  start_console();
  for(;;)
  {
      
  }
  for (;;);
	return 0xdeadbeef;

}
