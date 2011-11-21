#include "node.h"

Node_list* liftmem_node(Linker* link)
{
	FILE* f;
	Node_list* pnode;
	int i, j;

	f = fopen(link->disk_location, "r");
	pnode = nnode_list();
	pnode->disk_location = link->disk_location;

	for (i = 0; i < 1000000; i++)
	{
		fread(&(pnode->node_list[i]->is_placeholder), sizeof(uint8_t), 1, f);
		fread(&(pnode->node_list[i]->size), sizeof(uint16_t), 1, f);

		for (j = 0; j < 200; j++)
		{
			fread(&(pnode->node_list[i]->url[j]), sizeof(char), 1, f);
		}
	}

	fclose(f);
	return pnode;
}

void dropmem_node(Node_list* pnode)
{
	FILE* f;
	int i, j;

	f = fopen(pnode->disk_location, "w");

	for(i = 0; i < 1000000; i++)
	{
		fwrite(&(pnode->node_list[i]->is_placeholder), sizeof(uint8_t), 1, f);
		fwrite(&(pnode->node_list[i]->size), sizeof(uint16_t), 1, f);

		for(j = 0; j < 200; j++)
		{
			fwrite(&(pnode->node_list[i]->url[j]), sizeof(char), 1, f);
		}
	}

	knode_list(pnode);
	fclose(f);
}

void set_url(Node* ptr, char url[200])
{
	int i;

	for(i = 0; i < 200; i++)
	{
		ptr->url[i] = url[i];
	}
}

Node* nnode()
{
	Node* ptr = malloc(sizeof(Node));
	return ptr;
}

void knode(Node* ptr)
{
	free(ptr);
	ptr = NULL;
}

Node_list* nnode_list()
{
	Node_list* ptr = malloc(sizeof(Node_list));
	int i;

	for (i = 0; i < 1000000; i++)
	{
		ptr->node_list[i] = nnode();
	}

	return ptr;
}

void knode_list(Node_list* pnode)
{
	int i;

	for(i = 0; i < 1000000; i++)
	{
		knode(pnode->node_list[i]);
	}

	free(pnode);
	pnode = NULL;
}
