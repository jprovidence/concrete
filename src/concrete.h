#ifndef CONCRETE_H_
#define CONCRETE_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "haskell_bridge.h"
#include "spool_matrix.h"
#include "index.h"


#define ERL_CONNECTION_PORT 1313;
#define ERL_BUFF_SIZE 150000;
#define NODE_NAME "victoria";
#define NODE_INFO "victoria.phub.net.cable.rogers.com";
#define FULL_NODE_NAME "victoria@victoria.phub.net.cable.rogers.com";
#define COOKIE "_ticket";

int main(int argc, char** argv);


int test();


#endif /* CONCRETE_H_ */
