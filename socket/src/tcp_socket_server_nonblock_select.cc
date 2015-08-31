/*
 * Author   : Kai Zhao
 * Email    : loverszhao@gmail.com
 * Reference: http://velep.com/archives/1137.html
 */

/*
 * Description: This is a TCP socket server with nonblocking and select
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>

#include <fcntl.h>
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

static void set_nonblock(const int sockfd)
{
	int flags = fcntl(sockfd, F_GETFL, 0);
	if (flags == -1)
		error("fcntl() failed");

	flags = fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
	if (flags == -1)
		error("fcntl() failed");
}

static void talk_with_client(const int client_sockfd)
{
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

	set_nonblock(sockfd);

	// We only cares read, we need readfds to record the readfds since select
	// will change the readfds
	fd_set readfds, readfds_back;
	struct timeval timeout;
	int maxfd = sockfd;

	FD_ZERO(&readfds);
	FD_SET(sockfd, &readfds);
	readfds_back = readfds;

	while (1) {

		// timeout will be changed after select
		timeout.tv_sec = 2;
		timeout.tv_usec = 0;

		// readfds_back add new connection and remove end connection
		readfds = readfds_back;

		const int ret = select(maxfd + 1, &readfds, NULL, NULL, &timeout);
		if (ret == -1)
			error("select() failed");
		else if (ret == 0) {
			cout << "[s] There is no connection" << endl;
			continue;
		}

		for (int i = 0; i <= maxfd; i++) {
			if (!FD_ISSET(i, &readfds))
				continue;
			if (i == sockfd) {
				// Here comes a new connection
				const int client_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
				if (client_sockfd < 0)
					error("ERROR on accept");

				set_nonblock(client_sockfd);

				// Add to the readfds_back so that we can check whether there is emssage
				// from the client
				FD_SET(client_sockfd, &readfds_back);
				maxfd = maxfd > client_sockfd ? maxfd : client_sockfd;
			} else {
				// Here comes message from the client which we have connected before
				talk_with_client(i);

				close(i);

				// Remove the client_sockfd from readfds_back since it is over and we
				// do not care it anymore
				FD_CLR(i, &readfds_back);
			}
		}

	}
	close(sockfd);

	return 0;
}
