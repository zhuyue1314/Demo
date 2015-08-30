#include "nonblock_file_sender.h"

#include <stdio.h>

#include <iostream>

#include <fcntl.h>
#include <unistd.h>

#include "time.h"

#define BUF_SIZE 1024000

using std::cout;
using std::endl;

static int show_flags(const int fd)
{
	int flags = fcntl(fd, F_GETFL);
	if (flags == -1) {
		perror("fcntl() failed");
		return -1;
	}

	cout << "\tshow_flags=" << flags << endl;

	if (flags & O_NONBLOCK)
		cout << "\tnonblock I/O" << endl;
	else
		cout << "\tblock I/O" << endl;

	return 0;
}

int set_nonblock(const int fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1) {
		perror("fcntl() failed");
		return -1;
	}

	return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int NonblockFileSender::read_file_test(const char *file_name)
{
	cout << "NonblockFileSender::send_file(file_name='" << file_name << "')" << endl;

	int fd;
	int nread;
	int nwrite;
	char buf[BUF_SIZE];

	fd = open(file_name, O_RDONLY);
	if (fd == -1) {
		perror("open() failed");
		return -1;
	}

	if (show_flags(fd) == -1)
		return -1;

	if (set_nonblock(fd) == -1)
		return -1;

	if (show_flags(fd) == -1)
		return -1;


	int start = get_current_time();
	nread = read(fd, buf, sizeof(buf));
	int end = get_current_time();

	cout << "\tnread=" << nread << endl;

	cout << "buf[" << nread - 1 << "]=" << (int)buf[nread - 1] << endl;

	cout << "\tnonblock start=" << start << "us" << endl;
	cout << "\tnonblock end  =" << end << "us" << endl;
	cout << "\tread() takes  =" << end - start << "us" << endl;

	if (nread == -1) {
		perror("read() failed");
		return -1;
	} else if (nread == 0) {
		cout << "read() finished" << endl;
		return 0;
	}

	return 0;
}

int NonblockFileSender::send_file(const char *file_name, const int sockfd)
{
	cout << "NonblockFileSender::send_file(file_name='" << file_name << "', sockfd=" << sockfd << ")" << endl;

	int fd;
	int nread;
	int nwrite;
	char buf[BUF_SIZE];

	fd = open(file_name, O_RDONLY);
	if (fd == -1) {
		perror("open() failed");
		return -1;
	}

	if (show_flags(fd) == -1)
		return -1;

	if (set_nonblock(fd) == -1)
		return -1;

	if (show_flags(fd) == -1)
		return -1;


	int start = get_current_time();
	nread = read(fd, buf, sizeof(buf));
	int end = get_current_time();

	cout << "nread=" << nread << endl;
	cout << "\tnonblock start=" << start << "ms" << endl;
	cout << "\tnonblock end  =" << end << "ms" << endl;
	cout << "\tread() takes  =" << end - start << "ms" << endl;

	if (nread == -1) {
		perror("read() failed");
		return -1;
	} else if (nread == 0) {
		cout << "read() finished" << endl;
		return 0;
	}



	return 0;
}
