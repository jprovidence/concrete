#include "concrete.h"

int main(void)
{
	// test node storage

	// INITIALIZE --
	uint8_t uint;
	uint = 1;

	Node* node = nnode();
	set_node_placeholder(node, uint);

	char* c_ptr;
	c_ptr = "testurl\0";
	set_node_url(node, c_ptr, 0);

	printf("SUCCESS: Node Initialized \n");
	fflush(stdout);


	// DROP --
	memdrop_node(node);
	node = NULL;

	printf("SUCCESS: Node Dropped \n");
	fflush(stdout);


	// LIFT --
	node = memlift_node();

	printf("SUCCESS: Node Lifted \n");
	printf("EXPECTED: node->url :: testurl  || ACTUAL: node->url :: %s \n", node->url);
	printf("EXPECTED: node->placeholder :: 1  || ACTUAL: node->placeholder :: %u \n", *(node->is_placeholder));
	fflush(stdout);

	// LOAD --
	node = memlift_node();
	printf("SUCCESS: Node Loaded \n");
	fflush(stdout);

	// test indicies

	// INITIALIZE --
	initialize_indices();

	printf("SUCCESS: Indices initialized \n");
	fflush(stdout);


	// LOAD --
	Plateau* pplat = load_all_indices();

	printf("SUCCESS: Indices loaded \n");
	fflush(stdout);


	// AUGMENT --
	augment_index(pplat, "testpattern", 0, 12, 14);

	printf("SUCCESS: Index augmented \n");
	fflush(stdout);


	// LOOKUP --
	FileCoords* pfc = lookup_index(pplat, "testpattern", 0);

	printf("Initial INDEX::\n");
	printf("FC, bytes: %d || Desired: 12\n", pfc->mod_bytes);
	printf("FC, bits: %d  || Desired: 14\n", pfc->mod_bits);

	printf("SUCCESS: Index lookup completed\n");
	fflush(stdout);


	// DROP --
	drop_all_indices(pplat);

	printf("SUCCESS: Index drop completed\n");


	// RELOAD --
	Plateau* np_plat;
	FileCoords* npfc;

	np_plat = load_all_indices();
	npfc = lookup_index(pplat, "testpattern", 0);

	printf("Reload INDEX::\n");
	printf("FC, bytes: %d || Desired: 12\n", npfc->mod_bytes);
	printf("FC, bits: %d  || Desired: 14\n", npfc->mod_bits);
	fflush(stdout);


	exit(EXIT_SUCCESS);
}
