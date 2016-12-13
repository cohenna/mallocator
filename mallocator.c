#include "mallocator_utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>


// 50 MiB
const unsigned long DEFAULT_CHUNK_SIZE = 50<<20;
const useconds_t DEFAULT_SLEEP_TIME = 0;


static void* alloc_func(unsigned long size) {
	return malloc(size);
}

static void free_func(void *ptr) {
	free(ptr);
}


int main(int argc, const char* argv[]) {
	if (argc <= 1 || argc > 5) {
		printf("usage: mallocator TOTAL_ALLOC_SIZE [CHUNK_SIZE=50MiB] [USEC_SLEEP_BETWEEN_CHUNKS=1000000]\n");
		return -1;
	}

	unsigned long total = atoi(argv[1])<<20;
	unsigned long chunk_size = DEFAULT_CHUNK_SIZE;
	unsigned long sleep_time = DEFAULT_SLEEP_TIME;
	unsigned long ongoing = 0;
	MemoryAllocNode* node = NULL;
	MemoryAllocNode* tmp = NULL;

	if (argc > 2) {
		chunk_size = atoi(argv[2])<<20;
	}

	if (argc > 3) {
		sleep_time = atoi(argv[3]);
	}


	printf("total=%lu chunk_size=%lu sleep_time=%lu\n", total, chunk_size, sleep_time);

	

	
	node = mallocator_mallocate(total, chunk_size, sleep_time, alloc_func, free_func);
	
	tmp = node;
	while (tmp) {
		printf("size=%lu ptr=%p\n", tmp->size, tmp->ptr);
		tmp = tmp->next;
	}


	while (1) {
		usleep(sleep_time);
	}

	mallocator_mfree(node, free);

}
