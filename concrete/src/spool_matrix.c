#include "spool_matrix.h"

void initialize_node()
{
	char* node_dir = NODE_DIR;
	Node* pnode = nnode();
	memdrop_node(pnode, node_dir);
}

void set_node_url(Node* node, char* url)
{
	int len = strlen(url);
	node->url = malloc((sizeof(char) * len) + 1);
	memmove(node->url, url, (sizeof(char) * len));
	node->url[len] = '\0';
}

Node* node_concat(Node* node, Node* list, int position_flag)
{
	if(position_flag == 0)
	{
		node->next = list;
	}
	else if(position_flag == 1)
	{
		Node* copy = list;

		while(list->next != NULL)
		{
			node = list->next;
		}

		list->next = node;
		node = copy;
	}
	return node;
}


Node* memlift_node(char* file_name)
{
	if(strcmp(file_name, "default") == 0)
	{
		file_name = NODE_DIR;
	}

	Node* ret, * pnode;
	FILE* f = fopen(file_name, "r");
	int eof = 1;

	ret = nnode();
	pnode = ret;

	while(eof != 0)
	{
		eof = lift_node(pnode, f);
	}

	return ret;
}

int lift_node(Node* pnode, FILE* f)
{
	int end = 0;
	int i = 0;
	int eof;
	char tstore[200], * term;
	uint8_t* tuint = malloc(sizeof(uint8_t));

	term = ".";
	eof = fread(tuint, sizeof(uint8_t), 1, f);

	while(end == 0 && eof != 0)
	{
		eof = fread(&(tstore[i]), sizeof(char), 1, f);

		if(i > 0 && tstore[i] == *term && tstore[i - 1] == *term)
		{
			tstore[i] = '\0';
			tstore[i - 1] = '\0';

			if(strcmp(pnode->url, "zzz") == 0)
			{
				free(pnode->url);
				pnode->is_placeholder = tuint;
				indiv_strcpy(pnode->url, tstore);
			}
			else
			{
				pnode->next = nnode();
				pnode = pnode->next;

				free(pnode->url);
				pnode->is_placeholder = tuint;
				indiv_strcpy(pnode->url, tstore);
			}

			end = 1;
		}
		else
		{
			i++;
		}
	}
	return eof;
}

void indiv_strcpy(char* dest, char* orig)
{
	int len = strlen(orig);
	dest = malloc(sizeof(char) * len);
	strcpy(dest, orig);
}

void memdrop_node(Node* pnode, char* file_name)
{
	if(strcmp(file_name, "default") == 0)
	{
		file_name = NODE_DIR;
	}

	FILE* f = fopen(file_name, "w");
	Node* old;

	while(pnode != NULL)
	{
		drop_node(pnode, f);
		old = pnode;
		pnode = pnode->next;
		knode(old);
	}

	fclose(f);
}


void drop_node(Node* pnode, FILE* f)
{
	int i = 0;
	char* separator = SEPARATOR;

	fwrite(pnode->is_placeholder, sizeof(uint8_t), 1, f);

	if(pnode->url != NULL)
	{
		while(pnode->url[i] != '\0')
		{
			fwrite(&(pnode->url[i]), sizeof(char), 1, f);
			i++;
		}
	}

	fwrite(separator, (2 * sizeof(char)), 1, f);
}

Node* nnode()
{
	Node* node = malloc(sizeof(Node));

	node->is_placeholder = malloc(sizeof(uint8_t));
	*(node->is_placeholder) = 1;

	node->url = malloc(sizeof(char) * 3);
	strcpy(node->url, "zzz");

	return node;
}

void knode(Node* node)
{
	free(node->is_placeholder);
	node->is_placeholder = NULL;

	if(node->url != NULL)
	{
		free(node->url);
		node->url = NULL;
	}

	free(node);
	node = NULL;
}
