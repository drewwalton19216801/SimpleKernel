#include <simple/cmd/cmds.h>
#include <simple/initrd.h>
#include <simple/fs.h>

void cmd_ls() {
	int i = 0;
	struct dirent *node = 0;
	while ((node = readdir_fs(fs_root, i)) != 0) {
		monitor_write("Found file ");
		monitor_write(node->name);
		fs_node_t *fsnode = finddir_fs(fs_root, node->name);
		
		if ((fsnode->flags&0x7) == FS_DIRECTORY) {
			monitor_write("\n\t(directory)\n");
		} else {
			monitor_write("\n\t contents: \"");
			char buf[256];
			uint32_t sz = read_fs(fsnode, 0, 256, buf);
			int j;
			for (j = 0; j < sz; j++) {
				monitor_put(buf[j]);
			}
			monitor_write("\"\n");
		}
		i++;
	}
}
