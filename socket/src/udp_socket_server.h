#ifndef _UDP_SOCKET_SERVER_H_
#define _UDP_SOCKET_SERVER_H_

#include "udp_socket.h"

class UdpSocketServer : public UdpSocket {

public:
	int bind(const int port);
};

#endif
