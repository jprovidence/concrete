/*
 * index.h
 *
 *  Created on: 2011-11-21
 *      Author: providence
 */

#ifndef INDEX_H_
#define INDEX_H_


#define FILE_TERMINATOR "._ticket";
#define MASTER_URL_DIR "/home/providence/Dropbox/_ticket/c_devel/concretea/test_data/master_url_index";
#define ARRAYSIZE 26;
#define FCSIZE 64; // number bits in FileCoords
#define ALPHA "abcdefghijklmnopqrstuvwxyz";


#include <stdlib.h>
#include <string.h>
#include <glob.h>
#include "linker.h"

struct FileCoords
{
	uint32_t mod_bytes;
	uint32_t mod_bits;
};
typedef struct FileCoords FileCoords;

struct Plateau
{
	char characters[26];
	struct Plateau* pointers[26];
	struct FileCoords* file_coords;

};
typedef struct Plateau Plateau;


void augment_index(char* url, int idx, int mod_bytes, int mod_bits);

void initialize_indicies();

Plateau* nplat();

void kplat(Plateau* ptr);

void memdrop_idx(Plateau* pplat, char* file);

int drop_idx(Plateau* pplat, FILE* f, int arr_size, int index);

int write_plateau_signature(Plateau* pplat, FILE* f, int arr_size, int index);

int nth_bit_32(uint32_t var, nth);

Plateau* plateau_jump(Plateau* start, const char* letter);

char* index_file_name(char* url);

void free_index_file_name(char* ptr);

char* file_pivot_char(char* url);



#endif /* INDEX_H_ */
