#include "mallocator_utils.h"

#ifdef USER_LAND
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SLEEP usleep
#define PRINT(...) printf(__VA_ARGS__)

static void get_random_bytes(void *vbuf, int nbytes) {
	unsigned char *buf = (unsigned char*)vbuf;
	int i = 0;
	for(;i < nbytes; i++) {
		buf[i] = rand();
	}
}

#else
#include <linux/string.h>
#include <linux/delay.h>
//#include <linux/random.h>
extern void get_random_bytes(void *buf, int nbytes);
#define SLEEP msleep
#define PRINT printk
#define USER_LAND 0
#endif

static MemoryAllocNode* do_alloc(unsigned long size, void* (*alloc_func)(unsigned long), void (*free_func)(void*)) {
	void* ptr = NULL;
	MemoryAllocNode* node = alloc_func(sizeof(MemoryAllocNode));
	if (!node) return NULL;

	memset(node, 0, sizeof(MemoryAllocNode));

	ptr = alloc_func(size);
	if (ptr) {
		get_random_bytes(ptr, size);
		PRINT("allocated ptr=0x%p size=%lu\n", ptr, size);
		node->ptr = ptr;
		node->size = size;
		node->is_kmem = !USER_LAND;
		node->next = NULL;
	} else {
		PRINT("allocation error\n");
		free_func(node);
		node = NULL;
	}
	return node;
}


void mallocator_mfree(struct MemoryAllocNode* node, void (*free_func)(void*)) {
	MemoryAllocNode* current = node;
	MemoryAllocNode* tmp = NULL;
	while(current) {
		if (current->ptr) {
			free_func(current->ptr);
			current->ptr = NULL;
			PRINT("freed %lu bytes\n", current->size);
		}
		tmp = current;
		current = current->next;
		free_func(tmp);
	}
}

MemoryAllocNode* mallocator_mallocate(unsigned long total_size, unsigned long chunk_size, unsigned long millisecond_delay, void* (*alloc_func)(unsigned long), void (*free_func)(void*)) {
	unsigned long ongoing = 0;
	int i = 0;
	MemoryAllocNode *head = NULL;
	MemoryAllocNode *current = NULL;
	MemoryAllocNode *node = NULL;
	while(ongoing < total_size) {
		unsigned long size = chunk_size;
		ongoing += size;
		if (ongoing > total_size) {
			PRINT("last allocation size=%lu total_size=%lu ongoing=%lu\n", size, total_size, ongoing);
			ongoing -= size;
			size = total_size - ongoing;
			ongoing += size;
			PRINT("adjusting last allocation to %lu\n", size);
		} else {
			i++;
		}

		node = do_alloc(size, alloc_func, free_func);
		if (!node) {
			// allocation failed, break out of allocation loop
			break;
		}
		if (!head) {
			head = node;
			current = head;
		} else {
			current->next = node;
			current = node;
		}
		SLEEP(millisecond_delay);
	}

	return head;
}
