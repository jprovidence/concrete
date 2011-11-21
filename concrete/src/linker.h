#ifndef LINKER_H_
#define LINKER_H_


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glob.h>

#define STERM "zzzzz";

enum Kind
{
	binary_list,
	node_list
};
typedef enum Kind Kind;


struct Linker
{
	char* disk_location;
	Kind kind;
	struct Linker* next;
};
typedef struct Linker Linker;


void link(const char* dir_name, Kind k, Linker llist[26]);

void build_chain(char* sorted[26], Kind k, Linker llist[26]);

void populate(char** sorted);

void depopulate(char* sorted[26]);

void trivial_sort(char* sorted[26], glob_t* pglob);

void shift_after(int index, char* list[26]);


#endif /* LINKER_H_ */
