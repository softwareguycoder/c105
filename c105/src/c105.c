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
	struct sockaddr_in server = {0}, client = {0};

	log_info("Calling SocketDemoUtils_createTcpSocket");

	server_fd = SocketDemoUtils_createTcpSocket();

	/* Set up a structure to take the IP address the server is bound to.
	 * Use the AF_INET IP address family in order to tell the system we are
	 * using IPv4 as opposed to v6.
	 */

	log_info("main: Setting up server address...");

	err = SocketDemoUtils_populateAddressInfo(argv[1], "127.0.0.1", &server, &port);

	log_info("main: err = %d", err);

	if (err < 0) {
		fprintf(stderr, "Could not populate the server address information\n");
		print_errno("SocketDemoUtils_populateAddressInfo");
	}

	log_info("main: server.sin_port = %d", server.sin_port);

	log_info("main: server.sin_addr = %lu", server.sin_addr);

	log_info("main: server.sin_addr = '%s'", inet_ntoa(server.sin_addr));

	log_info("main: Binding server socket...");

	/* Associate the server socket with the local machine and the specified port */

	err = SocketDemoUtils_bind(server_fd, &server);

	log_info("main: err = %d", err);

	if (err < 0) {
		fprintf(stderr, "Could not bind server socket.\n");
		print_errno("SocketDemoUtils_bind");
		exit(-1);
	}

	/* Get the dotted-decimal IP address that the server is sitting on, as a string */
	/* This is done for display purposes only */
	char *ip = inet_ntoa(server.sin_addr);

	err = SocketDemoUtils_listen(server_fd, 128);

	log_info("main: err = %d", err);

	if (err < 0) {
		fprintf(stderr, "Could not listen on port %d.\n", port);
		print_errno("SocketDemoUtils_listen");
		exit(-1);
	}

	log_info("main: Server at IP address '%s' listening on port %d...", ip, port);

	while(true) {

		err = SocketDemoUtils_accept(server_fd, &client_fd, &client);
		if (err < 0) {
			fprintf(stderr, "Could not accept new client connection on port %d.", port);
			print_errno("SocketDemoUtils_accept");
			exit(-1);
		}

		if (client_fd < 0) on_error("Could not establish new connection\n");

		log_info("New client connection has been accepted!  Client IP address is %s",
				inet_ntoa(client.sin_addr));

		char* data = NULL;

		while(true){

			err = SocketDemoUtils_recv(client_fd, &data);

			log_info("main: err = %d", err);

			log_info("Checking whether we received valid data...");

			if (data) {
				log_info("Valid data has been received.");

				/* data was received; display it */
				log_info("main: data = '%s'", data);

				/* send the data back to the client */
				err = SocketDemoUtils_send(client_fd, data, strlen(data) + 1);

				log_info("main: err = %d", err);

				if (strcmp(data, ".\n") == 0) {
					/* done communicating with the client */
					/* release system resources associated with the client socket */
					close(client_fd);
					break;
				}

				free(data);
				data = NULL;
			}
		}

	}

	/* release system resources associated with the server socket */
	close(server_fd);

	return 0;
}
