#include <simple/cmd/cmds.h>
#include <simple/panic.h>

void cmd_panic() {
	asm volatile("int $0x3");
	for(;;);
}
