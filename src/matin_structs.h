/*
 * matin_structs.h
 *
 *  Created on: 2011-12-05
 *      Author: providence
 */

#ifndef MATIN_STRUCTS_H_
#define MATIN_STRUCTS_H_


#include <stdint.h>


struct fiber_coordinates
{
	uint64_t* mod_bytes;
	uint8_t* byte_length;
};
typedef struct fiber_coordinates floc;

struct index_coordinates
{
	uint64_t* mod_bytes;
};
typedef struct index_coordinates iloc;

struct read_index_level
{
	char* characters;
	floc* matrix_location;
	iloc* progressions;
};
typedef struct read_index_level _r_index;

struct write_index_level
{
	char* characters;
	floc* matrix_location;
	struct write_index_level** progressions;
};
typedef struct write_index_level _w_index;

struct node_prototype
{
	char** relationships;
	char* self;
	int* relationship_count;
};
typedef struct node_prototype prototype;

struct node
{
	floc** relationships;
	floc* self_coordinates;
};
typedef struct node Node;

struct fiber_matrix
{
	char* store_file;
	_r_index* read_index;
	_w_index* write_index;
};
typedef struct fiber_matrix matrix;
typedef struct fiber_matrix _matrix;


#endif /* MATIN_STRUCTS_H_ */
