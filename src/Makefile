SOURCES=boot.o \
		main.o \
		monitor.o \
		common.o \
		descriptor_tables.o \
		isr.o \
		interrupt.o \
		gdt.o

INCPATH=-I./include \
		-I./include/lib \
		-I./include/simple
		
CFLAGS=-m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector $(INCPATH)
LDFLAGS=-Tlink.ld
ASFLAGS=-felf

all: $(SOURCES) link

clean:
	-rm -rf *.o ../out/kernel

link:
	ld -melf_i386 $(LDFLAGS) -o ../out/kernel $(SOURCES)

.s.o:
	nasm $(ASFLAGS) $<
