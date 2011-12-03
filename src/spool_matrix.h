#ifndef SPOOL_MATRIX_H_
#define SPOOL_MATRIX_H_


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "index.h"


#define NODE_FILE "/home/providence/Dropbox/_ticket/c_devel/concretea/test_data/node_store/node_data._ticket";
#define SEPARATOR ",";
#define BGSIZE 8;


struct BitGroup
{
	uint8_t byte;
	struct BitGroup* next;
	struct BitGroup* prev;
};
typedef struct BitGroup BitGroup;


struct Node
{
	uint8_t* is_placeholder;
	char* url;
	struct Node* next;
	struct Node* prev;
};
typedef struct Node Node;


struct ConcMtx
{
	Node* node_list;
	BitGroup** matrix;
};
typedef struct ConcMtx ConcMtx;


BitGroup* memlift_bg(char* file_name);

void memdrop_bg(BitGroup* pbg, char* file_name);

void link_bg(BitGroup* first, BitGroup* rest);

int length_bg(BitGroup* bg);

void set_nth_bg(BitGroup* bg, int nth, int bit);

void set_adv_current(BitGroup* bg, int nth, int bit);

int nth_bit_bg(BitGroup* bg, int nth);

int read_adv_current(BitGroup* bg, int nth);

BitGroup* nbitgroup();

void kbitgroup(BitGroup* bg);

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
