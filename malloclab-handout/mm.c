/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "ED",
    /* First member's full name */
    "Doron Schwartz",
    /* First member's email address */
    "deschwa2@mail.yu.edu",
    /* Second member's full name (leave blank if none) */
    "Eitan Remer",
    /* Second member's email address (leave blank if none) */
    "eremer@mail.yu.edu"
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))
#define HEADER_SIZE (ALIGN(sizeof(node)))
// This is the header for each block, which is essentialy a double linked list
void *findspace(size_t size);
typedef struct node{
     node * next;
     node * prev;
    size_t s;
}node;
/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    node *start = mem_sbrk(HEADER_SIZE);
    start->next = start;
    start->prev = start;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    //Take the size of the block, take the amount we need for our stupid header, and check.
    int newsize = ALIGN(size + HEADER_SIZE);
    if(newsize < ALIGN(sizeof(node))){
            newsize = ALIGN(sizeof(node));
    }
    size_t *pointer;
    *pointer = find(newsize);
    if (pointer){
        //cast the pointer to the block as it is currently void and use the size anded with one to succesfully retrieve the header pointer from the void pointer 
        *pointer = ((node*)pointer)->s | 1;//QUESTION ON WHAT THIS LINE IS DOING
    }
	else {
        // Oh dang, we couldnt find any freaking pointer in the find method, 
        pointer = mem_sbrk(newsize);
        *pointer = newsize | 1;//SAME WITH THIS ONE BIG SAUCE 
    }
    return (void *)((char *)pointer + HEADER_SIZE);
}
// this method is mostly finished, I LOVE YOU, WHAT ELSE WOULD NEED TO BE DONE
void *findspace(size_t size){
     node *blockpointer = mem_heap_lo();
    // Go through the heap, trying to find the block that could be the fit for the space that we are looking for.
    for(blockpointer = blockpointer->next;blockpointer != mem_heap_lo();blockpointer = blockpointer->next){
        if(blockpointer->s == size){
            // essentialy we have a list of free blocks, so now i am going to remove the block using common linked list shtick and go by the seat of my pants
            // Make the previous of the block which is free have its next pointer point to the block that is after the free;
            blockpointer->prev->next = blockpointer->next;
            // make the block that is next have its previous block point towards the current 
            blockpointer->next->prev = blockpointer->prev;
            return blockpointer;
        }
    }
    return NULL;
}
/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    // This is where things get funky, got to use char pointer so the subtraction will move in one byte increments instead of 8 byte increments.
    node *blockheader = (char *)ptr - HEADER_SIZE;
    //Go get the beginning of the list
    node *listheader = mem_heap_lo();
    // going to need to finish up the problems of this not really 
    blockheader->s = *(size_t *)blockheader;
    //got to mess with the individual weirdness now of moving it back into the list, Ill put the bkock for our purposes there now.
    // The current block  
    blockheader->next = listheader -> next ;
    // The block's own previous will be the beginning
    blockheader->prev = listheader;
    // The freelist is now going to have its next objct in the list, BACK LINK will be the block header aspect
    listheader -> next -> prev = blockheader;
    // This is 
    listheader -> next = listheader -> next -> prev;

    // need to still coalesce the blocks when there is a lot of open space

}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;
    
    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;
    copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}














