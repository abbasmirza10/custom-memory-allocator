/**
 * Malloc
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


typedef char ALIGN[16]; // to align memory blocks

union header {
  struct {
    size_t size;
    unsigned is_free;
    union header *next;
  } s;
  ALIGN s_;
};

typedef union header header_t;

header_t *head, *tail;
header_t *free_header; // head of free list

void split_block(header_t *curr, size_t size) {
  header_t *new_block = (header_t*)((char*)curr + sizeof(header_t) + size);
  new_block->s.size = curr->s.size - sizeof(header_t) - size;
  new_block->s.is_free = 1;
  new_block->s.next = curr->s.next;
  curr->s.size = size;
  curr->s.next = new_block;
}

void free_list_remove(header_t *curr, header_t *prev) {
  if (prev)
    prev->s.next = curr->s.next;
  else
    free_header = curr->s.next;
}

header_t *get_free_block(size_t size) {
  header_t *curr = free_header;
  header_t *prev = NULL;
  while(curr) {
    if (curr->s.size >= size) {
      // Block splitting the relevant blocks
      if (abs(curr->s.size - size) >= 1) {
        split_block(curr, size);
      }
      // if (curr->s.size > size + sizeof(header_t)) {
      //   split_block(curr, size);
      // }
      free_list_remove(curr, prev);
      return curr;
    }
    prev = curr;
    curr = curr->s.next;
  }
  // Return NULL if no suitable block found
  return NULL;
}

/**
 * Allocate space for array in memory
 *
 * Allocates a block of memory for an array of num elements, each of them size
 * bytes long, and initializes all its bits to zero. The effective result is
 * the allocation of an zero-initialized memory block of (num * size) bytes.
 *
 * @param num
 *    Number of elements to be allocated.
 * @param size
 *    Size of elements.
 *
 * @return
 *    A pointer to the memory block allocated by the function.
 *
 *    The type of this pointer is always void*, which can be cast to the
 *    desired type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory, a
 *    NULL pointer is returned.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/calloc/
 */

void *calloc(size_t num, size_t size) {
  size_t total_size;
  void *block;
  total_size = num * size;
  if (!total_size)
    return NULL;
  block = malloc(total_size);
  if (!block)
    return NULL;
  memset(block, 0, total_size);
  return block;
}

/**
 * Allocate memory block
 *
 * Allocates a block of size bytes of memory, returning a pointer to the
 * beginning of the block.  The content of the newly allocated block of
 * memory is not initialized, remaining with indeterminate values.
 *
 * @param size
 *    Size of the memory block, in bytes.
 *
 * @return
 *    On success, a pointer to the memory block allocated by the function.
 *
 *    The type of this pointer is always void*, which can be cast to the
 *    desired type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory,
 *    a null pointer is returned.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/malloc/
 */


header_t *allocate_block(size_t total_size) {
  void *block = sbrk(total_size);
  if (block == (void*) -1)
    return NULL;
  return block;
}

void add_to_allocated_list(header_t *header) {
  if (!head)
    head = header;
  if (tail)
    tail->s.next = header;
  tail = header;
}

header_t *create_header(void *block, size_t size) {
  header_t *header = block;
  header->s.size = size;
  header->s.is_free = 0;
  header->s.next = NULL;
  return header;
}

void *malloc(size_t size) {
  size_t total_size;
  header_t *header;
  if (!size)
    return NULL;
  header = get_free_block(size);
  if (header) {
    header->s.is_free = 0;
    return (void*)(header + 1);
  }
  total_size = sizeof(header_t) + size;
  void *block = allocate_block(total_size);
  if (!block)
    return NULL;
  header = create_header(block, size);
  header->s.size = size;
  header->s.is_free = 0;
  add_to_allocated_list(header);
  return (void*)(header + 1);
}

/**
 * Deallocate space in memory
 *
 * A block of memory previously allocated using a call to malloc(),
 * calloc() or realloc() is deallocated, making it available again for
 * further allocations.
 *
 * Notice that this function leaves the value of ptr unchanged, hence
 * it still points to the same (now invalid) location, and not to the
 * null pointer.
 *
 * @param ptr
 *    Pointer to a memory block previously allocated with malloc(),
 *    calloc() or realloc() to be deallocated.  If a null pointer is
 *    passed as argument, no action occurs.
 */

void release_block(void *ptr, header_t *header) {
  header_t *tmp;
  if (head == tail) {
    head = tail = NULL;
  } else {
    tmp = head;
    while (tmp) {
      if(tmp->s.next == tail) {
        tmp->s.next = NULL;
        tail = tmp;
      }
      tmp = tmp->s.next;
    }
  }
  sbrk(0 - sizeof(header_t) - header->s.size);
}


void update_free_list(header_t *header, header_t *prev) {
  if (prev)
    prev->s.next = header;
  else
    free_header = header;
}

void coalesce_blocks(header_t *header) {
  header_t *prev = NULL;
  for (header_t *tmp = free_header; tmp != NULL; tmp = tmp->s.next) {
    if ((char*)tmp + tmp->s.size + sizeof(header_t) == (char*)header) {
      tmp->s.size += sizeof(header_t) + header->s.size;
      header = tmp;
    } else if ((char*)header + header->s.size + sizeof(header_t) == (char*)tmp) {
      header->s.size += sizeof(header_t) + tmp->s.size;
      header->s.next = tmp->s.next;
    } else {
      prev = tmp;
    }
  }
  update_free_list(header, prev);
}


void free(void *ptr) {
  header_t *header;
  void *programbreak;
  if (!ptr)
    return;
  header = (header_t*)ptr - 1;
  programbreak = sbrk(0);
  if ((char*)ptr + header->s.size == programbreak) {
    release_block(ptr, header);
    return;
  }
  header->s.is_free = 1;
  // Coalescing the blocks
  coalesce_blocks(header);
}


/**
 * Reallocate memory block
 *
 * The size of the memory block pointed to by the ptr parameter is changed
 * to the size bytes, expanding or reducing the amount of memory available
 * in the block.
 *
 * The function may move the memory block to a new location, in which case
 * the new location is returned. The content of the memory block is preserved
 * up to the lesser of the new and old sizes, even if the block is moved. If
 * the new size is larger, the value of the newly allocated portion is
 * indeterminate.
 *
 * In case that ptr is NULL, the function behaves exactly as malloc, assigning
 * a new block of size bytes and returning a pointer to the beginning of it.
 *
 * In case that the size is 0, the memory previously allocated in ptr is
 * deallocated as if a call to free was made, and a NULL pointer is returned.
 *
 * @param ptr
 *    Pointer to a memory block previously allocated with malloc(), calloc()
 *    or realloc() to be reallocated.
 *
 *    If this is NULL, a new block is allocated and a pointer to it is
 *    returned by the function.
 *
 * @param size
 *    New size for the memory block, in bytes.
 *
 *    If it is 0 and ptr points to an existing block of memory, the memory
 *    block pointed by ptr is deallocated and a NULL pointer is returned.
 *
 * @return
 *    A pointer to the reallocated memory block, which may be either the
 *    same as the ptr argument or a new location.
 *
 *    The type of this pointer is void*, which can be cast to the desired
 *    type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory,
 *    a NULL pointer is returned, and the memory block pointed to by
 *    argument ptr is left unchanged.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/realloc/
 */

void *realloc(void *ptr, size_t size) {
  if (!ptr || !size)
    return malloc(size);
  header_t *header = (header_t*)ptr - 1;
  if (header->s.size >= size)
    return ptr;
  void *new_ptr = malloc(size);
  if (!new_ptr)
    return NULL;
  memcpy(new_ptr, ptr, header->s.size);
  free(ptr);
  return new_ptr;
}



