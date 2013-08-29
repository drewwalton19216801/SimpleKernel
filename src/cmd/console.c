#include <simple/cmd/cmds.h>

const static struct {
	const char *name;
	void (*func)(void);
} function_map[] = {
	{ "panic", cmd_panic },
	{ "ver", cmd_ver },
	{ "reboot", cmd_reboot },
	{ "ls", cmd_ls },
};

void newcmd() {
	monitor_write("\n");
	monitor_write("# ");
}

int lookup_and_call(const char *name)
{
	int i;
	
	for (i = 0; i < (sizeof(function_map) / sizeof(function_map[0])); i++) {
		if (!strcmp(function_map[i].name, name) && function_map[i].func) {
			function_map[i].func();
			return 0;
		}
	}
	printk("Invalid command: %s", name);
	return -1;
}

char cmd[256];
char c;

void append(char * s, char c) {
	int len = strlen(s);
	s[len] = c;
	s[len+1] = '\0';
}

int conthread(void *arg)
{
	for (;;) {
		c = keyboard_getchar();
		if (c == '\n') {
			monitor_put(c);
			int ret;
			ret = return_fn(cmd);
			memset(&cmd[0], 0, sizeof(cmd));
			newcmd();
		} else {
			append(cmd, c);
			monitor_put(c);
		}
	}
	return 6;
}

void start_console() {
	uint32_t *stack = kmalloc(0x400) + 0x3F0;
	thread_t *t = create_thread(&conthread, (void*)0x567, stack);
	monitor_write("# ");
}

int return_fn(char *fn) {
	int ret;
	ret = lookup_and_call(fn);
	if (ret == -1) {
		memset(&cmd[0], 0, sizeof(cmd));
		return 1;
	}
	memset(&cmd[0], 0, sizeof(cmd));
	return 0;
}
