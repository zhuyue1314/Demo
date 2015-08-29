#include <stdio.h>
#include <stdlib.h>

#include <fstream>
#include <iostream>

#include <unistd.h>

using std::cout;
using std::endl;
using std::fstream;

static int write_to_target_fd;
static int read_from_target_fd;

// Execute target in order to init_forkserver
static int init()
{
	int to_target_fd[2]; // fuzzer write, target read
	int to_fuzzer_fd[2]; // target write, fuzzer read

	if (pipe(to_target_fd) == -1 || pipe(to_fuzzer_fd)) {
		perror("[f] pipe() failed");
		return -1;
	}

cout << "[f] to_target_fd[0]=" << to_target_fd[0] << endl;
cout << "[f] to_target_fd[1]=" << to_target_fd[1] << endl;
cout << "[f] to_fuzzer_fd[0]=" << to_fuzzer_fd[0] << endl;
cout << "[f] to_fuzzer_fd[1]=" << to_fuzzer_fd[1] << endl;

	pid_t child_pid = fork();
	char buffer[100];

	if (child_pid == -1) {
		perror("[f] fork() failed");
		return -1;
	}

	if (child_pid == 0) {
		cout << "[t] \tThis is child  process:[" << getpid() << "]" << endl;
		close(to_target_fd[1]);
		close(to_fuzzer_fd[0]);

		char * const argv[] = {
			NULL
		};

		char to_target_buff[100];
		char to_fuzzer_buff[100];

		sprintf(to_target_buff, "TO_TARGET_FD=%d", to_target_fd[0]);
		sprintf(to_fuzzer_buff, "TO_FUZZER_FD=%d", to_fuzzer_fd[1]);

		char * const envp[] = {
			to_target_buff,
			to_fuzzer_buff,
			NULL
		};

		execve("./target", argv, envp);
		return 0;
	}
	close(to_target_fd[0]);
	close(to_fuzzer_fd[1]);
	cout << "[f] \tThis is parent process:[" << getpid() << "]" << endl;

	write_to_target_fd = to_target_fd[1];
	read_from_target_fd = to_fuzzer_fd[0];

	if (read(read_from_target_fd, buffer, sizeof(buffer)) == -1) {
		perror("[f] read() failed");
		return -1;
	}
	cout << "[f] Get message from target: " << buffer << endl;
	cout << "[f] Target is ready" << endl << endl;

	return 0;
}

static int fuzz_next()
{
	static int i;
	fstream fs;

	if (i == 10)
		return 0;

	fs.open("input.txt", fstream::out);
	if (!fs.is_open()) {
		cout << "[f] failed to write to input.txt" << endl;
		return 0;
	}

	fs << i++ << endl;

	fs.close();

	return 1;
}

static int fuzz()
{
	int i = 1;
	char buffer[100];

	while (fuzz_next()) {
		// Run target
		sprintf(buffer, "Start fuzz: %d", i++);
		if (write(write_to_target_fd, buffer, sizeof(buffer)) == -1) {
			perror("[f] write() failed");
			return -1;
		}

		// Target is finished, read the status
		if (read(read_from_target_fd, buffer, sizeof(buffer)) == -1) {
			perror("[f] read() failed");
			return -1;
		}
		cout << "[f] Get status from target: " << buffer << endl << endl;
	}

	// Tell target to exit
	sprintf(buffer, "finish");
	if (write(write_to_target_fd, buffer, sizeof(buffer)) == -1) {
		perror("[f] write() failed");
		return -1;
	}

	// Remove input.txt
	sprintf(buffer, "rm input.txt");
	system(buffer);

	return 0;
}

int main()
{
	cout << "[f] ============================" << endl;
	cout << "[f] \tThis is fuzzer" << endl;
	cout << "[f] ============================" << endl;

	if (init() == -1)
		return -1;

	if (fuzz() == -1)
		return -1;

	sleep(1);

	cout << endl;
	cout << "[f] ============================" << endl;
	cout << "[f] Fuzz done!" << endl;
	cout << "[f] ============================" << endl;

	return 0;
}
