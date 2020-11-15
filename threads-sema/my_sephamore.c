#include "common_threads.h"

typedef struct __Zem_t {
	int value;
	pthread_cond_t cond;
	pthread_mutex_t lock;
} Zem_t;

void Zem_init(Zem_t *zem, int value) {
	zem->value = value;
	Cond_init(&zem->cond);
	Mutex_init(&zem->lock);
}

void Zem_wait(Zem_t *zem) {
	Mutex_lock(&zem->lock);
	while(zem->value <= 0) {
		Cond_wait(&zem->cond, &zem->lock);
	}
	zem->value--;
	Mutex_unlock(&zem->lock);
}

void Zem_post(Zem_t *zem) {
	Mutex_lock(&zem->lock);
	zem->value++;
	Cond_signal(&zem->cond);
	Mutex_unlock(&zem->lock);
}
