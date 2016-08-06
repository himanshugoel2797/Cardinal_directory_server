#ifndef _CARDINAL_MOUNT_DB_H_
#define _CARDINAL_MOUNT_DB_H_

#include <stdint.h>
#include <cardinal/limits.h>

#include "list.h"

typedef enum {
	FileSystemError_None,
	FileSystemError_AllocationFailed,
	FileSystemError_PathDoesNotExist,
	FileSystemError_PathInvalid,
	FileSystemError_NameTooLong,
	FileSystemError_PathTooLong
} FileSystemError;

typedef enum {
	FileSystemObjectType_MountPoint,
	FileSystemObjectType_Directory,
	FileSystemObjectType_File
} FileSystemObjectType;

typedef struct {
	FileSystemObjectType ObjectType;
	char Name[NAME_MAX];

	union{
		uint64_t TargetPID;
		List *Children;
		uint64_t FileID;
	};
} FileSystemObject;

void
InitializeDB(void);

FileSystemError
CreateDirectory(char *path);

FileSystemError
CreateFile(char *path, uint64_t *fd);

FileSystemError
RegisterMount(char *path, uint64_t pid);

FileSystemObject*
ParsePath(const char *path);

#endif