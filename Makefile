CSOURCES=$(shell find -maxdepth 10 -name *.c)
COBJECTS=$(patsubst %.c, %.o, $(CSOURCES))
SSOURCES=$(shell find -maxdepth 10 -name *.s)
SOBJECTS=$(patsubst %.s, %.o, $(SSOURCES))

CC=gcc
LD:=ld
        
KOUT=./out/kernel

INCPATH=-I./src/include

CFLAGS=-nostdlib -fno-builtin -m32 $(INCPATH)
LDFLAGS=-melf_i386 -Tsrc/link.ld
ASFLAGS=-felf

all: $(COBJECTS) $(SOBJECTS) link

clean:
	-find ./src/ -type f -name '*.o' -delete
	-rm -f $(KOUT)

link:
	@echo Linking
	@$(LD) $(LDFLAGS) -o $(KOUT) $(SOBJECTS) $(COBJECTS)

.s.o:
	@echo Assembling $<
	@nasm $(ASFLAGS) $<

.c.o:
	@echo Compiling $<
	@$(CC) $(CFLAGS) -o $@ -c $<
