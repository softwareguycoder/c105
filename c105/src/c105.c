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

	int port = 0;

	int server_fd, client_fd, err;
	struct sockaddr_in server, client;

	server_fd = SocketDemoUtils_createTcpSocket();

	/* Set up a structure to take the IP address the server is bound to.
	 * Use the AF_INET IP address family in order to tell the system we are
	 * using IPv4 as opposed to v6.
	 */

	printf("Setting up server address structure...\n");

	SocketDemoUtils_populateAddressInfo(argv[1], "127.0.0.1", server, &port);

	printf("Attempting to bind the server socket to the local machine's IP address and port...\n");

	/* Associate the server socket with the local machine and the specified port */

	err = SocketDemoUtils_bind(server_fd, &server);
	if (err < 0) {
		fprintf(stderr, "Could not bind server socket.\n");
		print_errno("SocketDemoUtils_bind");
		exit(-1);
	}

	/* Get the dotted-decimal IP address that the server is sitting on, as a string */
	/* This is done for display purposes only */
	char *ip = inet_ntoa(server.sin_addr);

	printf("Server at IP address %s listening on port %d...\n", ip, port);

	err = SocketDemoUtils_listen(server_fd, 128);
	if (err < 0) {
		fprintf(stderr, "Could not listen on port %d.\n", port);
		print_errno("SocketDemoUtils_listen");
		exit(-1);
	}

	printf("Server is listening on port %d.\n", port);

	while(true) {
		err = SocketDemoUtils_accept(server_fd, &client_fd, &client);
		if (err < 0) {
			fprintf(stderr, "Could not accept new client connection on port %d.\n", port);
			print_errno("SocketDemoUtils_accept");
			exit(-1);
		}

		if (client_fd < 0) on_error("Could not establish new connection\n");

		printf("New client connection has been accepted!  Client IP address is %s\n",
				inet_ntoa(client.sin_addr));

		char* data = NULL;

		SocketDemoUtils_recv(client_fd, &data);

		if (data) {
			/* data was received; display it */
			fprintf(stdout, "From client: '%s'", data);

			/* send the data back to the client */
			err = SocketDemoUtils_send(client_fd, data, strlen(data) + 1);
		}

		/* done communicating with the client */
		/* release system resources associated with the client socket */
		close(client_fd);
	}

	/* release system resources associated with the server socket */
	close(server_fd);

	return 0;
}
