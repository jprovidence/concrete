#include "matrix.h"


void add_to_matrix(matrix* mtx, prototype* proto)
{
	int i, file_pos;
	int dep_size = (sizeof(uint64_t) * proto->relationship_count);
	uint8_t total_size = (uint8_t) (dep_size + sizeof(uint8_t));
	floc* to_index;
	floc* dep_list = map_dependencies_to_matrix(mtx, proto);
	FILE* f = fopen(mtx->store_file, "w+");

	fseek(f, 0, SEEK_END);
	fwrite(&total_size, sizeof(uint8_t), 1, f);

	for(i = 0; i < proto->relationship_count; i++)
	{
		fwrite((dep_list->byte_length), sizeof(uint8_t), 1, f);
		fwrite((dep_list->mod_bytes), sizeof(uint64_t), 1, f);
	}

	file_pos = ftell(f) - total_size;
	to_index = new_floc((uint64_t) file_pos, (uint8_t) total_size);
	write_to_index(mtx->write_index, proto->self, to_index);

	fclose(f);
}

floc* map_dependencies_to_matrix(matrix* mtx, prototype* proto)
{
	int i, file_pos;
	floc* fiber_location, * ret_list;
	uint8_t blank = 0;
	FILE* f;

	f = fopen(mtx->store_file, "w+");
	ret_list = malloc(sizeof(floc) * proto->relationship_count);

	for(i = 0; i < proto->relationship_count; i++)
	{
		fiber_location = floating_index_lookup(mtx->write_index, proto->relationships[i]);

		if(fiber_location == NULL)
		{
			fseek(f, 0, SEEK_END);
			fwrite(&blank, sizeof(uint8_t), 1, f);
			file_pos = ftell(f);

			fiber_location = new_floc((uint64_t) (file_pos - 1), (uint8_t) 1);
		}

		write_to_index(mtx->write_index, proto->relationships[i], fiber_location);

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

	proto->relationship_count++;
	rc = proto->relationship_count;

	if(rc % 5 == 0 || rc == 1)
	{
		new_array = malloc(sizeof(char *) * (rc + 4));

		if(proto->relationship_count != 1)
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
	proto->relationships[rc - 1] = malloc(sizeof(char) * mvlen);
	memmove(proto->relationships[rc - 1], url, sizeof(char) * mvlen);
}

matrix* new_matrix(char* location)
{
	matrix* m = malloc(sizeof(matrix));
	m->store_file = location;
	m->write_index = new_w_index_level();
	return m;
}

prototype* new_prototype(char* url)
{
	prototype* proto = malloc(sizeof(prototype));

	proto->self = malloc(sizeof(char) * strlen(url));
	memmove(proto->self, url, sizeof(char) * strlen(url));

	proto->relationship_count = 0;
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
