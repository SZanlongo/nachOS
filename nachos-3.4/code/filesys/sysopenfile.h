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

#endif
