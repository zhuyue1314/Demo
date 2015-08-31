/*
 * Author   : Kai Zhao
 * Email    : loverszhao@gmail.com
 * Reference: http://www.linuxhowtos.org/data/6/server.c
 */

/*
 * Description: This is TCP socket client
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>

#include <netinet/in.h>
#include <netdb.h> 
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE       1024

using std::cout;
using std::endl;

static void error(const char *msg)
{
    perror(msg);
    exit(0);
}

static void usage()
{
	cout << endl;
	cout << "\tusage:" << endl;
	cout << "\t\t./tcp_socket_server  ip  port" << endl;
	cout << endl;
}

static void init_server_addr(struct sockaddr_in *server_addr, const char *ip, const char *port)
{
	struct hostent *server = gethostbyname(ip);
	if (server == NULL) {
		cout << "ERROR, no such host:" << ip << endl;
		exit(0);
	}

	bzero((char *) server_addr, sizeof(*server_addr));
	server_addr->sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&(server_addr->sin_addr.s_addr), server->h_length);
	server_addr->sin_port = htons(atoi(port));
}

int main(int argc, char **argv)
{
	cout << "======================================================" << endl;
	cout << "\tThis is a demo project for TCP socket client" << endl;
	cout << "======================================================" << endl;

	if (argc != 3) {
		usage();
		return -1;
	}

	const int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");

	struct sockaddr_in server_addr;
	init_server_addr(&server_addr, argv[1], argv[2]);

	if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
		error("ERROR connecting");

	cout << "[c] Please enter the message which will be sent to the server:" << endl;

	char buffer[BUFFER_SIZE];
	bzero(buffer, sizeof(buffer));
	fgets(buffer, sizeof(buffer), stdin);

	const int nwbytes = write(sockfd, buffer, strlen(buffer));
	if (nwbytes < 0)
		error("ERROR writing to the server");

	bzero(buffer, sizeof(buffer));
	const int nrbytes = read(sockfd, buffer, sizeof(buffer));
	if (nrbytes < 0)
		error("ERROR reading from the server");

	cout << "[c] Get message from the server: " << buffer << endl;

	close(sockfd);

	return 0;
}
