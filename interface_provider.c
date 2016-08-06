#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "interface_provider.h"
#include "list.h"
#include "mount_db.h"

static uint64_t mountPath_hash = 0;

uint64_t open_mount(FileHandlers *handlers, const char *file, int flags, int mode) {
	//return a file descriptor and make sure the path is expected

	if(strcmp(file, "/srv/fs/mount") != 0)
		return -1;

	if(mountPath_hash == 0)
		mountPath_hash = HashPath("/srv/fs/mount");

	return AllocateFileDescriptor(flags, mode, mountPath_hash);
}

int close_mount(FileHandlers *handlers, uint64_t fd) {

	uint64_t hash = 0;
	if(!GetFileDescriptor(fd, NULL, NULL, &hash))
		return -1;

	if(hash != mountPath_hash)
		return -1;

	FreeFileDescriptor(fd);
}

int read_mount(FileHandlers *handlers, uint64_t fd, void *buf, size_t cnt) {

	int flags = 0;
	int mode = 0;
	uint64_t hash = 0;
	if(!GetFileDescriptor(fd, &flags, &mode, &hash))
		return -1;

	if(hash != mountPath_hash)
		return -1;



}

int write_mount(FileHandlers *handlers, uint64_t fd, void *buf, size_t cnt) {

	int flags = 0;
	int mode = 0;
	uint64_t hash = 0;
	if(!GetFileDescriptor(fd, &flags, &mode, &hash))
		return -1;

	if(hash != mountPath_hash)
		return -1;


}

bool
InitializeInterface(void) {
	if(CreateDirectory("/srv") != FileSystemError_None)
		return false;

	if(CreateDirectory("/srv/fs") != FileSystemError_None)
		return false;

	FileHandlers *mnt_handlers = malloc(sizeof(FileHandlers));
	mnt_handlers->open = open_mount;
	mnt_handlers->close = close_mount;
	mnt_handlers->read = read_mount;
	mnt_handlers->write = write_mount;


	if(CreateFile("/srv/fs/mount", mnt_handlers) != FileSystemError_None)
		return false;

	return true;
}