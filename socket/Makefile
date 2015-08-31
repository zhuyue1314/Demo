all: udp_socket_server udp_socket_client tcp_socket_server

udp_socket_server: src/udp_socket.cc src/udp_socket.h \
                   src/udp_socket_server.cc src/udp_socket_server.h
	g++ src/udp_socket.cc src/udp_socket_server.cc -o udp_socket_server

udp_socket_client: src/udp_socket.cc src/udp_socket.h \
                   src/udp_socket_client.cc src/udp_socket_client.h
	g++ src/udp_socket.cc src/udp_socket_client.cc -o udp_socket_client

tcp_socket_server: src/tcp_socket_server.cc
	g++ src/tcp_socket_server.cc -o tcp_socket_server

clean:
	rm -f udp_socket_server
	rm -f udp_socket_client
	rm -f tcp_socket_server
