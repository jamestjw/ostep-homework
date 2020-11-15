#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
	printf("hello starting execution (pid: %d)\n", (int) getpid());

	int x = 100;
	int rc = fork();

	if (rc < 0) {
		fprintf(stderr, "Forking failed");
	} else if (rc == 0) {
		printf("child process (pid: %d)\n", (int) getpid());
	} else {
		int rc_wait = wait(NULL);
		printf("Parent process of %d (pid: %d)\n", rc, (int) getpid());
	}

	printf("Initially x is %d (pid: %d) \n", x, (int) getpid());
	x = (int) getpid();
	printf("Now x is %d (pid: %d) \n", x, (int) getpid());

	return 0;

}
