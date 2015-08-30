/*
 * Author   : Kai Zhao
 * Email    : loverszhao@gmail.com
 * Reference: http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html#advanced
 */

/*
 * Description: This file waits 2.5 seconds for something appear on standard input
 */

#include <stdio.h>

#include <iostream>

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

using std::cout;
using std::endl;

#define STDIN 0

int main()
{
	cout << "===================================" << endl;
	cout << "\tThis is a demo project for select" << endl;
	cout << "===================================" << endl;

	struct timeval tv;
	fd_set readfds;

	tv.tv_sec = 2;
	tv.tv_usec = 500 * 1000; // 0.5s

	FD_ZERO(&readfds);
	FD_SET(STDIN, &readfds);


	// Do not care writefds and exceptfds
	const int sret = select(STDIN + 1, &readfds, NULL, NULL, &tv);
	if (sret == -1) {
		perror("select() failed");
		return -1;
	}
	cout << "\tsret=" << sret << endl;

	if (FD_ISSET(STDIN, &readfds))
		cout << "\tA key was pressed" << endl;
	else
		cout << "\tTime out" << endl;

	return 0;
}
