#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void write_to_fd(int fd); 

int main(int argc, char *argv[]) {
	printf("hello starting execution (pid: %d)\n", (int) getpid());

	// file descriptor for read only
	int fd = open("./random_text.txt",  O_CREAT|O_WRONLY|O_TRUNC);
	if (fd < 0) {
		perror("Unable to open file descriptor");
		return 1;
	}

	int rc = fork();

	if (rc < 0) {
		fprintf(stderr, "Forking failed");
	} else if (rc == 0) {
		
		printf("child process wtf test(pid: %d)\n", (int) getpid());
		printf("test");
		char *to_write2;
		printf("testi2");
		int r = sprintf(to_write2, "hello from pid child");
		printf("Built to write string with status %d (pid: %d)", r, (int) getpid());
		printf("Starting write to fd: %d (pid: %d)\n", fd, (int) getpid());

		int sz = write(fd, to_write2, strlen(to_write2));

		printf("called write to fd: %d, It returned %d (pid: %d)\n", fd, sz, (int) getpid());
	} else {
		int rc_wait = wait(NULL);
		char *to_write;
		char pid[10];
		sprintf(pid, "%d", (int) getpid());
		sprintf(to_write, "hello from pid %s", pid);
		printf("Starting write to fd: %d (pid: %s)\n", fd, pid);

		int sz = write(fd, to_write, strlen(to_write));

		printf("called write to fd: %d, It returned %d (pid: %s)\n", fd, sz, pid);
		printf("Parent process of %d (pid: %d)\n", rc, (int) getpid());
	}

	return 0;

}

void write_to_fd(int fd) {
	char *to_write;
	char pid[10];
	sprintf(pid, "%d", (int) getpid());
	sprintf(to_write, "hello from pid %s", pid);
	printf("Starting write to fd: %d (pid: %s)\n", fd, pid);

	int sz = write(fd, to_write, strlen(to_write));

	printf("called write to fd: %d, It returned %d (pid: %s)\n", fd, sz, pid);
}
