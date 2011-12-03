#include "haskell_bridge.h"


void handle_incoming(Node* node_list, ConcMtx* matx)
{
	struct sockaddr_in serv_addr, cli_addr;
	int port = INCOMING_PORT;
	int buffer_size = INCOMING_BUFFER_SIZE;
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	int bind_status, read_status, comm_fd;
	socklen_t cliaddr_len;
	char buff[buffer_size];

	if(sock_fd < 0)
	{
		hberror("Could not create socket");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	bind_status = bind(sock_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
	if(bind_status < 0)
	{
		hberror("Could not bind socket");
	}

	listen(sock_fd, 5);

	cliaddr_len = sizeof(cli_addr);
	comm_fd = accept(sock_fd, (struct sockaddr *) &cli_addr, &cliaddr_len);
	if(comm_fd < 0)
	{
		hberror("Could not accept client");
	}

	memset(buff, 0, sizeof(char) * buffer_size);
	read_status = read(comm_fd, buff, buffer_size);
	if(read_status < 0)
	{
		hberror("Read failed");
	}

	process_arrivals(buff, node_list, matx);
}


void process_arrivals(char* buffer, Node* node_list, ConcMtx* matx)
{

}


void hberror(char* msg)
{
	fprintf(stderr, "Haskell Bridge Error: %s \n", msg);
	exit(EXIT_FAILURE);
}
