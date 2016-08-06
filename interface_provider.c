#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "interface_provider.h"
#include "list.h"
#include "mount_db.h"

uint64_t open_mount(FileHandlers *handlers, const char *file, int flags, int mode) {
	//return a file descriptor and make sure the path is expected
}

int close_mount(FileHandlers *handlers, uint64_t fd) {

}

int read_mount(FileHandlers *handlers, uint64_t fd, void *buf, size_t cnt) {

}

int write_mount(FileHandlers *handlers, uint64_t fd, void *buf, size_t cnt) {

}

bool
InitializeInterface(void) {
	if(CreateDirectory("/srv") != FileSystemError_None)
		return false;

	if(CreateDirectory("/srv/fs") != FileSystemError_None)
		return false;

	FileHandlers *mnt_handlers = malloc(sizeof(FileHandlers));
	mnt_handlers->fd_cnt = 0;
	mnt_handlers->fds = List_Create();
	mnt_handlers->open = open_mount;
	mnt_handlers->close = close_mount;
	mnt_handlers->read = read_mount;
	mnt_handlers->write = write_mount;


	if(CreateFile("/srv/fs/mount", mnt_handlers) != FileSystemError_None)
		return false;

	return true;
}