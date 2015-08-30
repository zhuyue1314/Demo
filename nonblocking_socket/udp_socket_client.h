#ifndef _UDP_SOCKET_CLIENT_H_
#define _UDP_SOCKET_CLIENT_H_

#include <arpa/inet.h>

class UdpSocketClient {

public:
	UdpSocketClient();
	~UdpSocketClient();
	int create_socket();
	int connect(const char *dst_ip, const int dst_port);
	int recv();
	int send(const char *buf, const int size);

private:
	int sockfd;
	struct sockaddr_in server_addr;
};

#endif
