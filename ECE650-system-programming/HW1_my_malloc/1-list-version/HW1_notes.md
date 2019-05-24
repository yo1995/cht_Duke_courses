## malloc referred resources

- official reference and definition of malloc:
http://man7.org/linux/man-pages/man3/malloc.3.html

- reference that I read but did not copy from:
https://danluu.com/malloc-tutorial/

- size_t maximum size:
https://en.wikipedia.org/wiki/C_data_types

## testing

### general test

valgrind --leak-check=full --show-leak-kinds=all ./mymalloc_test

- new + METADATA_SIZE
when moving pointers, +1 means advancing by 1 block, not 1 byte...
haven't done C for a while, and forgot this one. took me 1 hour to find it out.

https://stackoverflow.com/questions/6449935/increment-void-pointer-by-one-byte-by-two

### alloc policy test

valgrind --leak-check=full --show-leak-kinds=all ./equal_size_allocs

## either to track malloc'd blocks

in my previous implementation, I tracked all the blocks malloc'd and free'd from my funcs. however, after discussing with TAs, I found it might not need to check that. thus, the minimal items or the METADATA should be something like :

```c
struct _metadata_linked_list {
	size_t size;
	bool is_free;
	struct _metadata_linked_list* next;
};
```

## alignment issues

I read some materials about how alignment of memory would affect the performance of it. It suggests using 8 bytes aligned blocks for better performance.

In terms of splitting blocks, we can always round up blocks to 8n so that it would be better.

## more to add 

- a list of pointers to record all the free blocks

- efficiency improvement thoughts

to improve best fit method of searching, METADATA blocks could be saved in a binary search tree ordered by size.

to improve overall performance, one can maintain several different lists that fits different sizes. e.g. 8b, 32b, 64b, etc. so that malloc and free can directly go to corresponding lists.

## results

### First fit

- ./equal_size_allocs
Execution Time = 555.002142 seconds
Fragmentation  = 0.727273

- ./small_range_rand_allocs
data_segment_size = 3996368, data_segment_free_space = 3453296
Execution Time = 90.171192 seconds
Fragmentation  = 0.864109

- ./large_range_rand_allocs
Execution Time = 147.103333 seconds
Fragmentation  = 0.998035

### Best fit

