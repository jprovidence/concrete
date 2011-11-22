#include "index.h"

void augment_index(char* url, int idx, int mod_bytes, int mod_bits)
{
	const char* alpha = ALPHA;
}


//-------------------------------------------------------------------------------------------------

// Initialise an empty Plateau into each of the char-keyed index files so they they may be appended
// to later.

void initialize_indicies()
{
	glob_t globbuf;
	char* dir_name = MASTER_URL_DIR;
	char* sorted_files[26];
	Plateau* plateau;
	int arr_size = ARRAYSIZE;
	int i;

	glob(dir_name, GLOB_ERR, NULL, &globbuf);
	trivial_sort(sorted_files, &globbuf);

	for(i = 0; i < arr_size; i++)
	{
		plateau = nplat();
		memdrop_idx(plateau, sorted_files[i]);
	}

	globfree(&globbuf);
}


//-------------------------------------------------------------------------------------------------

// returns a pointer to the initialized Plateau
// --
// allocates memory for and provides default (mostly NULL) values for each of the Plateau fields.

Plateau* nplat()
{
	Plateau* ptr = malloc(sizeof(Plateau));
	const char* alpha = ALPHA;
	int arr_size = ARRAYSIZE;
	int i;

	for(i = 0; i < arr_size; i++)
	{
		ptr->characters[i] = alpha[i];
		ptr->pointers[i] = NULL;
	}

	ptr->file_coords = NULL;
	return ptr;
}


//-------------------------------------------------------------------------------------------------

// @ptr -> a pointer to the plateau whose memory is to be freed
// --
// frees memory previously allocated for the Plateau and nullifies the pointer to that memory block

void kplat(Plateau* ptr)
{
	free(ptr);
	ptr = NULL;
}


//-------------------------------------------------------------------------------------------------

void memlift_idx(char* file)
{
	FILE* f;
	int arr_size = ARRAYSIZE;
	int* idx = NULL;

	(*idx) = 1;
	f = fopen(file, "r");
	lift_idx(f, arr_size, idx); // !! It is imperative index starts at 1
	fclose(f);
}


Plateau* lift_idx(FILE* f, int arr_size, int* index)
{
	int i, null_status, tstore[32];
	Plateau* pplat = NULL;

	for(i = 0; i < arr_size; i++)
	{
		tstore[i] = file_read(f, (*index));
		(*index)++;
	}

	read_set_file_coords(f, pplat, index);

	null_status = are_all_null(tstore, 32);
	if(null_status == 1)
	{
		return pplat;
	}
	else
	{
		for(i = 0; i < arr_size; i++)
		{
			if(tstore[i] == 1)
			{
				pplat->pointers[i] = lift_idx(f, arr_size, index);
			}
		}
	}

	return pplat;
}


int are_all_null(int* ary, int arr_size)
{
	int i;

	for(i = 0; i < arr_size; i++)
	{
		if(ary[i] != 0)
		{
			return 0;
		}
	}

	return 1;
}


void read_set_file_coords(FILE* f, Plateau* pplat, int* index)
{
	int fcsize = FCSIZE;
	int i, j, tstore[(fcsize / 2)];
	uint32_t* ptr = NULL;

	pplat = nplat();

	for(i = 0; i < (fcsize / 2); i++)
	{
		tstore[i] = file_read(f, (*index));
		(*index)++;
	}

	j = 0;
	for(i = ((fcsize / 2) - 1); i >= 0; i--)
	{
		set_nth_32(ptr, i, tstore[j]);
		j++;
	}

	pplat->file_coords->mod_bits = *ptr;

	for(i = 0; i < (fcsize / 2); i++)
	{
		tstore[i] = file_read(f, (*index));
		(*index)++;
	}

	j = 0;
	for(i = ((fcsize / 2) - 1); i >= 0; i--)
	{
		set_nth_32(ptr, i, tstore[j]);
		j++;
	}

	pplat->file_coords->mod_bytes = *ptr;
}


int file_read(FILE* f, int index)
{
	int bsize = sizeof(uint8_t);
	int skips = index / bsize;
	int mod = index % bsize;
	uint8_t* ptr = NULL;

	fseek(f, skips, SEEK_SET);
	fread(ptr, bsize, 1, f);

	return nth_bit_8((*ptr), mod);
}

//-------------------------------------------------------------------------------------------------

// @pplat -> a pointer to the Plateau at the root of the index tree
// @file  -> a pointer to a char[] containing the name of the file to be written into
// --
// delegates the recursive write of the index tree to the database, and coordinates the opening +
// closing of the file.

void memdrop_idx(Plateau* pplat, char* file)
{
	FILE* f;
	int arr_size = ARRAYSIZE;

	f = fopen(file, "w");
	drop_idx(pplat, f, arr_size, 1); // !! It is imperative index starts at 1
	fclose(f);
}


//-------------------------------------------------------------------------------------------------

// @pplat    -> pointer to the Plateau being described to the open file
// @f        -> the file to which the current index tree is being written
// @arr_size -> the number of char-keys in the given Plateau
// @index    -> the location in @f to which the next unit of data should be written
// --
// returns -> the index as it has been advanced by any writes performed within the function
// --
// delegates the writing of the current Plateau to disk, then recursively writes any Plateaus
// pointed to by @pplat
// !! NOTE: It index must begin at 1 !!

int drop_idx(Plateau* pplat, FILE* f, int arr_size, int index)
{
	Plateau* next;
	const char* alpha = ALPHA;
	int i;

	index = write_plateau_signature(pplat, f, arr_size, index);

	for(i = 0; i < arr_size; i++) // write the signatures of subsequent plats from a - z
	{
		next = plateau_jump(pplat, &(alpha[i]));
		if(next != NULL)
		{
			index = drop_idx(next, f, arr_size, index);
		}
	}

	return index;
}


//-------------------------------------------------------------------------------------------------

// @pplat    -> the plateau being described to the open file.
// @f        -> the file the current index tree is being written into
// @arr_size -> the number of char-keys per Plateau
// @index    -> the location in @f to which the next data item should be written to.
// --
// returns -> the index as it has been advanced by any writes performed within the function
// --
// Iterates through each of the char-keys of @pplat. For each one, if the pointer associated with
// the current char-key is NULL, a 0 is written to the file and the index advanced. If it is not
// NULL a 1 is written and the index advanced. @pplat->file_coords is next written to the file, or
// 512 '0' bits if it does not exist.

int write_plateau_signature(Plateau* pplat, FILE* f, int arr_size, int index)
{
	Plateau* next;
	const char* alpha = ALPHA;
	int fcsize = FCSIZE;
	int i, bit;

	for(i = 0; i < arr_size; i++) // write the signature of this plateau
	{
		next = plateau_jump(pplat, &(alpha[i]));

		if(next == NULL)
		{
			file_write(f, index, 0);
		}
		else
		{
			file_write(f, index, 1);
		}

		index++;
	}

	if(pplat->file_coords == NULL)
	{
		for(i = 0; i < fcsize; i++)
		{
			file_write(f, index, 0);
			index++;
		}
	}
	else // write individual bits of file coords to f
	{
		for(i = ((fcsize / 2) - 1); i <= 0; i--)
		{
			bit = nth_bit_32(pplat->file_coords->mod_bits, i);
			file_write(f, index, bit);
		}

		for(i = ((fcsize / 2) - 1); i <= 0; i--)
		{
			bit = nth_bit_32(pplat->file_coords->mod_bytes, i);
			file_write(f, index, bit);
		}
	}

	return index;
}


//-------------------------------------------------------------------------------------------------

// @f     -> the file the current index tree is being written into
// @index -> the location in @f to which the next data item should be written to.
// @bit   -> the bit to be written to the position @index
//--
// Tries to read the byte in which index falls. If it does not exist, it is created. The index-th bit
// is then set via #set_nth_8 and the original byte is written over with the modified version.

void file_write(FILE* f, int index, int bit)
{
	int bsize = sizeof(uint8_t);
	int skips = index / bsize;
	int mod = index % bsize;
	int read_status;
	uint8_t* ptr = NULL;

	fseek(f, skips, SEEK_SET);
	read_status = fread(ptr, bsize, 1, f);

	if(read_status == 0)
	{
		ptr = 0;
		fseek(f, 0, SEEK_END);
		fwrite(ptr, bsize, 1, f);
	}

	set_nth_8(ptr, mod, bit);

	fseek(f, skips, SEEK_SET);
	fwrite(ptr, bsize, 1, f);
}


//-------------------------------------------------------------------------------------------------

// @ptr -> pointer to the byte which will be operated on
// @nth -> the bit to be set
// @bit -> the value (1 || 0) to set @nth to
// --
// if the bit is not already set to the desired value, XOR is used in conjunction with a byte of
// 0s (save for a 1 in the nth position) to flip it

void set_nth_8(uint8_t* ptr, int nth, int bit)
{
	int current = nth_bit_8(*ptr, nth);

	if(current != bit)
	{
		(*ptr) = (*ptr) ^ 1<<nth;
	}
}


void set_nth_32(uint32_t* ptr, int nth, int bit)
{
	int current = nth_bit_32(*ptr, nth);

	if(current != bit)
	{
		(*ptr) = (*ptr) ^ 1<<nth;
	}
}


//-------------------------------------------------------------------------------------------------

// identical to #nth_bit_32, save that @var is uint8_t.

int nth_bit_8(uint8_t var, int nth)
{
	int result = var & 1<<nth;

	if(result == 0)
	{
		return 0;
	}

	return 1;
}


//-------------------------------------------------------------------------------------------------

// @var -> the variable from which to extract a bit
// @nth -> the position of the bit to extract
// --
// returns -> 1 if the bit is 1, 0 otherwise
// --
// since 1 & 0 == 0 && 0 & 0 == 0, given a bitstring of all zeros, save for a 1 at the nth position,
// a logical & will return a non-zero number iff the nth bit is 1

int nth_bit_32(uint32_t var, int nth)
{
	int result = var & 1<<nth;

	if(result == 0)
	{
		return 0;
	}

	return 1;
}


//-------------------------------------------------------------------------------------------------

// @start  -> the Plateau being travelled away from
// @letter -> the char representing the Plateau being travelled to
// --
// returns -> a pointer to the Plateau being travelled to or NULL if it does not exist.
// --
// Iterates through all the char-keys on the given plateau. When one matches @letter, the matching
// index is looked up in @start->pointers. The pointer at this index is returned if it exists.

Plateau* plateau_jump(Plateau* start, const char* letter)
{
	int arr_size = ARRAYSIZE;
	int i;

	for(i = 0; i < arr_size; i++)
	{
		if(start->characters[i] == *letter)
		{
			return start->pointers[i];
		}
	}

	return NULL;
}


//-------------------------------------------------------------------------------------------------

// @url -> the url whose file coordinates are to be looked up in the index
// --
// returns -> file name which store index which has @url's file coordinates
// --
// Allocates memory for the entire file name. After receiving the pivot char from #file_pivot_car
// constructs the full file name in allocated memory. File name = 'pivot' ++ '._ticket'

char* index_file_name(char* url)
{
	char* file_name = malloc(sizeof(char) * 10);
	char terminator[9] = "._ticket";
	char* file_begin = file_pivot_char(url);
	int i;

	for(i = 0; i < 10; i++)
	{
		if(i == 0)
		{
			file_name[i] = *file_begin;
		}
		else
		{
			file_name[i] = terminator[i - 1];
		}
	}

	return file_name;
}


//-------------------------------------------------------------------------------------------------

// @ptr -> a pointer to a file_name "string" returned by a previous call to #index_file_name
// frees all memory allocation in a previous call to #index_file_name

void free_index_file_name(char* ptr)
{
	free(ptr);
	ptr = NULL;
}


//-------------------------------------------------------------------------------------------------

// @url -> the url whose file coordinates are to be looked up in the index
// --
// returns -> the "pivot char" (ie. http://www. --> g <-- oogle.com) this is used to determine
// which file the index assoicated with the given url is stored in.
// --
// this function works by counting the number of '.' chars have been passed. The index of the first
// dot + 1 will yield the "pivot char".

char* file_pivot_char(char* url)
{
	int num_dots = 0, count = 0;

	while(num_dots < 1)
	{
		if(strcmp(&(url[count]), ".") == 0)
		{
			num_dots++;
		}
		count++;
	}

	return &(url[count]);
}

