#ifndef _CARDINAL_MOUNT_DB_H_
#define _CARDINAL_MOUNT_DB_H_

#include <stdint.h>
#include <stdbool.h>
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

uint64_t
AllocateFileDescriptor(int flags, int mode);

bool
GetFileDescriptor(uint64_t fd, int *flags, int *mode);

void
FreeFileDescriptor(uint64_t fd);

typedef struct FileHandlers FileHandlers;
struct FileHandlers {
	uint64_t (*open)(FileHandlers *handlers, const char *file, int flags, int mode);
	int (*close)(FileHandlers *handlers, uint64_t fd);
	int (*read)(FileHandlers *handlers, uint64_t fd, void *buf, size_t cnt);
	int (*write)(FileHandlers *handlers, uint64_t fd, void *buf, size_t cnt);
};

typedef struct {
	FileSystemObject *Parent;
	FileSystemObjectType ObjectType;
	char Name[NAME_MAX];

	union{
		uint64_t TargetPID;
		List *Children;
		FileHandlers *handlers;
	};
} FileSystemObject;

void
InitializeDB(void);

FileSystemError
CreateDirectory(char *path);

FileSystemError
CreateFile(char *path, FileHandlers *handlers);

FileSystemError
RegisterMount(char *path, uint64_t pid);

FileSystemObject*
ParsePath(const char *path);

#endif