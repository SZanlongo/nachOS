#include "machine.h"
class CoreMap
{
	public:
		bool allocated;
		bool locked;
		AddrSpace *addrSpace;
		int virtualPageNumber;
};

class memMan
{
    public:

	memMan();
	int allocate();
	void deallocate(int pageNum);
	int getPages();
	CoreMap *coreMap;
 
    private:
    	bool mem[32];
	int numPages;
    int pageIndex;

};
