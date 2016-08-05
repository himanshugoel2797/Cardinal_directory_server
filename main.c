#include <stddef.h>
#include <stdlib.h>
#include <cardinal/ipc.h>
#include <cardinal/file_server.h>


int main() {

	RegisterSpecialDestination(CARDINAL_IPCDEST_FILESERVER);

	Message *m = malloc(UINT16_MAX);
	while(true) {
		while(!GetMessage(m));

		struct OpenRequest * test_req = (struct OpenRequest*) m->Content;

		switch(test_req->msg_type) {
			case CARDINAL_MSG_TYPE_OPENREQUEST:

			break;
			case CARDINAL_MSG_TYPE_CLOSEREQUEST:

			break;
			case CARDINAL_MSG_TYPE_READREQUEST:

			break;
			case CARDINAL_MSG_TYPE_WRITEREQUEST:

			break;
		}

	}

}