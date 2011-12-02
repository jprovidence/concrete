#include "spool_matrix.h"

BitGroup* memlift_bg(char* file_name)
{
	int read_status = 1;
	BitGroup* pbg = NULL;
	BitGroup* adv_pbg = NULL;
	BitGroup* first;
	uint8_t* p_uint = malloc(sizeof(uint8_t));
	FILE* f = fopen(file_name, "r");

	while(read_status != 0)
	{
		read_status = fread(p_uint, sizeof(uint8_t), 1, f);

		if(read_status == 0)
		{
			break;
		}
		else
		{
			if(pbg == NULL)
			{
				pbg = nbitgroup();
				memmove(&(pbg->byte), p_uint, sizeof(uint8_t));
				first = pbg;
			}
			else
			{
				adv_pbg = nbitgroup();
				pbg->next = adv_pbg;
				adv_pbg->prev = pbg;
				pbg = adv_pbg;

				memmove(&(adv_pbg->byte), p_uint, sizeof(uint8_t));
			}
		}
	}

	free(p_uint);
	p_uint = NULL;
	fclose(f);

	return first;
}

void memdrop_bg(BitGroup* pbg, char* file_name)
{
	int i;
	int bg_size = BGSIZE;
	int index = 0;
	FILE* f = fopen(file_name, "w+");

	while(1)
	{
		for(i = 0; i < bg_size; i++)
		{
			file_write(f, index, nth_bit_8(pbg->byte, i));
			index++;
		}

		if(pbg->next == NULL)
		{
			kbitgroup(pbg);
			break;
		}
		else
		{
			pbg = pbg->next;
			kbitgroup(pbg->prev);
		}
	}

	fclose(f);
}

void link_bg(BitGroup* first, BitGroup* rest)
{
	first->next = rest;
	rest->prev = first;
}

int length_bg(BitGroup* bg)
{
	BitGroup* other = bg;
	int count = 0;

	while(other != NULL)
	{
		other = other->next;
		count++;
	}

	return count * 8;
}

void set_nth_bg(BitGroup* bg, int nth, int bit)
{
	int i;
	int bg_size = BGSIZE;
	int shifts = nth / bg_size;
	int mod = nth % bg_size;
	BitGroup* other = bg;

	for(i = 0; i < shifts; i++)
	{
		if(other->next == NULL)
		{
			other->next = nbitgroup();
			other->next->prev = other;
			other = other->next;
		}
		else
		{
			other = other->next;
		}
	}

	set_nth_8(&(other->byte), mod, bit);
}

void set_adv_current(BitGroup* bg, int nth, int bit)
{
	set_nth_8(&(bg->byte), nth, bit);

	if(nth == 7)
	{
		if(bg->next == NULL)
		{
			bg->next = nbitgroup();
			bg->next->prev = bg;
			bg = bg->next;
		}
		else
		{
			bg = bg->next;
		}
	}
}

int nth_bit_bg(BitGroup* bg, int nth)
{
	int i;
	int bg_size = BGSIZE;
	int shifts = nth / bg_size;
	int mod = nth % bg_size;
	BitGroup* other = bg;

	for(i = 0; i < shifts; i++)
	{
		other = other->next;

		if(other == NULL)
		{
			fprintf(stderr, "Requested Bit Overflows Current BitGroup");
			return 0;
		}
	}

	return nth_bit_8(other->byte, mod);
}

int read_adv_current(BitGroup* bg, int nth)
{
	int bit = nth_bit_8(bg->byte, nth);

	if(nth == 7)
	{
		bg = bg->next;
	}

	return bit;
}

BitGroup* nbitgroup()
{
	BitGroup* bg = malloc(sizeof(BitGroup));
	bg->next = NULL;
	bg->prev = NULL;
	bg->byte = 0;
	return bg;
}

void kbitgroup(BitGroup* bg)
{
	free(bg);
	bg = NULL;
}




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
	node->url = malloc(sizeof(char) * strlen(url) + 1);
	strncpy(node->url, url, sizeof(char) * strlen(url) + 1);

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
			pnode->url = malloc(sizeof(char) * len + 1);
			strncpy(pnode->url, tstore, sizeof(char) * len + 1);
			break;
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
