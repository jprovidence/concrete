/*
 * blist.h
 *
 *  Created on: 2011-11-20
 *      Author: providence
 */

#ifndef BLIST_H_
#define BLIST_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "linker.h"


struct Blist
{
	uint16_t bit_list[62500];
	char* disk_location;
};
typedef struct Blist Blist;


Blist* liftmem_blist(Linker* link);

void dropmem_blist(Blist* pbin);

Blist* nbin();

void kbin(Blist* ptr);


#endif /* BLIST_H_ */
