#include <simple/cmd/cmds.h>
#include <simple/reboot.h>

void cmd_reboot() {
	monitor_write("System is going down for reboot NOW!\n");
}
