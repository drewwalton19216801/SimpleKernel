#ifndef _CMD_H
#define _CMD_H

#include <simple/string.h>
#include <simple/common.h>
#include <simple/monitor.h>
#include <simple/thread.h>
#include <simple/lock.h>
#include <simple/keyboard.h>

#include <simple/cmd/cmd_panic.h>
#include <simple/cmd/cmd_reboot.h>
#include <simple/cmd/cmd_ver.h>
#include <simple/cmd/cmd_ls.h>

extern int conthread(void *arg);
extern void start_console();
extern int return_fn(char *fn);
extern int lookup_and_call(const char *name);


#endif
