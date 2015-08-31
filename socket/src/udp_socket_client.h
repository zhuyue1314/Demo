#ifndef _UDP_SOCKET_CLIENT_H_
#define _UDP_SOCKET_CLIENT_H_

#include "udp_socket.h"

class UdpSocketClient : public UdpSocket {

public:
	int connect(const char *dst_ip, const int dst_port);

private:
	struct sockaddr_in server_addr;
};

#endif
