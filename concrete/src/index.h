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
	char characters[26];
	struct Plateau* pointers[26];
	struct FileCoords* file_coords;

};
typedef struct Plateau Plateau;


void augment_index(char* url, int idx, int mod_bytes, int mod_bits);

void initialize_indicies();

Plateau* nplat();

void kplat(Plateau* ptr);

void memlift_idx(char* file);

Plateau* lift_idx(FILE* f, int arr_size, int* index);

int are_all_null(int* ary, int arr_size);

void read_set_file_coords(FILE* f, Plateau* pplat, int* index);

int file_read(FILE* f, int index);

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
