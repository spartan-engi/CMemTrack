#include "mem.h"
#include <stdlib.h>



#if     CMemTrackACTIVE
// if active, compile normally

// most uncludes are only nescessary when active
#include <stdio.h>
#include <string.h>

typedef struct Cell Cell;
struct Cell {
	Cell* prev;
	size_t size;
	Cell* next;

	char* function;
	char* file;
	int   line;

	// the true size of the cell varies
	// rest of the cell is the requested allocated memory
};


// memory tracker is simply implemented as a linked list holding all the pointers allocated
Cell LL;
long total_memory_usage;
int number_of_allocations;


void memStart()
{
	total_memory_usage = 0;
	number_of_allocations = 0;

	LL.size = 0;
	LL.prev = &LL;
	LL.next = &LL;
	LL.function = 0;
	LL.file = 0;
	LL.line = 0;

	return;
}
void memRep()
{
	printf("\n=====================================================\n");
	printf("[MEM][INFO]: memory tracking requires %llu extra bytes.\n", sizeof(Cell));
	
	printf("[MEM][INFO]: a total of %d ", number_of_allocations);
	printf("allocation operation(s) have yet to be freed\n");

	printf("[MEM][INFO]: summing to %ld ", total_memory_usage);
	printf("byte(s) of data\n");

	memDump();

	printf("=====================================================\n");

	return;
}
void memEnd()
{
	// yes, it will clean
	Cell* temp = LL.next;
	while(temp != &LL)
	{
		Cell* t = temp;
		temp = temp->next;

		free(t->function);
		free(t->file);
		free(t);
	}
}

long memGetUsage()
{
	return total_memory_usage;
}
int  memGetAlocs()
{
	return number_of_allocations;
}

void memDump()
{
	printf("\n[MEM][INFO]: non freed memory locations:\n");

	// step on linked list
	Cell* temp = LL.next;
	while(temp != &LL)
	{
		Cell* t = temp;
		temp = temp->next;

		printf("\t%zu bytes in function: %s at %s:%d\n", t->size, t->function, t->file, t->line);
	}

	return;
}

// helper function
size_t strSize(const char* str)
{
	int i;
	for(i = 0; str[i] != '\0'; i++);
	return i + 1;
}

void* _memCLoc(size_t number_of_elements, size_t size_of_element, int lineNum, const char* function, const char* file)
{
	// there goes overflow protection
	// TODO: get it back, somehow
	size_t size = size_of_element * number_of_elements;

	// well this is dumb
	Cell* pnt = calloc(1, sizeof(Cell) + size);

	pnt->prev = LL.prev;
	pnt->next = &LL;
	pnt->size = size;

	pnt->line = lineNum;

	pnt->function = malloc(strSize(function));
	memcpy(pnt->function, function, strSize(function));

	pnt->file = malloc(strSize(file));
	memcpy(pnt->file, file, strSize(file));

	LL.prev->next = pnt;
	LL.prev = pnt;

	total_memory_usage += size;
	number_of_allocations++;

	// pointer arithmatic
	// jumps one Cell space
	return pnt + 1;
}

void* _memAloc(size_t size, int lineNum, const char* function, const char* file)
{
	Cell* pnt = malloc(sizeof(Cell) + size);

	pnt->prev = LL.prev;
	pnt->next = &LL;
	pnt->size = size;

	pnt->line = lineNum;

	pnt->function = malloc(strSize(function));
	memcpy(pnt->function, function, strSize(function));

	pnt->file = malloc(strSize(file));
	memcpy(pnt->file, file, strSize(file));

	LL.prev->next = pnt;
	LL.prev = pnt;

	total_memory_usage += size;
	number_of_allocations++;

	// pointer arithmatic
	// jumps one Cell space
	return pnt + 1;
}

void* _memRLoc(void* pointer, size_t size, int lineNum, const char* function, const char* file)
{
	Cell* temp = LL.next;

	Cell* p = pointer;
	// pointer arithmatic
	// backtracks one Cell space
	p = p  - 1;

	while((temp != &LL) && (temp != p))	temp = temp->next;

	// pointer not found
	if(temp == &LL)
	{
		// non intended behaviour
		if(pointer != NULL)
		{
			// shall presume this was an error and behave as if it was a NULL pointer
			printf(
			"\n[MEM][ERROR]: %s:%d: the function %s tried reallocating non alocated memory space different from NULL\n"
			, file, lineNum, function);
		}

		// realloc behaviour when fed NULL is same as malloc
		return _memAloc(size, lineNum, function, file);
	}

	// get desired size, plus cell space
	p = realloc(p, size + sizeof(Cell));

	// in case p's address changed
	p->next->prev = p;
	p->prev->next = p;

	// difference in sizes
	total_memory_usage += size - p->size;
	p->size = size;

#if ReallocCountsAsOrigin
	free(p->function);
	p->function = malloc(strSize(function));
	memcpy(p->function, function, strSize(function));

	free(p->file);
	p->file = malloc(strSize(file));
	memcpy(p->file, file, strSize(file));

	p->line = lineNum;
#endif

	// pointer arithmatic
	// jumps one Cell space
	return p + 1;
}

void  _memFree(void* pointer, int lineNum, const char* function, const char* file)
{
	Cell* temp = LL.next;

	Cell* p = pointer;
	// pointer arithmatic
	// backtracks one Cell space
	p = p  - 1;

	while(temp != &LL)
	{
		if(temp == p)
		{
			temp->prev->next = temp->next;
			temp->next->prev = temp->prev;

			total_memory_usage -= temp->size;
			number_of_allocations--;

			free(temp->function);
			free(temp->file);
			free(temp);
			return;
		}

		temp = temp->next;
	}

	printf("\n[MEM][ERROR]: %s:%d: the function %s tried freeing non alocated memory space\n",
	 file, lineNum, function);

	return;
}

#else //CMemTrackACTIVE
// if inactive, compile the simplest possible wrapper


void memStart()		{return;}
void memRep()		{return;}
void memEnd()		{return;}
long memGetUsage()	{return 0;}
int  memGetAlocs()	{return 0;}
void memDump()		{return;}

void* _memCLoc(size_t number_of_elements, size_t size_of_element, int lineNum, const char* function, const char* file)
{
	return calloc(number_of_elements, size_of_element);
}

void* _memAloc(size_t size, int lineNum, const char* function, const char* file)
{
	return malloc(size);
}

void* _memRLoc(void* pointer, size_t size, int lineNum, const char* function, const char* file)
{
	return realloc(pointer, size);
}

void  _memFree(void* pointer, int lineNum, const char* function, const char* file)
{
	free(pointer);
	return;
}

#endif//CMemTrackACTIVE