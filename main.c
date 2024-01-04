// example of usage of CMemTrack

// simply include header
// and start coding
#include "mem.h"
#include <stdio.h>

// used to mark the memory as 'used'
void fillMem(void* pnt, int size)
{
	char* p = pnt;

	for(int i = 0; i < (size/sizeof(char)); i++)
	{
		p[0] = 0;
	}
	return;
}

// arbitrary length function name encoding
void this_function_will_give_out_a_free_error_and_it_will_be_big()
{
	memAloc(64);
	memFree(0);
}




int main()
{
	void* things[7];
	
	// call to initialize memory tracker
	memStart();

	printf("mem:%ld\n", memGetUsage());
	
	things[0] = memAloc(10);				fillMem(things[0], 10);
	things[1] = memCLoc(8,1);				fillMem(things[1],  8);
	things[2] = memCLoc(8,8);				fillMem(things[2], 64);
	things[3] = memRLoc(NULL, 16);			fillMem(things[3], 16);
	things[4] = memRLoc(things[2], 30);		fillMem(things[4], 30);
	things[5] = memRLoc(&things[2], 30);	fillMem(things[5], 30);
	things[6] = memRLoc(things[5], 16);		fillMem(things[6], 30);

	memRep();

	printf("mem:%ld\n", memGetUsage());

	memFree(things[3]);
	memFree(things[1]);

	printf("mem:%ld\n", memGetUsage());
	
	memFree(0);
	memFree(things[3]);
	
	printf("mem:%ld\n", memGetUsage());

	memFree(things[4]);
	memFree(things[2]);

	this_function_will_give_out_a_free_error_and_it_will_be_big();

	memRep();

	// not required
	memEnd();

	return 0;
}
