#include <stdint.h>

#include <cardinal/file_server.h>

#include "file_request_handlers.h"
#include "mount_db.h"

void
HandleOpenRequest(Message *m) {
	struct OpenRequest *open_req = (struct OpenRequest*)m;

	FileSystemObject *fs_obj = ParsePath(open_req->path);

	uint64_t fd = 0;

	if(fs_obj->ObjectType == FileSystemObjectType_File)
	{
		fd = fs_obj->handlers->open(fs_obj->handlers, open_req->path, (int)open_req->flags, (int)open_req->mode);
	}else if(fs_obj->ObjectType == FileSystemObjectType_MountPoint)
	{
		//Direct request to mount point service and return the PID returned by it
	}
}