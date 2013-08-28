SOURCES=boot.o \
		main.o \
		monitor.o \
		common.o \
		descriptor_tables.o \
		isr.o \
		interrupt.o \
		gdt.o \
		timer.o \
        kheap.o \
        paging.o \
        ordered_array.o \
        fs.o \
        initrd.o \
        task.o \
        process.o \
        syscall.o
        
KOUT=../out/kernel

INCPATH=-I./include

CFLAGS=-m32 -nostdlib -nostdinc -fno-builtin $(INCPATH)
LDFLAGS=-Tlink.ld
ASFLAGS=-felf

all: $(SOURCES) link

clean:
	-rm *.o $(KOUT)

link:
	ld -melf_i386 $(LDFLAGS) -o $(KOUT) $(SOURCES)

.s.o:
	nasm $(ASFLAGS) $<
