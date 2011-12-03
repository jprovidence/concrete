#include "concrete.h"


int main(int argc, char** argv)
{
	if(strcmp(argv[1], "work") == 0) // work mode
	{
	}
	else if (strcmp(argv[1], "test") == 0) // test mode
	{
		test();
	}

	exit(EXIT_SUCCESS);
}





int test()
{
	// test bit group

	// CREATE --
	BitGroup* bg = nbitgroup();
	BitGroup* nbg = nbitgroup();

	fprintf(stderr, "SUCCESS: BG created \n");


	// SET --
	set_nth_bg(bg, 5, 1);
	set_nth_bg(bg, 2, 1);

	set_nth_bg(nbg, 1, 1);
	set_nth_bg(nbg, 4, 1);

	fprintf(stderr, "SUCCESS: BG set \n");


	// LINK --
	link_bg(bg, nbg);

	if(bg->next == nbg)
	{
		fprintf(stderr, "SUCCESS: BG linked \n");
	}
	else
	{
		fprintf(stderr, "ERROR: BGs LINKED INCORRECTLY \n");
	}


	// READ --
	if(nth_bit_bg(bg, 5) != 1 || nth_bit_bg(nbg, 1) != 1 || nth_bit_bg(nbg, 0) != 0 || nth_bit_bg(bg, 7) != 0)
	{
		fprintf(stderr, "ERROR: UNLINKED BG READING INCORRECTLY \n");
	}
	else
	{
		fprintf(stderr, "SUCCESS: Unlinked BG reads correctly. \n");
	}

	if(nth_bit_bg(bg, 5) != 1 || nth_bit_bg(bg, 9) != 1 || nth_bit_bg(bg, 12) != 1 || nth_bit_bg(bg, 7) != 0 ||
			nth_bit_bg(bg, 3) != 0)
	{
		fprintf(stderr, "ERROR: LINKED BG READING INCORRECTLY \n");
	}
	else
	{
		fprintf(stderr, "SUCCESS: Linked BG reading correctly \n");
	}


	// DROP --
	memdrop_bg(bg, "/home/providence/Dropbox/_ticket/c_devel/concretea/test_data/binlist_store/binlist._ticket");
	bg = NULL;
	nbg = NULL;

	fprintf(stderr, "SUCCESS: BG dropped \n");


	// LIFT --
	BitGroup* lft_bg = memlift_bg("/home/providence/Dropbox/_ticket/c_devel/concretea/test_data/binlist_store/binlist._ticket");

	fprintf(stderr, "SUCCESS: BG lifted \n");


	// POST-LIFT READ --
	if(nth_bit_bg(lft_bg, 5) != 1 || nth_bit_bg(lft_bg, 9) != 1 || nth_bit_bg(lft_bg, 12) != 1 || nth_bit_bg(lft_bg, 7) != 0 ||
			nth_bit_bg(lft_bg, 3) != 0)
	{
		fprintf(stderr, "ERROR: LIFTED BG READING INCORRECTLY \n");
	}
	else
	{
		fprintf(stderr, "SUCCESS: Lifted BG reading correctly \n");
	}


	// RUN-ON SET --
	set_nth_bg(lft_bg, 20, 1);

	if(nth_bit_bg(lft_bg, 20) != 1)
	{
		fprintf(stderr, "ERROR: RUN-ON SETTING NOT FUNCTIONING \n");
	}
	else
	{
		fprintf(stderr, "SUCCESS: Run-on sets functioning \n");
	}


	// COUNT --
	if(length_bg(lft_bg) != 24)
	{
		fprintf(stderr, "ERROR: BG LENGTH INCORRECT \n");
	}
	else
	{
		fprintf(stderr, "SUCCESS: BG has correct length \n");
	}

	memdrop_bg(lft_bg, "/home/providence/Dropbox/_ticket/c_devel/concretea/test_data/binlist_store/binlist._ticket");
	lft_bg = NULL;


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
