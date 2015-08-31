/*
 * Author   : Kai Zhao
 * Email    : loverszhao@gmail.com
 * Reference:
 */

/*
 * Description: fork()ed process could not change the static variable of parent process
 *
 */

#include <stdio.h>

#include <iostream>

#include <unistd.h>

using std::cout;
using std::endl;

int main()
{
	cout << "===================================" << endl;
	cout << "\tThis is a demo project" << endl;
	cout << "===================================" << endl;

	static int counter;

	pid_t child_pid = fork();
	if (child_pid < 0) {
		perror("fork() failed");
		return -1;
	} else if (child_pid == 0) {

		cout << "[c]\t\tThis is child process: " << getpid() << endl;
		cout << "[c]\t\tChild process is changing the counter " << endl;
		counter = 111;
		cout << "[c]\t\tcounter= " << counter << endl;

		sleep(2);

		return 0;
	}

	sleep(2);

	cout << "[p]\tThis is parent process: " << getpid() << endl;
	cout << "[p]\tcounter=" << counter << endl;
	cout << endl;
	cout << "***: fork()ed process could not change the static variable of parent process" << endl;
	cout << endl;

	return 0;
}
