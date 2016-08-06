#include <stdlib.h>
#include <string.h>

#include "mount_db.h"


static FileSystemObject *root;

int
strcmp_path(const char *a, const char *b) {

	const char *a0 = (strlen(a) < strlen(b))?a:b;	//a0 is the shorter string
	const char *a1 = (strlen(a) < strlen(b))?b:a;	//a1 is the longer string

	for(int i = 0; i < strlen(a0); i++) {
		if(a0[i] != a1[i])
			return 0;
	}

	return 1;
}

void
InitializeDB(void) {
	root = malloc(sizeof(FileSystemObject));

	root->ObjectType = FileSystemObjectType_Directory;
	root->Name[0] = 0;
	root->Children = List_Create();
}

FileSystemObject*
ParsePath(const char *path) {
	FileSystemObject *r = root;
	
	if(path[0] != '/')
		return NULL;

	path++;

	while(path[0] != 0) {


		uint64_t i = 0;
		for(; i < List_Length(r->Children); i++) {

			FileSystemObject *a = (FileSystemObject*)List_EntryAt(r->Children, i);

			if(strcmp_path(a->Name, path) && (path[strlen(a->Name)] == '/' || path[strlen(a->Name)] == 0)) {
				r = a;
				path += strlen(a->Name) + 1;
				break;
			}
		}
		if(i == List_Length(r->Children))
			return NULL;

		if(r->ObjectType == FileSystemObjectType_MountPoint || r->ObjectType == FileSystemObjectType_File)
			break;
	}

	return r;
}

FileSystemError
SetupEntry(char *path, FileSystemObject **obj, char **name) {
	//Extract the directory name
	size_t path_len = strnlen(path, PATH_MAX);

	if(path_len == PATH_MAX)
		return FileSystemError_PathTooLong;

	char *offset = NULL;

	for(size_t i = path_len - 1; i >= 0; i--){
		if(path[i] == '/' && i != 0) {
			offset = &path[i + 1];

			if(strlen(offset) >= NAME_MAX)
				return FileSystemError_NameTooLong;

			path[i] = 0;
			break;
		}
	}

	FileSystemObject* r = ParsePath(path);
	if(r == NULL)
		return FileSystemError_PathDoesNotExist;

	offset[-1] = '/';

	if(obj != NULL)*obj = r;
	if(name != NULL)*name = offset;

	return FileSystemError_None;
}

FileSystemError
CreateDirectory(char *path){
	
	char *offset = NULL;
	FileSystemObject *r = NULL;
	FileSystemError err = SetupEntry(path, &r, &offset);
	if(err != FileSystemError_None)
		return err;

	if(r->ObjectType != FileSystemObjectType_Directory)
		return FileSystemError_PathInvalid;

	FileSystemObject *dir = malloc(sizeof(FileSystemObject));
	if(dir == NULL)
		return FileSystemError_AllocationFailed;

	dir->ObjectType = FileSystemObjectType_Directory;
	strcpy(dir->Name, offset);
	dir->Children = List_Create();

	return FileSystemError_None;
}

FileSystemError
CreateFile(char *path, uint64_t *fd) {

	static uint64_t file_ids = 0;

	char *offset = NULL;
	FileSystemObject *r = NULL;
	FileSystemError err = SetupEntry(path, &r, &offset);
	if(err != FileSystemError_None)
		return err;

	if(r->ObjectType != FileSystemObjectType_Directory)
		return FileSystemError_PathInvalid;

	FileSystemObject *dir = malloc(sizeof(FileSystemObject));
	if(dir == NULL)
		return FileSystemError_AllocationFailed;

	dir->ObjectType = FileSystemObjectType_File;
	strcpy(dir->Name, offset);
	dir->FileID = ++file_ids;
	if(fd != NULL)*fd = dir->FileID;

	return FileSystemError_None;
}

FileSystemError
RegisterMount(char *path, uint64_t pid) {

	char *offset = NULL;
	FileSystemObject *r = NULL;
	FileSystemError err = SetupEntry(path, &r, &offset);
	if(err != FileSystemError_None)
		return err;

	if(r->ObjectType != FileSystemObjectType_Directory)
		return FileSystemError_PathInvalid;

	FileSystemObject *dir = malloc(sizeof(FileSystemObject));
	if(dir == NULL)
		return FileSystemError_AllocationFailed;

	dir->ObjectType = FileSystemObjectType_MountPoint;
	strcpy(dir->Name, offset);
	dir->TargetPID = pid;

	return FileSystemError_None;
}