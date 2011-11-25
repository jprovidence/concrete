#ifndef SPOOL_MATRIX_H_
#define SPOOL_MATRIX_H_


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "linker.h"


#define SEPARATOR "..";
#define NODE_DIR "/home/providence/Dropbox/_ticket/c_devel/concretea/test_data/node_store/node_data._ticket";


struct Node
{
	uint8_t* is_placeholder;
	char* url;
	struct Node* next;
};
typedef struct Node Node;

void initialize_node();

void set_node_url(Node* node, char* url);

Node* node_concat(Node* node, Node* list, int position_flag);

Node* memlift_node(char* file_name);

int lift_node(Node* pnode, FILE* f);

void indiv_strcpy(char* dest, char* orig);

void memdrop_node(Node* pnode, char* file_name);

void drop_node(Node* pnode, FILE* f);

Node* nnode();

void knode(Node* node);


#endif /* SPOOL_MATRIX_H_ */
