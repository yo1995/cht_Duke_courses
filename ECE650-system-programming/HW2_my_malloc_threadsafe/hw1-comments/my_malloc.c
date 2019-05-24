#include "my_malloc.h"

// init a head node
metadata_block* head = NULL;  
// init a tail node
metadata_block* tail = NULL;  
// init a head node for free list
metadata_block* free_head = NULL;  
// mutex lock as a global variable. see in report
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

/* helper funcs */

/**
 * check the validity of the to-free pointer
 * to see if it is malloc'd by my malloc func
 * @param  ptr the pointer to check    
 * @return     true: valid false: invalid
 */
bool metadata_check(void* ptr) {
    /*
     * do I need to add memory bound check here?
     */
    if(head == NULL || ptr == NULL) {
        return false;
    }
    metadata_block* check;
    check = ptr - METADATA_SIZE;
    return ptr == check -> check_ptr;
}

/**
 * round up the block size to 8n to align memory
 * @param  size input size
 * @return      output size of 8n
 */
size_t round_up_to8(size_t size) {
    return (size % 8) ? ((size / 8) + 1) * 8 : size;
}

/**
 * print the free_blocks list for debugging purpose
 */
void print_free_list() {
    if(!free_head) {
        printf("debug info: free head is NULL\n");
    }
    else {
        metadata_block* t = free_head;
        printf("debug info: free node size ");
        while(t) {
            if(t -> free_prev) {
                printf("<-");
            }
            printf(" (%d) ", (int)t -> size);
            if(t -> free_next) {
                printf("->");
            }
            t = t -> free_next;
        }
        printf("\n");
    }
}

/**
 * add a free block to the free list, manipulate its
 * prev and next pointers
 * @param b the block to be added
 */
void add_to_free_list(metadata_block* b) {
    if(!free_head) {
        free_head = b;
        b -> free_prev = NULL;
        b -> free_next = NULL;
        return;
    }
    else {
        metadata_block* t = free_head;
        metadata_block* p = NULL;
        while(b > t && t) {
            p = t;
            t = t -> free_next;
        }
        if(!t) {  // append to tail
            p -> free_next = b;
            b -> free_prev = p;
            b -> free_next = NULL;
            return;
        }
        else if(b < free_head) {  // append to head
            b -> free_next = free_head;
            b -> free_prev = NULL;
            free_head -> free_prev = b;
            free_head = b;
        }
        else if(b == free_head) {  // the block came from merging
            printf("Warning. it's impossible!!!!\n");
        }
        else {  // add this node to middle
            b -> free_prev = p;
            b -> free_next = t;
            t -> free_prev = b;
            p -> free_next = b;
        }
    }
}

/**
 * remove a block from free_blocks list
 * @param b the block to be removed
 */
void remove_from_free_list(metadata_block* b) {
    if(free_head == b) {
        free_head = b -> free_next;
        if(b -> free_next) {
            b -> free_next -> free_prev = NULL;
        }
        b -> free_prev = NULL;
        b -> free_next = NULL;
    }
    else {
        if(b -> free_next) {  // not the tail
            b -> free_prev -> free_next = b -> free_next; 
            b -> free_next -> free_prev = b -> free_prev;
            b -> free_prev = NULL;
            b -> free_next = NULL;
        }
        else {
            b -> free_prev -> free_next = NULL;
            b -> free_prev = NULL;
            b -> free_next = NULL;
        }
    }
}

/**
 * merge adjacent blocks in main list. when 2 blocks are 
 * adjacent to each other in the main list and are both
 * free, merge them into 1 block and re-connect the ptrs.
 *
 * will traverse to the first possible free block in main
 * list, and merge onward until no more adjacent free block
 * found.
 * 
 * @param b a free block to be merged with its neighbors
 */
void merge_adjacent(metadata_block* b) {  // metadata_block* 
    metadata_block* temp_free;
    metadata_block* temp_b;
    // first, loop towards the first consecutive free block
    if(b -> prev) {
        while(b -> prev && b -> prev -> is_free) {
            b = b -> prev;
        }
    }
    temp_b = b;
    // free all the following blocks until reach the end or not free.
    while(b -> next && b -> next -> is_free) {
        b -> size += METADATA_SIZE + b -> next -> size;
        b -> next = b -> next -> next;
    }
    
    if(b -> next) {  // if b->next is not NULL, then it's not free
        temp_free = b -> next -> prev;
        b -> next -> prev = b;

        if(temp_free -> free_next) {
            temp_free -> free_next -> free_prev = temp_b;
            temp_b -> free_next = temp_free -> free_next;
        }
        else {
            temp_b -> free_next = NULL;
        }
    }
    else {  // if b->next is NULL, then b eats up all remaining space
        temp_b -> free_next = NULL;
        tail = temp_b;  // the new end of the blocks
    }
    // printf("debug info: after merge, the size is %d\n", b -> size);
}

/**
 * split a large block into 2 smaller blocks, if it 
 * can hold sie + METADATA_SIZE
 * @param b    the block to be splitted
 * @param size the size for the first block
 */
void split_block(metadata_block* b, size_t size) {
    if((b -> size - size) <= (METADATA_SIZE)) {
        // printf("debug info: too small not split\n");
        return;  // do not split if too small
    }
    metadata_block* temp;
    void* p = b -> start_ptr;
    // printf("debug info: temp start at %x\n", p);
    p = (char*)p + size;
    temp = p;
    // printf("debug info: temp start at2 %x\n", p);
    temp -> size = b -> size - METADATA_SIZE - size;
    temp -> is_free = true;
    temp -> prev = b;
    
    if(b -> next) {
    	temp -> next = b -> next;
        b -> next -> prev = temp;
        if(b -> next -> is_free) {
        	b -> next -> free_prev = temp;
    	}
    }
    else {
    	temp -> next = NULL;
    	tail = temp;
    }
    temp -> free_prev = b;
    
    if(b -> free_next) {
    	temp -> free_next = b -> free_next;
        temp -> free_next -> free_prev = temp;
    }
    else {
    	temp -> free_next = NULL;
    }
    temp -> check_ptr = temp + 1;
    temp -> start_ptr = temp + 1;
    b -> size = size;
    b -> next = temp;
    b -> free_next = temp;
}

/**
 * my implementation of free function for both malloc's
 * @param ptr the address of start_ptr of a block
 */
void my_free(void *ptr) {
    metadata_block* temp;
    metadata_block* temp_head;
	if(!ptr || !metadata_check(ptr)) {
        printf("Error! not freed\n");
		return;
	}
    temp = ptr - METADATA_SIZE;
    temp -> is_free = true;
    add_to_free_list(temp);
    merge_adjacent(temp);
    // MARK:- no need to brk the last block right now.
}

/**
 * use first fit method to find a block.
 * @param  size the size of block to be found
 * @return      the pointer to the head of the found block
 */
metadata_block* first_fit_find(size_t size) {
    metadata_block* t = free_head;
    while(t && t -> size < size) {
        t = t -> free_next;
    }
    return t;  // found or NULL
}

/**
 * use best fit method to find a block.
 * @param  size the size of block to be found
 * @return      the pointer to the head of the found block
 */
metadata_block* best_fit_find(size_t size) {
    metadata_block* t = free_head;
    metadata_block* best = NULL;
    size_t diff = SIZE_MAX;
    size_t temp_diff;
    while(t != NULL) {
        if(t -> size >= size) {
            temp_diff = t -> size - size;
            if(temp_diff == 0) {  // end when find exact size
                best = t;
                return best;
            }
            if(temp_diff < diff) {
                diff = temp_diff;
                best = t;
            }
            t = t -> free_next;
            continue;
        }
        else {
            t = t -> free_next;
            continue;
        }
    }
    return best;  // found or NULL
}

/**
 * append a chunk to main list if no free block available to hold
 * the requested size.
 * @param  prev the tail of current main list
 * @param  size the size requested
 * @return      a pointer to the head of newly allocated memory
 */
metadata_block* append_space(metadata_block* prev, size_t size) {
    metadata_block* new;
    void* end;
    new = sbrk(0);
    end = sbrk(METADATA_SIZE + size);
    // printf("debug info: METADATA_SIZE is %d\n", METADATA_SIZE);
    // printf("debug info: alloc'd space at %x\n", new);
    if(end == (void *) - 1) {
        return NULL;  // sbrk failed
    }
    new -> size = size;
    new -> is_free = false;
    new -> prev = prev;
    new -> next = NULL;
    new -> free_prev = NULL;
    new -> free_next = NULL;
    new -> check_ptr = new + 1;
    new -> start_ptr = new + 1;  // a ptr to the beginning of data seg
    if(prev != NULL) {
        prev -> next = new;
        tail = new;
    }
    return new;  // usable addr
}

/* ================================================================ */
/* HW funcs */

void *ts_malloc_lock(size_t size) {

}

void ts_free_lock(void *ptr) {

}

void *ts_malloc_nolock(size_t size) {

}

void ts_free_nolock(void *ptr) {

}

//First Fit malloc/free
void* ff_malloc(size_t size) {
    // print_free_list();
    // printf("===========ff_malloc called============\n");
    size = round_up_to8(size);
    metadata_block* current;
    if(size <= 0) {
        return NULL;  // invalid case, just return NULL ptr
    }

    if(head == NULL) {  // first malloc
        // printf("debug info: first ff_malloc, METADATA_SIZE %d\n", METADATA_SIZE);
        current = append_space(NULL, size);
        head = current;  // only operation touches head, be careful!
        tail = current;
        return current -> start_ptr;  // fail: NULL, succeed: (new)
    }
    else {
        current = first_fit_find(size);
        if(current == NULL) {  // not found
            current = append_space(tail, size);  // extend heap
            // printf("debug info: current addr  is %x\n", current);
            return current -> start_ptr;
        }
        else {
            split_block(current, size);
            current -> is_free = false;
            remove_from_free_list(current);
            return current -> start_ptr;
        }
    }
}

//Best Fit malloc/free
void* bf_malloc(size_t size) {
    // print_free_list();
    // printf("===========bf_malloc called============\n");
    size = round_up_to8(size);
    metadata_block* current;
    if(size <= 0) {
        return NULL;  // invalid case, just return NULL ptr
    }

    if(head == NULL) {  // first malloc
        // printf("debug info: best bf_malloc, METADATA_SIZE %d\n", METADATA_SIZE);
        current = append_space(NULL, size);
        head = current;
        tail = current;
        return current -> start_ptr;  // fail: NULL, succeed: (new)
    }
    else {
        current = best_fit_find(size);
        // printf("debug info: temp_prev is %x\n", temp_prev);
        if(current == NULL) {  // not found
            current = append_space(tail, size);
            return current -> start_ptr;
        }
        else {
            split_block(current, size);
            current -> is_free = false;
            remove_from_free_list(current);
            return current -> start_ptr;
        }
    }
}

void ff_free(void *ptr) {
    my_free(ptr);
}

void bf_free(void *ptr) {
    my_free(ptr);
}

unsigned long get_data_segment_size() {
    metadata_block* b = head;
    unsigned long result = 0;
    size_t count = 0;
    while(b != NULL) {
        count++;
        result += b -> size;
        b = b -> next;
    }
    return result + count * METADATA_SIZE;
}

unsigned long get_data_segment_free_space_size() {
    metadata_block* b = head;
    unsigned long result = 0;
    while(b != NULL) {
        if(b -> is_free) {
            result += b -> size + METADATA_SIZE;    
        }
        b = b -> next;
    }
    return result;
}
