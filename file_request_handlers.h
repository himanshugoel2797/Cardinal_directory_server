#ifndef _CARDINAL_DIRECTORY_SERVER_FILE_REQUEST_HANDLERS_H_
#define _CARDINAL_DIRECTORY_SERVER_FILE_REQUEST_HANDLERS_H_

#include <cardinal/ipc.h>

//TODO: Implement system to handle list of service provider mounts and server files

void
HandleOpenRequest(Message *m);

void
HandleCloseRequest(Message *m);

void
HandleReadRequest(Message *m);

void
HandleWriteRequest(Message *m);


#endif