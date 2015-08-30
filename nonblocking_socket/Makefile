all: filesender udp_socket_server udp_socket_client

filesender: file_sender.cc file_sender.h nonblock_file_sender.cc \
            nonblock_file_sender.h time.h time.cc \
            main.cc
	g++ -g file_sender.cc nonblock_file_sender.cc time.cc \
           main.cc -o filesender

udp_socket_server: udp_socket_server.cc udp_socket_server.h
	g++ udp_socket_server.cc -o udp_socket_server

udp_socket_client: udp_socket_client.cc udp_socket_client.h
	g++ udp_socket_client.cc -o udp_socket_client

clean:
	rm -f filesender
	rm -f udp_socket_server
	rm -f udp_socket_client
