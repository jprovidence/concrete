#include "concrete.h"

int main(void)
{
	// test node storage

	// INITIALIZE --
	initialize_node();

	printf("SUCCESS: Node initialized \n");
	fflush(stdout);


	// LOAD --
	Node* node = memlift_node("default");

	printf("SUCCESS: Node loaded \n");
	fflush(stdout);


	// ASSIGN --
	*(node->is_placeholder) = 1;
	set_node_url(node, "testpattern");

	printf("ASSIGNED: %s  ||  DESIRED: testpattern\n", node->url);
	fflush(stdout);


	// CONCAT --
	Node* other = nnode();
	*(other->is_placeholder) = 1;
	set_node_url(other, "otherpattern");
	node = node_concat(other, node, 0);

	printf("INITIAL LINKAGE::\n");
	printf("LINKED: First: otherpattern  ||  Second: testpattern\n");
	printf("ACTUAL: First: %s  ||  Second: %s \n", node->url, node->next->url);
	fflush(stdout);


	// DROP --
	memdrop_node(node, "default");

	printf("SUCCESS: Node list dropped\n");


	// RELOAD --
	Node* sec_node = memlift_node("default");

	printf("RELOAD::\n");
	printf("LINKED: First otherpattern  || Second: testpattern\n");
	printf("ACTUAL: First %s  || Second: %s \n", sec_node->url, sec_node->next->url);
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
