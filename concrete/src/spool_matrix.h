#ifndef SPOOL_MATRIX_H_
#define SPOOL_MATRIX_H_


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "linker.h"


#define NODE_FILE "/home/providence/Dropbox/_ticket/c_devel/concretea/test_data/node_store/node_data._ticket";
#define SEPARATOR ",";


struct Node
{
	uint8_t* is_placeholder;
	char* url;
	struct Node* next;
	struct Node* prev;
};
typedef struct Node Node;


void push_node(Node* node, Node* list);

Node* head_node(Node* list);

void set_node_url(Node* node, char* url, int free_flag);

void set_node_placeholder(Node* node, uint8_t ptr);

Node* memlift_node();

int lift_node_head(Node* pnode, FILE* f);

int lift_node_tail(Node* pnode, FILE* f);

void memdrop_node(Node* pnode);

void drop_node(Node* pnode, FILE* f);

Node* nnode();

void knode(Node* pnode);

#endif /* SPOOL_MATRIX_H_ */
