#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {
	printf("hello starting execution (pid: %d)\n", (int) getpid());

	int rc = fork();

	if (rc >= 0) {
		printf("running ls (pid: %d)\n", (int) getpid());
		char *args[1];
		args[0] = strdup("/bin/ls");

		execvp(args[0], args);
	}

	if (rc < 0) {
		fprintf(stderr, "Forking failed");
	} else if (rc == 0) {
		int rc_wait = wait(NULL);
		printf("in child process\n");
		printf("Parent process of %d (pid: %d)\n", rc, (int) getpid());
	} else {
		int rc_wait = wait(NULL);
		printf("goodbye\n");
		printf("Parent process of %d (pid: %d)\n", rc, (int) getpid());
	}

	return 0;

}
