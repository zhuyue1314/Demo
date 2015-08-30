#ifndef _FILE_SENDER_H_
#define _FILE_SENDER_H_

class FileSender {

public:
	int read_file_test(const char *file_name);
	int send_file(const char *file_name, int socket);

};

#endif
