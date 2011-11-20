#include <stdio.h>
#include <stdlib.h>
#include "concrete.h"
#include "ladder.h"

int main(void)
{
	// test creation of SoftItem linked list

	SoftItem* ptr;
	SoftItem* psi = link_softs("/home/providence/Dropbox/Uniq/*");
	HardItem* phi;
	int i;

	ptr = psi;
	while(ptr != NULL)
	{
		printf("DIR: %s \n", ptr->disk_location);
		fflush(stdout);
		ptr = ptr->soft_next;
	}

	// test memlift and memlower

	phi = nhard();
	phi->disk_location = "/home/providence/Dropbox/_ticket/c_devel/concrete/test_data/a._ticket";

	for (i = 0; i < 62500; i++)
	{
		phi->midlist[i] = 1;
	}

	psi = memlower(phi);
	phi = memlift(psi);

	printf("Disk Location: %s \n", phi->disk_location);
	fflush(stdout);
	khard(phi);

	exit(EXIT_SUCCESS);
}

