/*
 * haskell_bridge.h
 *
 *  Created on: 2011-12-03
 *      Author: providence
 */

#ifndef HASKELL_BRIDGE_H_
#define HASKELL_BRIDGE_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "spool_matrix.h"


#define INCOMING_PORT 5100;
#define INCOMING_BUFFER_SIZE 62500;


void handle_incoming(Node* node_list, ConcMtx* matx);

void process_arrivals(char* buffer, Node* node_list, ConcMtx* matx);

void hberror();


#endif /* HASKELL_BRIDGE_H_ */
