/*
 * index.h
 *
 *  Created on: 2011-12-05
 *      Author: providence
 */

#ifndef INDEX_H_
#define INDEX_H_


#include <stdlib.h>
#include <string.h>
#include "matin_structs.h"
#include "matrix.h"


#define INDEX_FILE "/home/providence/Dropbox/_ticket/c_devel/fiber/data/index_store._ticket";


floc* floating_index_lookup(_w_index* idx, char* string);

floc* _floating_index_lookup(_w_index* idx, char* string, int string_len, int position);

void write_to_index(_w_index* idx, char* string, floc* mtx_loc);

void _write_to_index(_w_index* idx, char* string, int string_len, floc* mtx_loc, int position);

int currently_indexed(char c, _w_index* idx, int ilength);

_w_index* new_w_index_level();


#endif /* INDEX_H_ */
