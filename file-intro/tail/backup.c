#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

const int BUFFER_SZ = 100;

int main(int argc, char *argv[]) {
	char *filename;

	if (argc < 2) {
		fprintf(stderr, "Please pass in a filename");
		exit(-1);
	}	
	
	// get filename from argv
	char *fname = argv[1];

	// num of new lines we encountered so far
	int num_nl = 0;
	// num of new lines to meet before calling it a day
	int targ_nl = 4;

	int fd = open(fname, O_RDONLY);
	int block_end = lseek(fd, 0, SEEK_END);

	// start offset of next block
	int block_start = block_end - BUFFER_SZ;
	if (block_start < 0) {
		block_start = 0;
	}
	int block_sz = block_end - block_start;
	char buffer[BUFFER_SZ];

	// seek to start of the next block
	lseek(fd, block_start, SEEK_SET);
	read(fd, buffer, block_sz);

	printf("%s", buffer);

	// close file before ending program
	close(fd);
}
