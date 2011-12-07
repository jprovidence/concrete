#include "index.h"

floc* floating_index_lookup(_w_index* idx, char* string)
{
	return _floating_index_lookup(idx, string, strlen(string), 0);
}

floc* _floating_index_lookup(_w_index* idx, char* string, int string_len, int position)
{
	int current_index;

	if(position == string_len)
	{
		return idx->matrix_location;
	}

	if(idx->characters == NULL)
	{
		return NULL;
	}
	else
	{
		current_index = currently_indexed(string[position], idx, strlen(idx->characters));

		if(current_index != -1)
		{
			return _floating_index_lookup(idx->progressions[current_index], string, string_len, position + 1);
		}
	}

	return NULL;
}

void write_to_index(_w_index* idx, char* string, floc* mtx_loc)
{
	_write_to_index(idx, string, strlen(string), mtx_loc, 0);
}

void _write_to_index(_w_index* idx, char* string, int string_len, floc* mtx_loc, int position)
{
	char* new_characters;
	_w_index** new_w_indices;
	int current_index, ichars_len;

	if(position == string_len)
	{
		memmove(idx->matrix_location->byte_length, mtx_loc->byte_length, sizeof(uint8_t));
		memmove(idx->matrix_location->mod_bytes, mtx_loc->mod_bytes, sizeof(uint64_t));
		return;
	}

	if(idx->characters == NULL)
	{
		ichars_len = 0;
		current_index = -1;
		new_characters = malloc(sizeof(char) + 1);
		new_w_indices = malloc(sizeof(_w_index *));
	}
	else
	{
		ichars_len = strlen(idx->characters);
		current_index = currently_indexed(string[position], idx, ichars_len);

		if(current_index == -1)
		{
			new_characters = malloc(sizeof(char) * (ichars_len + 2));
			new_w_indices = malloc(sizeof(_w_index *) * (ichars_len + 1));

			memmove(new_characters, idx->characters, sizeof(char) * ichars_len);
			memmove(new_w_indices, idx->progressions, sizeof(_w_index *) * ichars_len);

			free(idx->characters);
			free(idx->progressions);
		}
		else
		{
			_write_to_index(idx->progressions[current_index], string, string_len, mtx_loc, position + 1);
			return;
		}
	}

	memmove(&(new_characters[ichars_len]), &(string[position]), sizeof(char));
	new_characters[ichars_len + 1] = '\0';
	new_w_indices[ichars_len] = new_w_index_level();

	idx->characters = new_characters;
	idx->progressions = new_w_indices;

	_write_to_index(idx->progressions[ichars_len], string, string_len, mtx_loc, position + 1);

}


int currently_indexed(char c, _w_index* idx, int ilength)
{
	int i;

	for(i = 0; i < ilength; i++)
	{
		if(c == idx->characters[i])
		{
			return i;
		}
	}

	return -1;
}

_w_index* new_w_index_level()
{
	_w_index* idx = malloc(sizeof(_w_index));
	uint8_t blank = 0;
	uint64_t blank64 = 0;

	idx->characters = NULL;
	idx->progressions = NULL;
	idx->matrix_location = malloc(sizeof(floc));

	idx->matrix_location->byte_length = malloc(sizeof(uint8_t));
	memmove(idx->matrix_location->byte_length, &blank, sizeof(uint8_t));
	idx->matrix_location->mod_bytes = malloc(sizeof(uint64_t));
	memmove(idx->matrix_location->mod_bytes, &blank64, sizeof(uint64_t));

	return idx;
}

void free_w_index_level(_w_index* idx)
{
	if(idx != NULL)
	{
		if(idx->characters != NULL)
		{
			free(idx->characters);
		}
		if(idx->matrix_location != NULL)
		{
			if(idx->matrix_location->byte_length != NULL)
			{
				free(idx->matrix_location->byte_length);
			}
			if(idx->matrix_location->mod_bytes != NULL)
			{
				free(idx->matrix_location->mod_bytes);
			}
			free(idx->matrix_location);
		}
		if(idx->progressions != NULL)
		{
			free(idx->progressions);
		}
		free(idx);
		idx = NULL;
	}
}

void commit_w_index(_w_index* idx)
{
	iloc* loc;
	char* index_file = INDEX_FILE;
	char* header = INDEX_HEADER_FILE;

	FILE* f = fopen(index_file, "w+");
	fseek(f, 0, SEEK_END);
	loc = _commit_w_index(idx, f);
	fclose(f);

	f = fopen(header, "w");
	fwrite(loc->mod_bytes, sizeof(uint64_t), 1, f);
	fclose(f);

	free_iloc(loc);
}

iloc* _commit_w_index(_w_index* idx, FILE* f)
{
	int i, num_chars;
	uint8_t num_chars8;
	iloc** iloc_list;
	iloc* ret_iloc = new_iloc();
	uint8_t blank8 = 0;

	if(idx->progressions != NULL)
	{
		num_chars = strlen(idx->characters);
		num_chars8 = (uint8_t) num_chars;

		iloc_list = malloc(sizeof(iloc *) * num_chars);

		for(i = 0; i < num_chars; i++)
		{
			iloc_list[i] = _commit_w_index(idx->progressions[i], f);
		}

		*(ret_iloc->mod_bytes) = (uint64_t) ftell(f);
		fwrite(&num_chars, sizeof(uint8_t), 1, f);

		for(i = 0; i < num_chars; i++)
		{
			fwrite(&(idx->characters[i]), sizeof(char), 1, f);
			fwrite(iloc_list[i]->mod_bytes, sizeof(uint64_t), 1, f);
			free_iloc(iloc_list[i]);
		}

		free(iloc_list);
	}
	else
	{
		*(ret_iloc->mod_bytes) = ftell(f);
		fwrite(&blank8, sizeof(uint8_t), 1, f);
	}

	fwrite(idx->matrix_location->mod_bytes, sizeof(uint64_t), 1, f);
	fwrite(idx->matrix_location->byte_length, sizeof(uint8_t), 1, f);

	free_w_index_level(idx);
	return ret_iloc;
}

_w_index* float_index()
{
	_w_index* ret;
	uint64_t idx_start;
	iloc* loc = new_iloc();
	char* index_header = INDEX_HEADER_FILE;
	char* index_file = INDEX_FILE;

	FILE* f = fopen(index_header, "r");
	fread(&idx_start, sizeof(uint64_t), 1, f);
	fclose(f);

	memmove(loc->mod_bytes, &idx_start, sizeof(uint64_t));

	f = fopen(index_file, "r");
	ret = _float_index(loc, f);
	fclose(f);

	free_iloc(loc);
	return ret;
}

_w_index* _float_index(iloc* loc, FILE* f)
{
	int i, int_num_chars;
	iloc* iloc_list;
	uint8_t* num_chars = malloc(sizeof(uint8_t));
	_w_index* index = new_w_index_level();

	fseek(f, (int) *(loc->mod_bytes), SEEK_SET);
	fread(num_chars, sizeof(uint8_t), 1, f);
	int_num_chars = (int) *num_chars;

	index->characters = malloc(sizeof(char) * (int_num_chars + 1));
	index->characters[int_num_chars] = '\0';
	index->progressions = malloc(sizeof(_w_index *) * int_num_chars);
	iloc_list = malloc(sizeof(iloc) * int_num_chars);

	for(i = 0; i < int_num_chars; i++)
	{
		fread(&(index->characters[i]), sizeof(char), 1, f);
		iloc_list[i].mod_bytes = malloc(sizeof(uint64_t));
		fread(iloc_list[i].mod_bytes, sizeof(uint64_t), 1, f);
	}

	fread(index->matrix_location->mod_bytes, sizeof(uint64_t), 1, f);
	fread(index->matrix_location->byte_length, sizeof(uint8_t), 1, f);

	for(i = 0; i < int_num_chars; i++)
	{
		index->progressions[i] = _float_index(&(iloc_list[i]), f);
		free(iloc_list[i].mod_bytes);
	}

	free(num_chars);
	free(iloc_list);
	return index;
}

iloc* new_iloc()
{
	iloc* i = malloc(sizeof(iloc));
	i->mod_bytes = malloc(sizeof(uint64_t));
	memset(i->mod_bytes, 0, sizeof(uint64_t));
	return i;
}

void free_iloc(iloc* i)
{
	if(i != NULL)
	{
		if(i->mod_bytes != NULL)
		{
			free(i->mod_bytes);
		}
		free(i);
		i = NULL;
	}
}
