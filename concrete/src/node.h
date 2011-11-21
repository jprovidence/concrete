#ifndef NODE_H_
#define NODE_H_


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "linker.h"

struct Node
{
	uint8_t is_placeholder;
	char url[200];
	uint16_t size;
};
typedef struct Node Node;

struct Node_list
{
	Node* node_list[1000000];
	char* disk_location;
};
typedef struct Node_list Node_list;


Node_list* liftmem_node(Linker* link);

void dropmem_node(Node_list* pnode);

void set_url(Node* ptr, char url[200]);

Node* nnode();

void knode(Node* ptr);

Node_list* nnode_list();

void knode_list(Node_list* pnode);

#endif /* NODE_H_ */
