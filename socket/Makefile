all: udp_socket_server udp_socket_client \
     tcp_socket_server tcp_socket_server_fork \
     tcp_socket_server_nonblock tcp_socket_server_nonblock_select \
     tcp_socket_client

udp_socket_server: src/udp_socket.cc src/udp_socket.h \
                   src/udp_socket_server.cc src/udp_socket_server.h
	g++ src/udp_socket.cc src/udp_socket_server.cc -o udp_socket_server

udp_socket_client: src/udp_socket.cc src/udp_socket.h \
                   src/udp_socket_client.cc src/udp_socket_client.h
	g++ src/udp_socket.cc src/udp_socket_client.cc -o udp_socket_client

tcp_socket_server: src/tcp_socket_server.cc
	g++ src/tcp_socket_server.cc -o tcp_socket_server

tcp_socket_server_fork: src/tcp_socket_server_fork.cc
	g++ src/tcp_socket_server_fork.cc -o tcp_socket_server_fork

tcp_socket_server_nonblock: src/tcp_socket_server_nonblock.cc
	g++ src/tcp_socket_server_nonblock.cc -o tcp_socket_server_nonblock

tcp_socket_server_nonblock_select: src/tcp_socket_server_nonblock_select.cc
	g++ src/tcp_socket_server_nonblock_select.cc -o tcp_socket_server_nonblock_select

tcp_socket_client: src/tcp_socket_client.cc
	g++ src/tcp_socket_client.cc -o tcp_socket_client

clean:
	rm -f udp_socket_server
	rm -f udp_socket_client
	rm -f tcp_socket_server
	rm -f tcp_socket_server_fork
	rm -f tcp_socket_server_nonblock
	rm -f tcp_socket_server_nonblock_select
	rm -f tcp_socket_client
