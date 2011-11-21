#include "linker.h"

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


void populate(char* sorted[26])
{
	int i;

	for(i = 0; i < 26; i++)
	{
		sorted[i] = STERM;
	}
}

void trivial_sort(char* sorted[26], glob_t* pglob)
{
	int count, icount;
	char* current_sorted, * current_unsorted;

	for(count = 0; count < pglob->gl_pathc; count++)
	{
		for(icount = 0; icount <= count; icount++)
		{
			current_sorted = sorted[icount];
			current_unsorted = pglob->gl_pathv[count];

			if (strcmp(current_sorted, current_unsorted) > 0)
			{
				shift_after(icount, sorted);
				sorted[icount] = current_unsorted;
				break;
			}
		}
	}
}


void shift_after(int index, char* list[26])
{
	int i;

	for (i = 24; i > index; i--)
	{
		list[i + 1] = list[i];
	}
}
