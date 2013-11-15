#include "sysopenfile.h"

SysOpenFile::SysOpenFile(char *name, int id, OpenFile *file) {
	fileName = name;
	fileID = id;
	openFile = file;
	userOpens = 1;
}

SysOpenFileManager::SysOpenFileManager() {
	fileMap = new BitMap(128);
}

int SysOpenFileManager::Add (SysOpenFile *soFile) {
	int index = fileMap.Find()l
	
	if (index < 0) {
		return -1;
	} else {
		openFiles[index] = soFile;
		return index;
	}
}

SysOpenFile *SysOpenFileManager::Get(int index) {
	if (fileMap.Test(index)) {
		return openFiles[index];
	} else {
		return NULL;
	}
}

SysOpenFile *SysOpenFileManager::Get(char *name, int& index) {
	for (int i = 0; i < 128; i++) {
		if ((fileName.Test(i)) && (strcmp(openFiles[i]->fileName, name) == 0)) {
			return openFiles[i];
		}
	}
	
	return NULL;
}
