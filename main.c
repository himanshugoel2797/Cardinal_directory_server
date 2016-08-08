#include <stddef.h>
#include <stdlib.h>
#include <time.h>

#include <cardinal/ipc.h>
#include <cardinal/file_server.h>

#include "mount_db.h"
#include "interface_provider.h"
#include "file_request_handlers.h"

int main() {

	RegisterSpecialDestination(CARDINAL_IPCDEST_FILESERVER);
	InitializeDB();
	InitializeInterface();


	struct timespec t;
	t.tv_sec = 0;
	t.tv_nsec = 100;

	Message *m = malloc(UINT16_MAX);
	while(true) {
		while(!GetIPCMessageFrom(m, 0, 0))
			nanosleep(&t, NULL);

		struct OpenRequest * test_req = (struct OpenRequest*) m;

		switch(test_req->msg_type) {
			case CARDINAL_MSG_TYPE_OPENREQUEST:
				HandleOpenRequest(m);
			break;
			case CARDINAL_MSG_TYPE_CLOSEREQUEST:
				HandleCloseRequest(m);
			break;
			case CARDINAL_MSG_TYPE_READREQUEST:
				HandleReadRequest(m);
			break;
			case CARDINAL_MSG_TYPE_WRITEREQUEST:
				HandleWriteRequest(m);
			break;
		}

	}

}