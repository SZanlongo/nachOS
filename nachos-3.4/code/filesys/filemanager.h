#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "openfile.h"
#include "bitmap.h"
#include "sysopenfile.h"

class FileManager {
	public:
		SysOpenFileManager();
		int Add(SysOpenFile *newsoFile);
		SysOpenFile *Get(int index);
		SysOpenFile *Get(char *fileName, int& index);		
	
	private:
		SysOpenFile *openFiles[128];
		BitMap *fileMap;
};

#endif
