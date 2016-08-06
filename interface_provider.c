#include <string.h>
#include <stdint.h>

#include "mount_db.h"

bool
InitializeInterface(void) {
	if(CreateDirectory("/srv") != FileSystemError_None)
		return FALSE;

	if(CreateDirectory("/srv/fs") != FileSystemError_None)
		return FALSE;

	uint64_t fd = 0;
	if(CreateFile("/srv/fs/mount", &fd) != FileSystemError_None)
		return FALSE;

	
}