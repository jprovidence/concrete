#include "linker.h"

Linker* load(const char* dir_name, Kind k)
{
	Linker* llist = malloc(sizeof(Linker) * 26);
	link(dir_name, k, llist);
	return llist;
}

void unload(Linker* ptr)
{
	free(ptr);
	ptr = NULL;
}

void link(const char* dir_name, Kind k, Linker llist[26])
{
	glob_t globbuf;
	char* sorted[26];

	populate(sorted);
	glob(dir_name, GLOB_ERR, NULL, &globbuf);
	trivial_sort(sorted, &globbuf);
	build_chain(sorted, k, llist);
	globfree(&globbuf);
}

void build_chain(char* sorted[26], Kind k, Linker llist[26])
{
	int i;
	char* cpy_dest;

	for(i = 0; i < 26; i++)
	{
		if (strcmp(sorted[i], "zzzzz") == 0)
		{
			llist[i - 1].next = NULL;
			break;
		}

		if (i > 0)
		{
			llist[i - 1].next = &(llist[i]);
		}

		cpy_dest = malloc(100);
		memmove(cpy_dest, sorted[i], 100);
		llist[i].disk_location = cpy_dest;
		llist[i].kind = k;
	}
}



