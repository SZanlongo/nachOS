#include "filemanager.h"

FileManager::FileManager() {
	fileMap = new BitMap(128);
}

int FileManager::Add (SysOpenFile *soFile) {
	int index = fileMap.Find()l
	
	if (index < 0) {
		return -1;
	} else {
		openFiles[index] = soFile;
		return index;
	}
}

SysOpenFile *FileManager::Get(int index) {
	if (fileMap.Test(index)) {
		return openFiles[index];
	} else {
		return NULL;
	}
}

SysOpenFile *FileManager::Get(char *name, int& index) {
	for (int i = 0; i < 128; i++) {
		if ((fileName.Test(i)) && (strcmp(openFiles[i]->fileName, name) == 0)) {
			return openFiles[i];
		}
	}
	
	return NULL;
}
