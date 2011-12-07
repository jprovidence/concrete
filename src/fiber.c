#include "fiber.h"

int main(int argc, char** argv)
{
	if(strcmp(argv[1], "work") == 0)
	{

	}
	else if(strcmp(argv[1], "test") == 0)
	{
		test();
	}
	return EXIT_SUCCESS;
}

void test()
{
	char* matrix_file = FIBER_FILE;
	char* test_url = "www.test.com/test";
	matrix* mtx = new_matrix(matrix_file);
	prototype* proto = new_prototype(test_url);

	// Test prototype
	if(strncmp(proto->self, test_url, sizeof(char) * strlen(test_url)) == 0 && *(proto->relationship_count) == 0 && proto->relationships == NULL)
	{
		fprintf(stderr, "_test: prototypes initializing correctly...\n");
	}
	else
	{
		fprintf(stderr, "_test: _ERR_ prototypes not initializing correctly...\n");
	}

	char* rel1_str = "www.relation1.com/1";
	char* rel2_str = "www.relation2.com/2";
	char* rel3_str = "www.relation3.com/3";

	prototype_push_relation(proto, rel1_str);
	prototype_push_relation(proto, rel2_str);
	prototype_push_relation(proto, rel3_str);

	if(strncmp(proto->relationships[0], rel1_str, sizeof(char) * strlen(rel1_str)) == 0)
	{
		if(strncmp(proto->relationships[2], rel3_str, sizeof(char) * strlen(rel3_str)) == 0)
		{
			fprintf(stderr, "_test: prototypes pushing correctly...\n");
		}
		else
		{
			fprintf(stderr, "_test: _ERR_ prototypes not pushing correctly...\n");
		}
	}
	else
	{
		fprintf(stderr, "_test: _ERR_ prototypes not pusing correctly...\n");
	}

	if(*(proto->relationship_count) == 3)
	{
		fprintf(stderr, "_test: prototype relationship counter incrementing correctly...\n");
	}
	else
	{
		fprintf(stderr, "_test: _ERR_ prototype relationship counter not incrementing correctly...\n");
	}


	// Test index
	_w_index* idx = new_w_index_level();
	floc* test_floc1 = new_floc(100, 1);
	floc* test_floc2 = new_floc(200, 2);
	uint8_t test8_1 = 1;
	uint8_t test8_2 = 2;
	uint64_t test64_1 = 100;
	uint64_t test64_2 = 200;

	write_to_index(idx, "index_test_1", test_floc1);
	write_to_index(idx, "index_test_2", test_floc2);

	free_floc(test_floc1);
	free_floc(test_floc2);

	floc* res_floc1 = floating_index_lookup(idx, "index_test_1");
	floc* res_floc2 = floating_index_lookup(idx, "index_test_2");
	floc* res_floc3 = floating_index_lookup(idx, "this_fails");

	if(res_floc3 == NULL && *(res_floc1->mod_bytes) == test64_1 && *(res_floc2->mod_bytes) == test64_2)
	{
		fprintf(stderr, "_test: mod bytes indexing correctly...\n");
	}
	if(*(res_floc1->byte_length) == test8_1 && *(res_floc2->byte_length) == test8_2)
	{
		fprintf(stderr, "_test: byte length indexing correctly...\n");
	}

	free_floc(res_floc1);
	free_floc(res_floc2);
	free_floc(res_floc3);

	// Test matrix
	char* test_url2 = "www.depend.com/test";
	prototype* test_proto1 = new_prototype(test_url2);
	prototype_push_relation(test_proto1, test_url);

	add_to_matrix(mtx, test_proto1);
	add_to_matrix(mtx, proto);
	drop_floating_matrix(mtx);

	matrix* new_matrix = float_matrix();

	Node* node = w_matrix_lookup(new_matrix, test_url);
	Node* node2 = w_matrix_lookup(new_matrix, test_url2);
	Node* node3 = w_matrix_lookup(new_matrix, rel1_str);

	if(node2->relationships[0] == node->self_coordinates && node->relationships[0] == node3->self_coordinates)
	{
		fprintf(stderr, "_test: matrix drop/reloading apparently functioning...\n");
	}
	else
	{
		fprintf(stderr, "_test: _ERR_ matrix drop/realoading not functioning...\n");
	}

	drop_floating_matrix(new_matrix);
}
