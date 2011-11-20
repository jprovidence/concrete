#ifndef LADDER_H_
#define LADDER_H_


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <glob.h>


struct HardItem
{
	uint16_t midlist[62500];
	char* disk_location;
	struct SoftItem* soft_next;
	struct HardItem* hard_next;
};
typedef struct HardItem HardItem;

struct SoftItem
{
	char* disk_location;
	struct SoftItem* soft_next;
	struct HardItem* hard_next;
};
typedef struct SoftItem SoftItem;


HardItem* memlift(SoftItem* psi);

SoftItem* memlower(HardItem* phi);

SoftItem* link_softs(const char* dir_name);

void trivial_sort(glob_t* pglob, char* sorted[20]);

void populate(char* list[20]);

void shift_after(int index, char* list[20]);

SoftItem* ll_link(char* list[20]);

SoftItem* nsoft();

void ksoft(SoftItem* ptr);

HardItem* nhard();

void khard(HardItem* ptr);


#endif /* LADDER_H_ */

