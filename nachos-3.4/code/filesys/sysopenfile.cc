#include "sysopenfile.h"

SysOpenFile::SysOpenFile(char *name, int id, OpenFile *file) {
	fileName = name;
	fileID = id;
	openFile = file;
	userOpens = 1;
}
