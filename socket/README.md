# This is a demo project for socket

The basic knowledge of udp socket

## 1. How to use it

### 1.1 UDP socket

```
$ make
$ ./udp_socket_server 2000
UdpSocketServer::create_socket()
	sockfd=3
UdpSocketServer::bind(port=2000)
	bind() ok
[s] Server is waiting message from client

$ ./udp_socket_client 127.0.0.1 2000
UdpSocketClient::create_socket()
	sockfd=3
UdpSocketClient::connect(dst_ip='127.0.0.1', dst_port=2000)
	connect() ok
[c] Please enter message which will be sent to the server:
this is from client
[c] Client is waiting message from server
```

### 1.2 TCP socket

```
$ make
$ ./tcp_socket_server 2000

```
