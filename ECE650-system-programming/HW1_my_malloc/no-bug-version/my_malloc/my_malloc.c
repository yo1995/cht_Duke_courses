#include "my_malloc.h"

metadata_block* head = NULL;  // init a head node
metadata_block* tail = NULL;  // init a tail node
metadata_block* free_head = NULL;  // init a head node for free list

/* helper funcs */
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

size_t round_up_to8(size_t size) {
    return (size % 8) ? ((size / 8) + 1) * 8 : size;
}

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


void add_to_free_list(metadata_block* b) {
    // printf("debug info: add size of %d\n", b -> size);
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

void remove_from_free_list(metadata_block* b) {
    // printf("debug info: to remove %x\n", b);
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
            // printf("debug info: remove from last free list\n");
            b -> free_prev -> free_next = NULL;
            b -> free_prev = NULL;
            b -> free_next = NULL;
        }
    }


}

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
        // if(b -> free_next) {
        //     b -> free_next = b -> free_next -> free_next;
        // }
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

        // while(b -> next && !(b -> next -> is_free)) {
        //     b = b -> next;
        // }
        // if(b -> next) {  // it is a free block
        //     temp_b -> free_next = b -> next;
        //     b -> next -> free_prev = temp_b;
        // }
        // else {
        //     temp_b -> free_next = NULL;
        // }
    }
    else {  // if b->next is NULL, then b eats up all remaining space
        temp_b -> free_next = NULL;
        tail = temp_b;  // the new end of the blocks
    }

    

    // if(b -> free_next) {
    //     b -> free_next -> free_prev = b;
    // }
    
    // printf("debug info: after merge, the size is %d\n", b -> size);
    // return temp_head;
}

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
    
    // b -> free_prev = NULL;
    // printf("debug info: splitted\n");
}

void my_free(void *ptr) {
    metadata_block* temp;
    metadata_block* temp_head;
	if(!ptr || !metadata_check(ptr)) {
        printf("Error! not freed\n");
		return;
	}
    temp = ptr - METADATA_SIZE;
    temp -> is_free = true;
    add_to_free_list(temp);  // temp_head
    merge_adjacent(temp);  // temp_head = 
    
    // MARK:- no need to brk the last block right now.
}


/* HW funcs */

metadata_block* first_fit_find(size_t size) {
    metadata_block* t = free_head;
    // not free or too small
    // printf("debug info: size to find %d\n", size);
    while(t && t -> size < size) {
        // printf("    debug info: size looped %d\n", t -> size);
        t = t -> free_next;
    }
    /*
    if(!t) {
        printf("debug info: fff not found\n");
    }
    else {
        printf("debug info: ff found space at %x, ", t);
        printf("size %d\n", t->size);
    }
    */
    return t;  // found or NULL
}


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
    new -> start_ptr = new + 1;  // a pointer to the beginning data seg
    if(prev != NULL) {
        prev -> next = new;
        tail = new;
    }
    return new;  // usable addr
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
    // printf("+++++++++++++++++++++++\n");
    my_free(ptr);
}

void bf_free(void *ptr) {
    // printf("+++++++++++++++++++++++\n");
    // my_free(ptr);
    metadata_block* temp;
    metadata_block* temp_head;
	if(!ptr || !metadata_check(ptr)) {
        printf("Error! not freed\n");
		return;
	}
    temp = ptr - METADATA_SIZE;
    temp -> is_free = true;
    add_to_free_list(temp);  // temp_head
    merge_adjacent(temp);
    // merge_adjacent_onward(temp);
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
