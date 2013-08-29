// main.c -- Defines the C-code kernel entry point, calls initialisation routines.

#include <simple/monitor.h>
#include <simple/descriptor_tables.h>
#include <simple/timer.h>
#include <simple/paging.h>
#include <simple/multiboot.h>
#include <simple/fs.h>
#include <simple/initrd.h>
#include <simple/task.h>
#include <simple/syscall.h>
#include <simple/string.h>

extern u32int placement_address;
u32int initial_esp;

int main(struct multiboot *mboot_ptr, u32int initial_stack)
{
    initial_esp = initial_stack;
    // Initialise all the ISRs and segmentation
    init_descriptor_tables();
    // Initialise the screen (by clearing it)
    monitor_clear();

    // Initialise the PIT to 100Hz
    asm volatile("sti");
    init_timer(50);

    // Find the location of our initial ramdisk.
    ASSERT(mboot_ptr->mods_count > 0);
    u32int initrd_location = *((u32int*)mboot_ptr->mods_addr);
    u32int initrd_end = *(u32int*)(mboot_ptr->mods_addr+4);
    // Don't trample our module with placement accesses, please!
    placement_address = initrd_end;

    // Start paging.
    initialise_paging();

    // Start multitasking.
    initialise_tasking();

    // Initialise the initial ramdisk, and set it as the filesystem root.
    fs_root = initialise_initrd(initrd_location);

    initialise_syscalls();

    switch_to_user_mode();

    syscall_monitor_write("Hello, user world!\n");
    int compare = strcmp("hello", "bye");
    if (compare == 0) {
		// this should never happen
		syscall_monitor_write("\"hello\" and \"bye\" are equal\n");
	} else {
		syscall_monitor_write("\"hello\" and \"bye\" are not equal\n");
	}
	
	compare = strcmp("hello", "hello");
    if (compare == 0) {
		syscall_monitor_write("\"hello\" and \"hello\" are equal\n");
	} else {
		// this should never happen
		syscall_monitor_write("\"hello\" and \"hello\" are not equal\n");
	}

    return 0;
}
