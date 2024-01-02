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
	void* things[5];
	
	memStart();

	printf("mem:%ld\n", memGetUsage());
	
	things[0] = memAloc(10);	fillMem(things[0], 10);
	things[1] = memAloc( 8);	fillMem(things[1],  8);
	things[2] = memAloc(16);	fillMem(things[2], 16);
	things[3] = memAloc(30);	fillMem(things[3], 30);
	things[4] = memAloc(64);	fillMem(things[4], 64);

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
