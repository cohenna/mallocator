#ifndef _MALLOCATOR_UTILS_H
#define _MALLOCATOR_UTILS_H

typedef struct MemoryAllocNode {
	void *ptr;
	long size;
	short is_kmem;
	struct MemoryAllocNode *next;
} MemoryAllocNode;


extern MemoryAllocNode* mallocator_mallocate(unsigned long total_size, unsigned long chunk_size, unsigned long millisecond_delay, void* (*alloc_func)(unsigned long), void (*free_func)(void*));
extern void mallocator_mfree(MemoryAllocNode* node, void (*free_func)(void*));

#endif
