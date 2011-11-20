#include "ladder.h"


void blevel_iterate()
{

}



HardItem* memlift(SoftItem* psi)
{
	FILE* f;
	int i;
	HardItem* phi;

	f = fopen(psi->disk_location, "r");
	phi = nhard();

	for(i = 0; i < 62500; i++)
	{
		fread(&(phi->midlist[i]), sizeof(uint16_t), 1, f);
	}

	phi->disk_location = psi->disk_location;

	if(psi->soft_next != NULL)
	{
		phi->soft_next = psi->soft_next;
	}

	ksoft(psi);
	return(phi);
}

SoftItem* memlower(HardItem* phi)
{
	FILE* f;
	SoftItem* psi;
	int i;

	f = fopen(phi->disk_location, "w");

	for(i = 0; i < 62500; i++)
	{
		fwrite(&(phi->midlist[i]), sizeof(uint16_t), 1, f);
	}

	psi = nsoft();
	psi->disk_location = phi->disk_location;

	if (phi->soft_next != NULL)
	{
		psi->soft_next = phi->soft_next;
	}

	khard(phi);
	return psi;
}




//-------------------------------------------------------------------------------------------------

// initializes the soft item chain. These are used to stitch together binary lists generated
// on different cycles of the state machine.

SoftItem* link_softs(const char* dir_name)
{
	glob_t globbuf;
	char* sorted[20];

	populate(sorted);
	glob(dir_name, GLOB_ERR, NULL, &globbuf);
	trivial_sort(&globbuf, sorted);
	globfree(&globbuf);

	return ll_link(sorted);
}


//-------------------------------------------------------------------------------------------------

// A simple sort of file names to ensure they are in order. Speed is unnecessary.

void trivial_sort(glob_t* pglob, char* sorted[20])
{
	int count, icount;
	char* current_sorted, * current_unsorted;
	void* cpy_dest;

	count = 0;
	while(count < pglob->gl_pathc)
	{
		current_unsorted = pglob->gl_pathv[count];
		if (strcmp(current_unsorted, ".") == 0 || strcmp(current_unsorted, "..") == 0)
		{
			continue;
		}

		icount = 0;
		while(icount <= count)
		{
			current_sorted = sorted[icount];

			if (strcmp(current_sorted, current_unsorted) > 0)
			{
				shift_after(icount, sorted);
				cpy_dest = (void*) malloc(100);
				memmove(cpy_dest, current_unsorted, 100);
				sorted[icount] = cpy_dest;
			}

			icount ++;
		}
		count++;
	}
}


//-------------------------------------------------------------------------------------------------

// initializes a array [20] of strings (char *) to "zzzzz"

void populate(char* list[20])
{
	int i = 0;

	while(i < 20)
	{
		list[i] = "zzzzz";
		i++;
	}
}


//-------------------------------------------------------------------------------------------------

// moves every item in the given array which falls after @index@ to the right one position.
// used to make room for an insertion.

void shift_after(int index, char* list[20])
{
	int i;

	for(i = 19; i > 0; i--)
	{
		if(i != 19)
		{
			list[i + 1] = list[i];
		}

		if(i == index)
		{
			break;
		}
	}
}


//-------------------------------------------------------------------------------------------------

// Stitches a given list of file names into (SoftItem *)s. SoftItems are chained together blocks
// of disk space. Individual elements of the chain are lifted into memory as they are needed.

SoftItem* ll_link(char* list[20])
{
	SoftItem* tlst[20], * psi;
	int i = 0;

	while(i < 20)
	{
		if(strcmp(list[i], "zzzzz") != 0)
		{
			psi = nsoft();
			psi->disk_location = list[i];
			tlst[i] = psi;
		}
		else
		{
			tlst[i + 1] = NULL;
			break;
		}
		i++;
	}

	i = 0;
	while(i < 20)
	{
		if (tlst[i + 1] == NULL)
		{
			tlst[i]->soft_next = NULL;
			break;
		}
		else
		{
			tlst[i]->soft_next = tlst[i + 1];
		}
		i++;
	}

	return tlst[0];
}


//-------------------------------------------------------------------------------------------------

// allocates and returns a pointer to a memory block containing a SoftItem.

SoftItem* nsoft()
{
	SoftItem* psi = (SoftItem*) malloc(sizeof(SoftItem));
	return psi;
}


//-------------------------------------------------------------------------------------------------

// deallocates and nullifies a pointer to a memory block containing a SoftItem.

void ksoft(SoftItem* ptr)
{
	free(ptr);
	ptr = NULL;
}

HardItem* nhard()
{
	HardItem* phi = (HardItem*) malloc(sizeof(HardItem));
	return phi;
}


void khard(HardItem* ptr)
{
	free(ptr);
	ptr = NULL;
}

