#include <stdio.h>
#include <stdlib.h>

#include <fstream>
#include <iostream>

#include "file_sender.h"
#include "nonblock_file_sender.h"

using std::cout;
using std::endl;
using std::fstream;

#define FILENAME "input.txt"

static int create_file()
{
	fstream fs;

	fs.open(FILENAME, fstream::out);	

	if (!fs.is_open()) {
		perror("failed to open()");
		return -1;
	}

	for (int i = 0; i < 2000000; i++) {
		fs << i << endl;
	}

	fs.close();
	return 0;
}

static int test_file_read()
{
	cout << endl;
	cout << "===========================" << endl;
	cout << "\tfile sender" << endl;
	cout << "===========================" << endl;
	cout << endl;

	FileSender fsender;
	fsender.read_file_test(FILENAME);

	return 0;
}

static int test_nonblock_file_read()
{
	cout << endl;
	cout << "=====================================" << endl;
	cout << "\t nonblock file sender" << endl;
	cout << "=====================================" << endl;
	cout << endl;

	NonblockFileSender nbfsender;
	nbfsender.read_file_test(FILENAME);

	return 0;
}

static int test_file_sender()
{
	cout << endl;
	cout << "===========================" << endl;
	cout << "\tfile sender" << endl;
	cout << "===========================" << endl;
	cout << endl;

	FileSender fsender;

	int sockfd = fileno(stdout);
	if (sockfd == -1) {
		perror("fileno() failed");
		return -1;
	}

	fsender.send_file(FILENAME, sockfd);

	return 0;
}

static int test_nonblock_file_sender()
{
	cout << endl;
	cout << "=====================================" << endl;
	cout << "\t nonblock file sender" << endl;
	cout << "=====================================" << endl;
	cout << endl;

	NonblockFileSender nbfsender;

	int sockfd = fileno(stdout);
	if (sockfd == -1) {
		perror("fileno() failed");
		return -1;
	}

	nbfsender.send_file(FILENAME, sockfd);

	return 0;
}

static void remove_file()
{
	char cmd[30];
	sprintf(cmd, "rm -f %s", FILENAME);

	system(cmd);
}

static void test_udp_socket()
{
}

int main()
{
	test_udp_socket();

/*
	if (create_file() == -1)
		return -1;

	if (test_file_read() == -1)
		return -1;

	if (test_nonblock_file_read() == -1)
		return -1;


	if (test_file_sender() == -1)
		return -1;

	if (test_nonblock_file_sender() == -1)
		return -1;


	remove_file();
*/

	return 0;
}
