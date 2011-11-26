#include "spool_matrix.h"

void push_node(Node* node, Node* list)
{
	node->next = list;
}

Node* head_node(Node* list)
{
	while(list->prev != NULL)
	{
		list = list->prev;
	}
	return list;
}

void set_node_url(Node* node, char* url, int free_flag)
{
	node->url = malloc(sizeof(char) * strlen(url));
	memmove(node->url, url, sizeof(char) * strlen(url));

	if(free_flag == 1)
	{
		free(url);
		url = NULL;
	}
}

void set_node_placeholder(Node* node, uint8_t uint)
{
	*(node->is_placeholder) = uint;
}

Node* memlift_node()
{
	char* file_name = NODE_FILE;
	FILE* f = fopen(file_name, "r");
	Node* pnode = nnode();
	int read_status;

	while(1)
	{
		read_status = lift_node_head(pnode, f);

		if(read_status == 0)
		{
			break;
		}

		read_status = lift_node_tail(pnode, f);

		pnode->next = nnode();
		pnode->next->prev = pnode;
		pnode = pnode->next;

		if(read_status == 0)
		{
			break;
		}

	}

	pnode = pnode->prev;
	knode(pnode->next);
	pnode->next = NULL;
	fclose(f);

	while(pnode->prev != NULL)
	{
		pnode = pnode->prev;
	}

	return(pnode);
}

int lift_node_head(Node* pnode, FILE* f)
{
	int read_status;
	uint8_t* ptr = malloc(sizeof(uint8_t));

	read_status = fread(ptr, sizeof(uint8_t), 1, f);

	if(read_status != 0)
	{
		memmove(pnode->is_placeholder, ptr, sizeof(uint8_t));
	}

	free(ptr);
	ptr = NULL;
	return read_status;
}

int lift_node_tail(Node* pnode, FILE* f)
{
	int read_status, i, len;
	char tstore[200], * sep;
	sep = SEPARATOR;

	for(i = 0; i < 200; i++)
	{
		read_status = fread(&(tstore[i]), sizeof(char), 1, f);

		if(read_status == 0)
		{
			break;
		}

		if(tstore[i] == *sep)
		{
			tstore[i] = '\0';
			len = strlen(tstore);
			pnode->url = malloc(sizeof(char) * len);
			strcpy(pnode->url, tstore);
		}
	}

	return read_status;
}


void memdrop_node(Node* pnode)
{
	char* file_name = NODE_FILE;
	FILE* f = fopen(file_name, "w+");

	while(1)
	{
		drop_node(pnode, f);

		if(pnode->prev != NULL)
		{
			knode(pnode->prev);
		}

		if(pnode->next == NULL)
		{
			break;
		}
		else
		{
			pnode = pnode->next;
		}
	}

	knode(pnode);
	fclose(f);
}

void drop_node(Node* pnode, FILE* f)
{
	int i, len;
	char* sep = SEPARATOR;

	if(pnode->url != NULL)
	{
		len = strlen(pnode->url);
	}
	else
	{
		len = 0;
	}

	fwrite(pnode->is_placeholder, sizeof(uint8_t), 1, f);

	for(i = 0; i < len; i++)
	{
		fwrite(&(pnode->url[i]), sizeof(char), 1, f);
	}

	fwrite(sep, sizeof(char), 1, f);
}

Node* nnode()
{
	Node* pnode = malloc(sizeof(Node));
	pnode->is_placeholder = malloc(sizeof(uint8_t));
	pnode->url = NULL;
	pnode->next = NULL;
	pnode->prev = NULL;
	return pnode;
}

void knode(Node* pnode)
{
	if(pnode->url != NULL)
	{
		free(pnode->url);
		pnode->url = NULL;
	}

	free(pnode->is_placeholder);
	pnode->is_placeholder = NULL;

	free(pnode);
	pnode = NULL;
}
