#include <assert.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fstream>
#include <iostream>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using std::cout;
using std::endl;
using std::fstream;

void init_forkserver()
{
	char *to_target_env = getenv("TO_TARGET_FD");
	char *to_fuzzer_env = getenv("TO_FUZZER_FD");

	const int to_target_fd = atoi(to_target_env);
	const int to_fuzzer_fd = atoi(to_fuzzer_env);

	cout << "<t> to_target_fd=" << to_target_fd << endl;
	cout << "<t> to_fuzzer_fd=" << to_fuzzer_fd << endl;

	char buffer[100];

	sprintf(buffer, "%s", "This message is from target");

	if (write(to_fuzzer_fd, buffer, sizeof(buffer)) == -1) {
		perror("<t> write() failed");
		return;
	}

	int child_pid, status;

	while (true) {
		if (read(to_target_fd, buffer, sizeof(buffer)) == -1) {
			perror("<t> read() failed");
			return;
		}
		cout << "<t> Get message from fuzzer: " << buffer << endl;
		if (!strcmp(buffer, "finish")) {
			cout << "<t> Target forkserver going to exit" << endl;
			exit(0);
		}

		child_pid = fork();

		if (child_pid == -1) {
			perror("<t> fork() failed");
			return;
		}

		if (child_pid == 0) {
			return;
		}

		if (waitpid(child_pid, &status, 0) == -1) {
			perror("<t> waitpid() failed");
			return;
		}

		sprintf(buffer, "%d", status);
		if (write(to_fuzzer_fd, buffer, sizeof(buffer)) == -1) {
			perror("<t> write() failed");
			return;
		}
	}
}

void open_file()
{
	fstream fs;

	fs.open("input.txt", fstream::in);

	if (!fs.is_open()) {
		perror("<t> failed to open input.txt");
		return;
	}

	int i;
	fs >> i;

	cout << "<t> content= " << i << endl;

	if (i % 3 == 1)
		assert(false); // Introduce a bug

	fs.close();
}

int main()
{
	cout << "<t> ****************************" << endl;
	cout << "<t> \tThis is target" << endl;
	cout << "<t> ****************************" << endl;

	init_forkserver();

	open_file();

	return 0;
}
