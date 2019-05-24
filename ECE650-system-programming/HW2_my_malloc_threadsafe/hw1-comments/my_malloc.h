#ifndef _MY_MALLOC_H
#define _MY_MALLOC_H

// include unistd for sbrk()
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>

/* HW1 deprecated funcs */

//First Fit malloc/free
// void* ff_malloc(size_t size);
// void ff_free(void *ptr);

//Best Fit malloc/free
// void* bf_malloc(size_t size);
// void bf_free(void *ptr);

/* HW2 funcs */

//Thread Safe malloc/free: locking version
void *ts_malloc_lock(size_t size);
void ts_free_lock(void *ptr);

//Thread Safe malloc/free: non-locking version
void *ts_malloc_nolock(size_t size);
void ts_free_nolock(void *ptr);

unsigned long get_data_segment_size();  // in bytes
unsigned long get_data_segment_free_space_size();  // in bytes

struct _metadata_linked_list {
	size_t size;
	bool is_free;
	struct _metadata_linked_list* prev;
	struct _metadata_linked_list* next;
	struct _metadata_linked_list* free_prev;
	struct _metadata_linked_list* free_next;
	void* check_ptr;  // add a sentinel to check if malloc'd by us
	void* start_ptr;  // the starting addr of data
};
typedef struct _metadata_linked_list metadata_block;

#define METADATA_SIZE sizeof(metadata_block)  // current design is 48bytes

#endif
