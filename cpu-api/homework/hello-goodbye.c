#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
	printf("hello starting execution (pid: %d)\n", (int) getpid());

	int rc = fork();

	if (rc < 0) {
		fprintf(stderr, "Forking failed");
	} else if (rc == 0) {
		printf("hello\n");
	} else {
		int rc_wait = wait(NULL);
		printf("goodbye\n");
		printf("Parent process of %d (pid: %d)\n", rc, (int) getpid());
	}

	return 0;

}
