#include <stddef.h>
#include <stdbool.h>
// gets size_t

	// simply include this and start calling.

	// pure C memory tracker
	// using a linked list
	// inneficient and slow
	// costs at least 44* extra bytes per alloc called
	// *on 64 bit systems
	// 2 pointers, size_t, int, and 2 strings



/* configurations */

	// by default "1", activates the whole project
	// if deactivated, the wrappers will simply become calls to alloc/calloc/realloc
	// use to deactivate all profiling without messing with your build tool much
	// compiler will just optimise away the intermediary function calls
	#ifndef CMemTrackACTIVE
	#define CMemTrackACTIVE 1
	// #define CMemTrackACTIVE 0
	#endif

	// by default "0", reallocated pointers still count as 'responsability' of the original alloc,
	// thus, by default realloc doesn't change the function/file origin information
	#ifndef ReallocCountsAsOrigin
	#define ReallocCountsAsOrigin 0
	// #define ReallocCountsAsOrigin 1
	#endif

	// by default "1", searches for the pointers on the linked list before free()/realloc()
	// thus makes code slower, but easier to debug
	// disabling this makes code down right crash instead of erroring out when an exception is encountered
	#ifndef CheckIfPointerValid
	#define CheckIfPointerValid 1
	// #define CheckIfPointerValid 0
	#endif

/* configurations */


// name mangling can break stuff
// so including thi in a c++ source files requires this
#if defined(__cplusplus)
extern "C" {
#endif

// call before anything, initializes linked list
void memStart();
// call to print a 'small' report
void memRep();
// call for deinitialization, not required
void memEnd();

// returns memory usage from allocations
long memGetUsage();
// difference of allocations and free operations
int  memGetAlocs();

// returns true if a pointer was alocated by CMemTracker
// included by the sake of completness, used internally
bool memIsTracked(void* pointer);
// returns the size of memory that the pointer is pointing to
// returns 0 if no pointer found. also returns 0 if pointer has size of 0 bytes.
// one can allocate 0 bytes of memory... this sounds so wrong
size_t memPointerSize(void* pointer);

// prints the size of all things allocated on the heap, can get big.
void memDump();

// use these macros to get automatic get (line number / function name / file name) errors and report
#define memAloc(size)      _memAloc(size, __LINE__, __FUNCTION__, __FILE__)
#define memCLoc(size, num) _memCLoc(size, num, __LINE__, __FUNCTION__, __FILE__)
#define memRLoc(pnt, size) _memRLoc(pnt, size, __LINE__, __FUNCTION__, __FILE__)
#define memFree(pointer)   _memFree(pointer, __LINE__, __FUNCTION__, __FILE__)

// allocation wrapper
void* _memAloc(size_t size, int lineNum, const char* function, const char* file);
// allocation wrapper2, doesn't have calloc's int overflow protection... never understood calloc either
void* _memCLoc(size_t number_of_elements, size_t size_of_element, int lineNum, const char* function, const char* file);
// re-allocation wrapper
void* _memRLoc(void* pointer, size_t size, int lineNum, const char* function, const char* file);
// de-allocation wrapper
void  _memFree(void* pointer, int lineNum, const char* function, const char* file);


#if defined(__cplusplus)
}
#endif