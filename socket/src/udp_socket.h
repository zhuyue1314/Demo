#ifndef _UDP_SOCKET_H_
#define _UDP_SOCKET_H_

#include <arpa/inet.h>

class UdpSocket {

public:
	UdpSocket();
	~UdpSocket();
	int create_socket();
	int recv();
	int send(const char *buf, const int size);

protected:
	int sockfd;
	struct sockaddr_in addr;
};

#endif
