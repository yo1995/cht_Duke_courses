#include "my_malloc.h"

metadata_block* head = NULL;  // init a head node
metadata_block* free_list_head = NULL;  // init a head node for free'd list

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


void merge_adjacent(metadata_block* b) {
    // first, loop towards the first consecutive free-able block

    if(b -> prev) {
        while(b -> prev && b -> prev -> is_free) {
            // printf("debug info: addr of prev %x\n", b -> prev);
            b = b -> prev;
        }
    }
    
    // free all the following blocks until reach the end or not free.
    while(b -> next && b -> next -> is_free) {
        b -> size += METADATA_SIZE + b -> next -> size;
        b -> next = b -> next -> next;
    }
    if(b -> next) {
        b -> next -> prev = b;
    }
    // printf("debug info: after merge, the size is %d\n", b -> size);
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
    temp -> next = b -> next;
    if(temp -> next) {
        temp -> next -> prev = temp;
    }
    temp -> check_ptr = temp + 1;
    temp -> start_ptr = temp + 1;
    b -> size = size;
    b -> next = temp;
    // printf("debug info: splitted\n");
}

void my_free(void *ptr) {
    metadata_block* temp;
	if(ptr == NULL || !metadata_check(ptr)) {
		return;
	}
    temp = ptr - METADATA_SIZE;
    temp -> is_free = true;
    merge_adjacent(temp);
    // MARK:- no need to brk the last block right now.
}


/* HW funcs */

metadata_block* first_fit_find(metadata_block** prev, size_t size) {
    metadata_block* t = head;
    *prev = head;
    // not free or too small
    // printf("debug info: size to find %d\n", size);
    while(t && !(t -> is_free && t -> size >= size)) {
        // printf("    debug info: size looped %d\n", t -> size);
        *prev = t;
        t = t -> next;
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


metadata_block* best_fit_find(metadata_block** prev, size_t size) {
    metadata_block* t = head;
    *prev = head;
    metadata_block* best = NULL;
    size_t diff = SIZE_MAX;
    size_t temp_diff;
    while(t != NULL) {
        if(t -> is_free && t -> size >= size) {
            temp_diff = t -> size - size;
            if(temp_diff == 0) {  // end when find exact size
                best = t;
                return best;
            }
            if(temp_diff < diff) {
                diff = temp_diff;
                best = t;
            }
            *prev = t;
            t = t -> next;  
        }
        else {
            *prev = t;
            t = t -> next;
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
    new -> check_ptr = new + 1;
    new -> start_ptr = new + 1;  // a pointer to the beginning data seg
    if(prev != NULL) {
        prev -> next = new;
    }
    return new;  // usable addr
}


//First Fit malloc/free
void* ff_malloc(size_t size) {
    // printf("===========ff_malloc called============\n");
    size = round_up_to8(size);
    metadata_block* current;
    metadata_block* temp_prev;
    if(size <= 0) {
        return NULL;  // invalid case, just return NULL ptr
    }

    if(head == NULL) {  // first malloc
        // printf("debug info: first ff_malloc, METADATA_SIZE %d\n", METADATA_SIZE);
        current = append_space(NULL, size);
        head = current;  // only operation touches head, be careful!
        return current -> start_ptr;  // fail: NULL, succeed: (new)
    }
    else {
        current = first_fit_find(&temp_prev, size);
        if(current == NULL) {  // not found
            current = append_space(temp_prev, size);
            return current -> start_ptr;
        }
        else {
            split_block(current, size);
            current -> is_free = false;
            return current -> start_ptr;
        }
    }
}

//Best Fit malloc/free
void* bf_malloc(size_t size) {
    size = round_up_to8(size);
    printf("===========bf_malloc called============\n");
    metadata_block* current;
    metadata_block* temp_prev;
    if(size <= 0) {
        return NULL;  // invalid case, just return NULL ptr
    }

    if(head == NULL) {  // first malloc
        // printf("debug info: best bf_malloc, METADATA_SIZE %d\n", METADATA_SIZE);
        current = append_space(NULL, size);
        head = current;  // only operation touches head, be careful!
        return current -> start_ptr;  // fail: NULL, succeed: (new)
    }
    else {
        current = best_fit_find(&temp_prev, size);
        // printf("debug info: temp_prev is %x\n", temp_prev);
        if(current == NULL) {  // not found
            current = append_space(temp_prev, size);
            return current -> start_ptr;
        }
        else {
            split_block(current, size);
            current -> is_free = false;
            return current -> start_ptr;
        }
    }
}

void ff_free(void *ptr) {
    my_free(ptr);
}

void bf_free(void *ptr) {
    // printf("+++++++++++++++++++++++\n");
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
        result += b -> size;
        b = b -> next;
    }
    return result;
}
