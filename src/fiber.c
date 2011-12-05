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
	char* index_file = INDEX_FILE;
	matrix* mtx = new_matrix(index_file);
	prototype* proto = new_prototype("www.test.com/test");

	// Test prototype
	if(strcmp(proto->self, "www.test.com/test") == 0 && proto->relationship_count == 0 && proto->relationships == NULL)
	{
		fprintf(stderr, "_test: prototypes initializing correctly...\n");
	}
	prototype_push_relation(proto, "www.relation1.com/1");
	prototype_push_relation(proto, "www.relation2.com/2");
	prototype_push_relation(proto, "www.relation3.com/3");

	if(strcmp(proto->relationships[0], "www.relation1.com/1") == 0 && strcmp(proto->relationships[2], "www.relation3.com/3") == 0)
	{
		fprintf(stderr, "_test: prototypes pushing correctly...\n");
	}
	if(proto->relationship_count == 3)
	{
		fprintf(stderr, "_test: prototype relationship counter incrementing correctly...\n");
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

	// Test matrix
	add_to_matrix(mtx, proto);
}
