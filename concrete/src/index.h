/*
 * index.h
 *
 *  Created on: 2011-11-21
 *      Author: providence
 */

#ifndef INDEX_H_
#define INDEX_H_


#define FILE_TERMINATOR "._ticket";
#define MASTER_URL_DIR "/home/providence/Dropbox/_ticket/c_devel/concretea/test_data/master_url_index/*";
#define ARRAYSIZE 26;
#define URLSIZE 200;
#define FCSIZE 64; // number bits in FileCoords
#define ALPHA "abcdefghijklmnopqrstuvwxyz";


#include <stdlib.h>
#include <string.h>
#include <stdint.h>
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
	char* characters;
	struct Plateau** pointers;
	struct FileCoords* file_coords;

};
typedef struct Plateau Plateau;


void augment_index(Plateau* pplat, char* url, int idx, uint32_t mod_bytes, uint32_t mod_bits);

FileCoords* lookup_index(Plateau* pplat, char url[200], int idx);

int is_terminus(char url[200], int idx);

void initialize_indices();

Plateau* load_all_indices();

void drop_all_indices(Plateau* pplat);

Plateau* nplat();

void kplat(Plateau* ptr);

Plateau* memlift_idx(char* file);

Plateau* lift_idx(FILE* f, int arr_size, int* index);

int are_all_null(int* ary, int arr_size);

void read_set_file_coords(FILE* f, Plateau* pplat, int* index);

void file_read(FILE* f, int index, int* ptr);

void memdrop_idx(Plateau* pplat, char* file);

int drop_idx(Plateau* pplat, FILE* f, int arr_size, int index);

int write_plateau_signature(Plateau* pplat, FILE* f, int arr_size, int index);

void file_write(FILE* f, int index, int bit);

void set_nth_8(uint8_t* ptr, int nth, int bit);

void set_nth_32(uint32_t* ptr, int nth, int bit);

int nth_bit_8(uint8_t var, int nth);

int nth_bit_32(uint32_t var, int nth);

Plateau* plateau_jump(Plateau* start, const char* letter);

char* index_file_name(char* url);

void free_index_file_name(char* ptr);

char* file_pivot_char(char* url);



#endif /* INDEX_H_ */
