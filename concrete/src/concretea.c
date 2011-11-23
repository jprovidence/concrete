#include "concrete.h"

int main(void)
{
	// test dir sorting and linking
/*
	Kind k = binary_list;
	Linker lkr[26], * ptr;
	link("/home/providence/Dropbox/_ticket/c_devel/concretea/test_data/*", k, lkr);

	ptr = lkr;

	while(ptr != NULL)
	{
		printf("DIR: %s \n", ptr->disk_location);
		fflush(stdout);
		ptr = ptr->next;
	}

	Linker * plkr;
	plkr = load("/home/providence/Dropbox/_ticket/c_devel/concretea/test_data/*", k);
	ptr = plkr;

	while(ptr != NULL)
	{
		printf("DIR: %s \n", ptr->disk_location);
		fflush(stdout);
		ptr = ptr->next;
	}

	unload(plkr);


	// test bin list lift and drop

	Blist* pbin = nbin();
	ptr = lkr;
	int i;

	for(i = 0; i < 62500; i++)
	{
		pbin->bit_list[i] = 1;
	}

	pbin->disk_location = "/home/providence/Dropbox/_ticket/c_devel/concretea/test_data/a._ticket";

	dropmem_blist(pbin);
	pbin = liftmem_blist(ptr);
	dropmem_blist(pbin);

	printf("SUCCESS: Drop/Lift of Blist.\n");
	fflush(stdout);


	// test node list lift and drop

	k = node_list;
	Linker lkr_two[26];
	link("/home/providence/Dropbox/_ticket/c_devel/concretea/test_data/*", k, lkr_two);
	Node_list* nlist = nnode_list();
	ptr = lkr_two;
	ptr = ptr->next;

	for(i = 0; i < 1000000; i++)
	{
		nlist->node_list[i] = nnode();
		nlist->node_list[i]->is_placeholder = 0;
		nlist->node_list[i]->size = 1;
		set_url(nlist->node_list[i], "test");
	}

	nlist->disk_location = "/home/providence/Dropbox/_ticket/c_devel/concretea/test_data/b._ticket";

	dropmem_node(nlist);
	nlist = liftmem_node(ptr);
	dropmem_node(nlist);

	printf("SUCCESS: Drop/Lift of node list");

*/
	// test indicies

	initialize_indices();

	printf("SUCCESS: Indices initialized \n");
	fflush(stdout);


	Plateau* pplat = load_all_indices();

	printf("SUCCESS: Indices loaded \n");
	fflush(stdout);


	exit(EXIT_SUCCESS);
}
