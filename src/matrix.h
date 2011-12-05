#ifndef MATRIX_H_
#define MATRIX_H_


#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "matin_structs.h"
#include "index.h"


#define FIBER_FILE "/home/providence/Dropbox/_ticket/c_devel/fiber/data/fiber_store._ticket";
#define WRITE_INDEX_STORE "/home/providence/Dropbox/_ticket/c_devel/fiber/data/write_index_store._ticket";


void add_to_matrix(matrix* mtx, prototype* proto);

floc* map_dependencies_to_matrix(matrix* mtx, prototype* proto);

void prototype_push_relation(prototype* proto, char* url);

matrix* new_matrix(char* location);

prototype* new_prototype(char* url);

floc* new_floc(uint64_t mbytes, uint8_t length);


#endif /* MATRIX_H_ */
