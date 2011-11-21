#include "blist.h"

Blist* liftmem_blist(Linker* link)
{
	FILE* f;
	Blist* pbin;
	int i;

	f = fopen(link->disk_location, "r");
	pbin = nbin();
	pbin->disk_location = link->disk_location;

	for (i = 0; i < 62500; i++)
	{
		fread(&(pbin->bit_list[i]), sizeof(uint16_t), 1, f);
	}

	fclose(f);
	return pbin;
}


void dropmem_blist(Blist* pbin)
{
	FILE* f;
	int i;

	f = fopen(pbin->disk_location, "w");

	for(i = 0; i < 62500; i++)
	{
		fwrite(&(pbin->bit_list[i]), sizeof(uint16_t), 1, f);
	}

	kbin(pbin);
	fclose(f);
}


Blist* nbin()
{
	Blist* ptr = malloc(sizeof(Blist));
	return ptr;
}

void kbin(Blist* ptr)
{
	free(ptr);
	ptr = NULL;
}
