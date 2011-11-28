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


	// LINK --
	Node* first_node = nnode();
	char* text = "testurltwo\0";
	uint8_t u = 1;

	set_node_url(first_node, text, 0);
	set_node_placeholder(first_node, u);

	push_node(first_node, node);

	printf("SUCCESS: Nodes linked. \n");
	printf("First node url: %s  ||  Second node url: %s \n", first_node->url, first_node->next->url);
	fflush(stdout);

	memdrop_node(first_node);

	Node* node1 = memlift_node();

	printf("SUCCESS: Linked nodes lifted \n");
	printf("EXPECTED:: First node url: testurltwo  ||  Second node url: testurl \n");
	printf("ACTUAL:: First node url: %s  ||  Second node url: %s \n", node1->url, node1->next->url);

	memdrop_node(node1);


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
	npfc = lookup_index(np_plat, "testpattern", 0);

	printf("Reload INDEX::\n");
	printf("FC, bytes: %d || Desired: 12\n", npfc->mod_bytes);
	printf("FC, bits: %d  || Desired: 14\n", npfc->mod_bits);
	fflush(stdout);

	exit(EXIT_SUCCESS);
}
