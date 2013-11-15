#ifndef SYSOPENFILE_H
#define SYSOPENFILE_H

#include "openfile.h"

class SysOpenFile {
    public:
		char *fileName;
		int fileID;
		OpenFile *openFile;	
		int userOpens;
		SysOpenFile (char *name, int id, OpenFile *file);
};

class SysOpenFileManager {
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
