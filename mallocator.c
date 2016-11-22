#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

// 50 MiB
const size_t DEFAULT_CHUNK_SIZE = 50<<20;
// 1 sec
//const useconds_t DEFAULT_SLEEP_TIME = 1000000;
const useconds_t DEFAULT_SLEEP_TIME = 0;


int main(int argc, const char* argv[]) {
	int i = 0;

	if (argc <= 1 || argc > 5) {
		printf("usage: mallocator TOTAL_ALLOC_SIZE [CHUNK_SIZE=50MiB] [USEC_SLEEP_BETWEEN_CHUNKS=1000000]\n");
		return -1;
	}

	size_t total = atoi(argv[1])<<20;
	size_t chunk_size = DEFAULT_CHUNK_SIZE;
	size_t sleep_time = DEFAULT_SLEEP_TIME;
	size_t ongoing = 0;

	if (argc > 2) {
		chunk_size = atoi(argv[2])<<20;
	}

	if (argc > 3) {
		sleep_time = atoi(argv[3]);
	}


	printf("total=%lu chunk_size=%lu sleep_time=%lu\n", total, chunk_size, sleep_time);

	

	while(ongoing < total) {
		size_t size = chunk_size;
		ongoing += size;
		if (ongoing > total) {
			printf("last allocation size=%lu total=%lu ongoing=%lu\n", size, total, ongoing);
			ongoing -= size;
			size = total - ongoing;
			ongoing += size;
			printf("adjusting last allocation to %lu\n", size);
		} else {
			i++;
		}
		void* ptr = malloc(size);
		memset(ptr, i&0xffff, size);
		printf("allocated ptr=0x%p size=%lu, total MiB=%lu\n", ptr, size, ongoing>>20);
		usleep(sleep_time);
	}

	
	while (1) {
		usleep(sleep_time);
	}
}
