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

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0) {
		fprintf(stderr, "Could not create socket\n");
		print_errno("socket");
	}

	/* Set up a structure to take the IP address the server is bound to.
	 * Use the AF_INET IP address family in order to tell the system we are
	 * using IPv4 as opposed to v6.
	 */

	printf("Setting up server address structure...");

	server.sin_family = AF_INET;
	server.sin_port = htons(port);    /* specify the port in network byte-ordering with htons (s for a short int) */
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	/* Make the server socket reusable */

	printf("Setting options to allow the socket to be re-used for a different address later on...\n");

	int opt_val = 1;
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val));

	printf("Attempting to bind the server socket to the local machine's IP address and port...\n");

	/* Associate the server socket with the local machine and the specifed port */

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

		/* prep some storage to hold the text from the client */

		while(1) {
			int read = recv(client_fd, buf, BUFFER_SIZE, 0);

			if (!read) break;		// done reading
			if (read < 0){
				fprintf(stderr, "Client read failed\n");
				print_errno("read");
			}

			for(int i = 0;i < read;i++) {
				/* check if we need to stop */
				if (buf[i] == '\0') {
					break;
				}
			}


			err = send(client_fd, buf, read, 0);
			if (err < 0) on_error("Client write failed\n");
		}

		/* done communicating with the client */
		/* release system resources associated with the client socket */
		close(client_fd);
	}

	/* release system resources associated with the server socket */
	close(server_fd);

	return 0;
}
