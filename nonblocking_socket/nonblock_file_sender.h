#ifndef _NONBLOCK_FILE_SENDER_H_
#define _NONBLOCK_FILE_SENDER_H_

class NonblockFileSender {

public:
	int read_file_test(const char *file_name);
	int send_file(const char *file_name, const int sockfd);

};

#endif
