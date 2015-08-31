/*
 * Author   : Kai Zhao
 * Email    : loverszhao@gmail.com
 * Reference: http://www.linuxhowtos.org/data/6/server.c
 */

/*
 * Description: This is a simple TCP socket server
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>

#include <netinet/in.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <unistd.h>

#define LISTEN_QUEUE_SIZE 20
#define BUFFER_SIZE       1024

using std::cout;
using std::endl;

static void error(const char *msg)
{
	perror(msg);
	exit(1);
}

static void usage()
{
	cout << endl;
	cout << "\tusage:" << endl;
	cout << "\t\t./tcp_socket_server  port" << endl;
	cout << endl;
}

static void init_server_addr(struct sockaddr_in *server_addr, const char *port)
{
	bzero((char *) server_addr, sizeof(*server_addr));
	server_addr->sin_family = AF_INET;
	server_addr->sin_addr.s_addr = INADDR_ANY;
	server_addr->sin_port = htons(atoi(port));
}

static void talk_with_client(const int client_sockfd)
{
cout << "talk_with_client(" << client_sockfd << ")" << endl;
	char buffer[BUFFER_SIZE];
	bzero(buffer, sizeof(buffer));

	const int nrbytes = read(client_sockfd, buffer, sizeof(buffer));
	if (nrbytes < 0)
		error("ERROR reading from the client");

	cout << "[s][" << getpid() << "] Get message from the client: " << buffer << endl;
	bzero(buffer, sizeof(buffer));

	cout << "[s][" << getpid() <<"] Please enter the message you want to send to the client:" << endl;
	fgets(buffer, sizeof(buffer), stdin);

	const int nwbytes = write(client_sockfd, buffer, strlen(buffer));
	if (nwbytes < 0)
		error("ERROR writing to the client");
}

int main(int argc, char **argv)
{
	cout << "======================================================" << endl;
	cout << "\tThis is a demo project for TCP socket server" << endl;
	cout << "======================================================" << endl;

	if (argc != 2) {
		usage();
		return -1;
	}

	const int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");

	struct sockaddr_in server_addr;
	init_server_addr(&server_addr, argv[1]);

	if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
		error("ERROR on binding");

	if (listen(sockfd, LISTEN_QUEUE_SIZE) < 0)
		error("ERROR on listening");

	socklen_t client_len;
	struct sockaddr_in client_addr;

	cout << "[s] TCP server is accepting" << endl;

	while (1) {

		cout << "[s][" << getpid() << "] is waiting for accept" << endl;
		const int client_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
		if (client_sockfd < 0)
			error("ERROR on accept");

		const int child_pid = fork();
		if (child_pid < 0)
			error("fork() failed");
		else if (child_pid == 0) {
			close(sockfd);
			talk_with_client(client_sockfd);
			close(client_sockfd);
			exit(0);
		}
		close(client_sockfd);
	}
	close(sockfd);

	return 0;
}
