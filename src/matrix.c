#include "matrix.h"


Node* w_matrix_lookup(matrix* mtx, char* string)
{
	int i;
	uint64_t reindex_ptr = 0;
	uint8_t rel_size = 0;
	char* file = FIBER_FILE;
	FILE* f = fopen(file, "r");
	Node* result = new_node();
	floc* node_self = floating_index_lookup(mtx->write_index, string);

	memmove(result->self_coordinates->byte_length, node_self->byte_length, sizeof(uint8_t));
	memmove(result->self_coordinates->mod_bytes, node_self->mod_bytes, sizeof(uint64_t));

	fseek(f, ((int) *(result->self_coordinates->mod_bytes)), SEEK_SET);
	fread(&rel_size, sizeof(uint8_t), 1, f);

	if(rel_size == 0)
	{
		fread(&reindex_ptr, sizeof(uint64_t), 1, f);

		if(reindex_ptr == *(result->self_coordinates->mod_bytes))
		{
			return result;
		}
		else
		{
			fseek(f, (int) reindex_ptr, SEEK_SET);
		}
	}
	else if(rel_size == 1)
	{
		return result;
	}

	result->relationships = malloc(sizeof(floc *) * ((int) rel_size));

	for(i = 0; i < ((int) (rel_size - 1)); i++)
	{
		result->relationships[i] = new_floc(0, 0);
		fread(result->relationships[i]->byte_length, sizeof(uint8_t), 1, f);
		fread(result->relationships[i]->mod_bytes, sizeof(uint64_t), 1, f);
	}

	return result;
}


void add_to_matrix(matrix* mtx, prototype* proto)
{
	int i, file_pos;
	uint64_t file_pos64;
	uint8_t total_size = (uint8_t) (*(proto->relationship_count) + 1);
	floc* to_index, * is_indexed;
	floc* dep_list = map_dependencies_to_matrix(mtx, proto);
	FILE* f = fopen(mtx->store_file, "a+");

	fseek(f, 0, SEEK_END);
	file_pos = (int) ftell(f);
	fwrite(&total_size, sizeof(uint8_t), 1, f);

	for(i = 0; i < *(proto->relationship_count); i++)
	{
		fwrite((dep_list[i].byte_length), sizeof(uint8_t), 1, f);
		fwrite((dep_list[i].mod_bytes), sizeof(uint64_t), 1, f);
	}

	is_indexed = floating_index_lookup(mtx->write_index, proto->self);
	if(is_indexed != NULL)
	{
		file_pos64 = (uint64_t) file_pos;
		fseek(f, (int) is_indexed->mod_bytes + 1, SEEK_SET);
		fwrite(&file_pos64, sizeof(uint64_t), 1, f);
	}

	to_index = new_floc((uint64_t) file_pos, (uint8_t) total_size);
	write_to_index(mtx->write_index, proto->self, to_index);

	free_floc(to_index);
	free(dep_list);
	fclose(f);
}

floc* map_dependencies_to_matrix(matrix* mtx, prototype* proto)
{
	int i;
	floc* fiber_location, * ret_list;
	uint8_t blank = 0;
	uint64_t reindex_ptr = 0;
	FILE* f;

	f = fopen(mtx->store_file, "a+");
	ret_list = malloc(sizeof(floc) * *(proto->relationship_count));

	for(i = 0; i < *(proto->relationship_count); i++)
	{
		fiber_location = floating_index_lookup(mtx->write_index, proto->relationships[i]);

		if(fiber_location == NULL)
		{
			fseek(f, 0, SEEK_END);
			reindex_ptr = (uint64_t) ftell(f);
			fwrite(&blank, sizeof(uint8_t), 1, f);
			fwrite(&reindex_ptr, sizeof(uint64_t), 1, f);

			fiber_location = new_floc(reindex_ptr, (uint8_t) 1);
			write_to_index(mtx->write_index, proto->relationships[i], fiber_location);

			free_floc(fiber_location);
			fiber_location = floating_index_lookup(mtx->write_index, proto->relationships[i]);
		}

		ret_list[i].mod_bytes = fiber_location->mod_bytes;
		ret_list[i].byte_length = fiber_location->byte_length;
	}

	fclose(f);
	return ret_list;
}

void prototype_push_relation(prototype* proto, char* url)
{
	int i, mvlen, rc;
	char** new_array;

	*(proto->relationship_count) += 1;
	rc = *(proto->relationship_count);

	if(rc % 5 == 0 || rc == 1)
	{
		new_array = malloc(sizeof(char *) * (rc + 4));

		if(rc != 1)
		{
			for(i = 0; i < rc - 1; i++)
			{
				mvlen = strlen(proto->relationships[i]);
				new_array[i] = malloc(sizeof(char) * mvlen);
				memmove(new_array[i], proto->relationships[i], sizeof(char) * mvlen);
			}
			free(proto->relationships);
		}

		proto->relationships = new_array;
	}

	mvlen = strlen(url);
	proto->relationships[rc - 1] = malloc(sizeof(char) * mvlen + 1);
	memmove(proto->relationships[rc - 1], url, sizeof(char) * mvlen);
	proto->relationships[rc - 1][mvlen] = '\0';
}

matrix* new_matrix(char* location)
{
	matrix* m = malloc(sizeof(matrix));
	m->store_file = location;
	m->write_index = new_w_index_level();
	return m;
}

matrix* float_matrix()
{
	char* file = FIBER_FILE;
	matrix* mtx = malloc(sizeof(matrix));
	mtx->store_file = file;
	mtx->write_index = float_index();
	return mtx;
}

void drop_floating_matrix(matrix* mtx)
{
	commit_w_index(mtx->write_index);
	free(mtx);
}

prototype* new_prototype(char* url)
{
	prototype* proto = malloc(sizeof(prototype));
	int blank = 0;
	int url_len = strlen(url);

	proto->self = malloc(sizeof(char) * url_len + 1);
	memmove(proto->self, url, sizeof(char) * url_len);
	proto->self[url_len] = '\0';

	proto->relationship_count = malloc(sizeof(int));
	memmove(proto->relationship_count, &blank, sizeof(int));

	proto->relationships = NULL;

	return proto;
}

floc* new_floc(uint64_t mbytes, uint8_t length)
{
	floc* f = malloc(sizeof(floc));

	f->byte_length = malloc(sizeof(uint8_t));
	memmove(f->byte_length, &length, sizeof(uint8_t));

	f->mod_bytes = malloc(sizeof(uint64_t));
	memmove(f->mod_bytes, &mbytes, sizeof(uint64_t));
	return f;
}

void free_floc(floc* f)
{
	if(f != NULL)
	{
		if(f->byte_length != NULL)
		{
			free(f->byte_length);
		}

		if(f->mod_bytes != NULL)
		{
			free(f->mod_bytes);
		}

		free(f);
	}
}

Node* new_node()
{
	Node* n = malloc(sizeof(Node));
	n->relationships = NULL;
	n->self_coordinates = malloc(sizeof(floc));
	n->self_coordinates->byte_length = malloc(sizeof(uint8_t));
	n->self_coordinates->mod_bytes = malloc(sizeof(uint64_t));
	return n;
}
