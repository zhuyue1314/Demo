#include "file_sender.h"

#include <stdio.h>

#include <iostream>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "time.h"

#define BUF_SIZE 1024000

using std::cout;
using std::endl;

int FileSender::read_file_test(const char *file_name)
{
	int fd;
	int nread;
	char buf[BUF_SIZE];

	cout << "FileSender::read_file_test(file_name='" << file_name << "')" << endl;

	fd = open(file_name, O_RDONLY);
	if (fd == -1) {
		perror("open() failed");
		return -1;
	}

	int start = get_current_time();
	nread = read(fd, buf, sizeof(buf));
	int end = get_current_time();

	cout << "\tnread=" << nread << endl;

	cout << "buf[" << nread - 1 << "]=" << (int)buf[nread - 1] << endl;

	cout << "\tblock  start =" << start << "us" << endl;
	cout << "\tblock  end   =" << end << "us" << endl;
	cout << "\tread() takes =" << end - start << "us" << endl;

	if (nread == -1) {
		perror("read() failed");
		return -1;
	}

	close(fd);
	return 0;
}

int FileSender::send_file(const char *file_name, const int sockfd)
{
	int fd;
	int nread;
	int nwrite;
	char buf[BUF_SIZE];

	cout << "\tsend_file(file_name='" << file_name << "', sockfd=" << sockfd << ")" << endl;

	fd = open(file_name, O_RDONLY);
	if (fd == -1) {
		perror("open() failed");
		return -1;
	}

	while (1) {
		nread = read(fd, buf, sizeof(buf));
		if (nread == -1) {
			perror("read() failed");
			break;
		} else if (nread == 0) {
			cout << "read() finish" << endl;
			break;
		}

		cout << "nread=" << nread << endl;
		cout << "buf = ";
		for (int i = 0; i < nread; i++) {
			printf("%02X ", (unsigned char)buf[i]);
		}
		cout << "||||" << endl;

		cout << endl;
		nwrite = write(sockfd, buf, nread);
		if (nwrite == -1) {
			perror("write() failed");
			break;
		}
		cout << endl;
	}

	close(fd);
	return 0;
}
