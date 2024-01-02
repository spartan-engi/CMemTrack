
	// pure C memory tracker
	// inneficient and slow
	// costs at least 44* extra bytes per alloc called
	// *on 64 bit systems
	// 2 pointers, size_t, int, and 2 strings

// call before anything
void memStart();
// call to print a small report
void memRep();
// call for deinitialization, not required
void memEnd();

// returns memory usage from allocations
long memGetUsage();
// difference of allocations and free operations
int  memGetAlocs();

// prints the size of all things allocated on the heap
void memDump();

// use these macros to get automatic get (line number / function name / file name) errors and report
#define memAloc(size)      _memAloc(size, __LINE__, __FUNCTION__, __FILE__)
#define memFree(pointer)   _memFree(pointer, __LINE__, __FUNCTION__, __FILE__)

// allocation wrapper
void* _memAloc(long size, int lineNum, const char* function, const char* file);
// de-allocation wrapper
void  _memFree(void* pointer, int lineNum, const char* function, const char* file);