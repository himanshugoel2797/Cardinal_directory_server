#include <stdint.h>

#include <time.h>
#include <cardinal/file_server.h>

#include "file_request_handlers.h"
#include "mount_db.h"

void
HandleOpenRequest(Message *m) {
	struct OpenRequest *open_req = (struct OpenRequest*)m;
	struct OpenResponse response;

	FileSystemObject *fs_obj = ParsePath(open_req->path);

	uint64_t fd = -1;
	
	response.m.SourcePID = CARDINAL_IPCDEST_FILESERVER;
	response.m.DestinationPID = open_req->m.SourcePID;
	response.m.MsgID = open_req->m.MsgID;
	response.m.Size = sizeof(struct OpenResponse);

	if(fs_obj != NULL){
		if(fs_obj->ObjectType == FileSystemObjectType_File)
		{
			fd = fs_obj->handlers->open(fs_obj->handlers, open_req->path, (int)open_req->flags, (int)open_req->mode);	
		}
		else if(fs_obj->ObjectType == FileSystemObjectType_MountPoint)
		{
			//Direct request to mount point service and return the PID returned by it
			uint64_t src = open_req->m.SourcePID;

			open_req->m.SourcePID = CARDINAL_IPCDEST_FILESERVER;
			open_req->m.DestinationPID = fs_obj->TargetPID;

			PostIPCMessages((Message**)&open_req, 1);

			struct timespec t;
			t.tv_sec = 0;
			t.tv_nsec = 100;

			while(GetIPCMessageFrom((Message*)&response, fs_obj->TargetPID, open_req->m.MsgID) != 1)
				nanosleep(&t, NULL);

			response.m.SourcePID = CARDINAL_IPCDEST_FILESERVER;
			response.m.DestinationPID = src;

			Message *ma = (Message*)&response;
			PostIPCMessages(&ma, 1);
			return;
		}
	}

	response.msg_type = CARDINAL_MSG_TYPE_OPENRESPONSE;
	response.fd = fd;
	response.targetPID = CARDINAL_IPCDEST_FILESERVER;

	Message *ma = (Message*)&response;
	PostIPCMessages(&ma, 1);
	return;
}