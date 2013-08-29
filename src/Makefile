CC=gcc
SOURCES=asm/boot.o \
		init/main.o \
		drivers/video/monitor.o \
		lib/common.o \
		lib/string.o \
		init/descriptor_tables.o \
		kernel/isr.o \
		asm/interrupt.o \
		asm/gdt.o \
		kernel/timer.o \
        mm/kheap.o \
        mm/paging.o \
        mm/ordered_array.o \
        fs/fs.o \
        fs/initrd.o \
        kernel/task.o \
        kernel/process.o \
        kernel/syscall.o
        
KOUT=../out/kernel

INCPATH=-I./include

CFLAGS=-m32 -nostdlib -nostdinc -fno-builtin $(INCPATH)
LDFLAGS=-Tlink.ld
ASFLAGS=-felf

all: $(SOURCES) link

clean:
	-find ./ -type f -name '*.o' -delete
	-rm -f $(KOUT)

link:
	ld -melf_i386 $(LDFLAGS) -o $(KOUT) $(SOURCES)

.s.o:
	nasm $(ASFLAGS) $<
