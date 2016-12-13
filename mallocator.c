#include "mallocator_utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
//#include <sys/kmem.h>
//#include <sys/driver.h>
//#include <driver.h>
//void *kmem_alloc(size_t size, int flag);
//void *kmem_zalloc(size_t size, int flag);
//void kmem_free(void*buf, size_t size);


// 50 MiB
const size_t DEFAULT_CHUNK_SIZE = 50<<20;
// 1 sec
//const useconds_t DEFAULT_SLEEP_TIME = 1000000;
const useconds_t DEFAULT_SLEEP_TIME = 0;


int main(int argc, const char* argv[]) {
	if (argc <= 1 || argc > 5) {
		printf("usage: mallocator TOTAL_ALLOC_SIZE [CHUNK_SIZE=50MiB] [USEC_SLEEP_BETWEEN_CHUNKS=1000000]\n");
		return -1;
	}

	size_t total = atoi(argv[1])<<20;
	size_t chunk_size = DEFAULT_CHUNK_SIZE;
	size_t sleep_time = DEFAULT_SLEEP_TIME;
	size_t ongoing = 0;
	MemoryAllocNode* node = NULL;
	MemoryAllocNode* tmp = NULL;

	if (argc > 2) {
		chunk_size = atoi(argv[2])<<20;
	}

	if (argc > 3) {
		sleep_time = atoi(argv[3]);
	}


	printf("total=%lu chunk_size=%lu sleep_time=%lu\n", total, chunk_size, sleep_time);

	

	
	node = mallocate(total, chunk_size, sleep_time, malloc, free);
	
	tmp = node;
	while (tmp) {
		printf("size=%lu ptr=%p\n", tmp->size, tmp->ptr);
		tmp = tmp->next;
	}

	mfree(node, free);


	while (1) {
		usleep(sleep_time);
	}
}
