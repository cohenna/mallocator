#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/slab.h>

#include "mallocator_utils.h"


static size_t total_alloc_size = 0;
static size_t chunk_size = 256<<20;
static long millisecond_delay = 0;

module_param(total_alloc_size, long, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(total_alloc_size, "Amount of KMEM to allocate in MiB.");
module_param(chunk_size, long, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(chunk_size, "Size in MiB of KMEM chunks to allocate.");
module_param(millisecond_delay, long, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(millisecond_delay, "Delay in ms between KMEM chunk allocations.");

static MemoryAllocNode *node = NULL;


static void* alloc(size_t size) {
	return kmalloc(size, __GFP_NORETRY);
}

static void free_func(void* ptr) {
	return kfree(ptr);
}

static int __init mallocator_init(void)
{
	printk("mallocator module init success\n");
	printk("mallocator module total_alloc_size=%lu chunk_size=%lu millisecond_delay=%lu\n", total_alloc_size, chunk_size, millisecond_delay);
	node = mallocate(total_alloc_size, chunk_size, millisecond_delay, alloc, free_func);
	return 0;
}

static void __exit mallocator_exit(void)
{
	mfree(node, free_func);
	printk(KERN_ALERT "mallocator module cleanup finished\n");
}  

module_init(mallocator_init);
module_exit(mallocator_exit);

MODULE_LICENSE("GPL");
