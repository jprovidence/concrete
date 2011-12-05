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

	current_index = currently_indexed(string[position], idx, string_len);

	if(current_index != -1)
	{
		return _floating_index_lookup(idx->progressions[current_index], string, string_len, position + 1);
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
		new_characters = malloc(sizeof(char));
		new_w_indices = malloc(sizeof(_w_index *));
	}
	else
	{
		ichars_len = strlen(idx->characters);
		current_index = currently_indexed(string[position], idx, ichars_len);

		if(current_index == -1)
		{
			new_characters = malloc(sizeof(char) * (ichars_len + 1));
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
