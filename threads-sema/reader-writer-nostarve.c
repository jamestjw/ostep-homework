#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "common_threads.h"
#include "my_sephamore.c"
//
// Your code goes in the structure and functions below
//

typedef struct __rwlock_t {
	int num_readers;
	// to provide mutex to readers
	Zem_t mut;
	// to track num writers
	Zem_t sem;
	// service queue, this only works if sephamore signalling has FIFO properties
	Zem_t queue;
} rwlock_t;


void rwlock_init(rwlock_t *rw) {
	rw->num_readers = 0;
	// allow one thread to mutate the num_readers at a time
	Zem_init(&rw->mut, 1);
	// allow one thread to have write access
	Zem_init(&rw->sem, 1);
	Zem_init(&rw->queue, 1);
}

void rwlock_acquire_readlock(rwlock_t *rw) {
	// wait in line to be serviced
	Zem_wait(&rw->queue);
	Zem_wait(&rw->mut);
	rw->num_readers++;
	// if we are the first reader, we need to try to get the write lock
	// so that no one else can write while we are reading!
	if (rw->num_readers == 1) {
		Zem_wait(&rw->sem);
	}
	// let others be serviced
	Zem_post(&rw->queue);
	Zem_post(&rw->mut);
}

void rwlock_release_readlock(rwlock_t *rw) {
	Zem_wait(&rw->mut);
	rw->num_readers--;
	// if we are the last thread to finish reading, we need to 
	// give back control so that the writers can continue writing
	if (rw->num_readers == 0) {
		Zem_post(&rw->sem);
	}
	Zem_post(&rw->mut);
}

void rwlock_acquire_writelock(rwlock_t *rw) {
	Zem_wait(&rw->queue);
	Zem_wait(&rw->sem);
	Zem_post(&rw->queue);
}

void rwlock_release_writelock(rwlock_t *rw) {
	Zem_post(&rw->sem);
}

//
// Don't change the code below (just use it!)
// 

int loops;
int value = 0;

rwlock_t lock;

void *reader(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
	rwlock_acquire_readlock(&lock);
	printf("read %d\n", value);
	rwlock_release_readlock(&lock);
    }
    return NULL;
}

void *writer(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
	rwlock_acquire_writelock(&lock);
	value++;
	printf("write %d\n", value);
	rwlock_release_writelock(&lock);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    assert(argc == 4);
    int num_readers = atoi(argv[1]);
    int num_writers = atoi(argv[2]);
    loops = atoi(argv[3]);

    pthread_t pr[num_readers], pw[num_writers];

    rwlock_init(&lock);

    printf("begin\n");

    int i;
    for (i = 0; i < num_readers; i++)
	Pthread_create(&pr[i], NULL, reader, NULL);
    for (i = 0; i < num_writers; i++)
	Pthread_create(&pw[i], NULL, writer, NULL);

    for (i = 0; i < num_readers; i++)
	Pthread_join(pr[i], NULL);
    for (i = 0; i < num_writers; i++)
	Pthread_join(pw[i], NULL);

    printf("end: value %d\n", value);

    return 0;
}

