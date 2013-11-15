#ifndef USEROPENFILE_H
#define USEROPENFILE_H

class UserOpenFile {
    public:
		char *fileName;
		int index;
		int offset;
		UserOpenFile(char *name, int in, int off);
};

#endif
