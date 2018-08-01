/*
 * c105.c
 *
 *  Created on: Jul 31, 2018
 *      Author: bhart
 */

/*
 * This program implements a simple TCP echo server.
 */

#include "stdafx.h"
#include "c105.h"

int main(int argc, char* argv[]) {
	// Reset the errno value
	errno = 0;

	if (argc < 2) on_error("main: Usage: %s [port]\n", argv[0]);

	int port = atoi(argv[1]);
	if (port <= 1023){
		on_error("Ports 0-1023 are reserved for standard services; please choose a port number greater than 1023.\n");
	}

	if (port > 65535) {
		on_error("Port number may not be greater than 65535.\n");
	}

	int server_fd, client_fd, err;
	struct sockaddr_in server, client;
	char buf[BUFFER_SIZE];

	server_fd = SocketDemoUtils_createTcpSocket();

	/* Set up a structure to take the IP address the server is bound to.
	 * Use the AF_INET IP address family in order to tell the system we are
	 * using IPv4 as opposed to v6.
	 */

	printf("Setting up server address structure...\n");

	SocketDemoUtils_populateAddressInfo(argv[1], "127.0.0.1", server);

	printf("Attempting to bind the server socket to the local machine's IP address and port...\n");

	/* Associate the server socket with the local machine and the specified port */

	err = bind(server_fd, (struct sockaddr*)&server, sizeof(server));
	if (err < 0){
		fprintf(stderr, "Could not bind socket\n");
		print_errno("bind");
	}

	/* Get the dotted-decimal IP address that the server is sitting on, as a string */
	/* This is done for display purposes only */
	char *ip = inet_ntoa(server.sin_addr);

	printf("Server at IP address %s listening on port %d...\n", ip, port);

	err = listen(server_fd, 128 /* connection backlog */);

	printf("Server is listening on port %d.\n", port);

	while(1) {
		socklen_t client_len = sizeof(client);
		client_fd = accept(server_fd, (struct sockaddr*) &client, &client_len);

		if (client_fd < 0) on_error("Could not establish new connection\n");

		printf("New client connection has been accepted!  Client IP address is %s\n",
				inet_ntoa(client.sin_addr));

		char *buffer = (char*)calloc(BUFFER_SIZE, sizeof(char));
		char *current_chunk = (char*)calloc(BUFFER_SIZE, sizeof(char));

		while(1) {
			int read = recv(client_fd, buffer, BUFFER_SIZE, 0);

			if (read < 0) break;

		}

		/* done communicating with the client */
		/* release system resources associated with the client socket */
		close(client_fd);
	}

	/* release system resources associated with the server socket */
	close(server_fd);

	return 0;
}
