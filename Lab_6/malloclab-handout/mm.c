/* 
 * mm-implicit.c -  Simple allocator based on implicit free lists, 
 *                  first fit placement, and boundary tag coalescing. 
 *
 * Each block has header and footer of the form:
 * 
 *      31                     3  2  1  0 
 *      -----------------------------------
 *     | s  s  s  s  ... s  s  s  0  0  a/f
 *      ----------------------------------- 
 * 
 * where s are the meaningful size bits and a/f is set 
 * iff the block is allocated. The list has the following form:
 *
 * begin                                                          end
 * heap                                                           heap  
 *  -----------------------------------------------------------------   
 * |  pad   | hdr(8:a) | ftr(8:a) | zero or more usr blks | hdr(8:a) |
 *  -----------------------------------------------------------------
 *          |       prologue      |                       | epilogue |
 *          |         block       |                       | block    |
 *
 * The allocated prologue and epilogue blocks are overhead that
 * eliminate edge conditions during coalescing.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
  "Team 1",
  "Naureen Bharwani",
  "naureen.bharwani@colorado.edu",
  "Emily Carpenter",
  "emca5057@colorado.edu"
};

/////////////////////////////////////////////////////////////////////////////
// Constants and macros
//
// These correspond to the material in Figure 9.43 of the text
// The macros have been turned into C++ inline functions to
// make debugging code easier.
//
/////////////////////////////////////////////////////////////////////////////
#define WSIZE       4       /* word size (bytes) */  
#define DSIZE       8       /* doubleword size (bytes) */
#define CHUNKSIZE  (1<<12)  /* initial heap size (bytes) */
#define OVERHEAD    8       /* overhead of header and footer (bytes) */

static inline int MAX(int x, int y) {
  return x > y ? x : y;
}

//
// Pack a size and allocated bit into a word
// We mask of the "alloc" field to insure only
// the lower bit is used
//
// combines a size and an allocate bit and returns a value that can be stored in a header or footer
static inline uint32_t PACK(uint32_t size, int alloc) {
  return ((size) | (alloc & 0x1));
}

//
// Read and write a word at address p
//
static inline uint32_t GET(void *p) { return  *(uint32_t *)p; }
static inline void PUT( void *p, uint32_t val)
{
  *((uint32_t *)p) = val;
}

//
// Read the size and allocated fields from address p
//
static inline uint32_t GET_SIZE( void *p )  { 
  return GET(p) & ~0x7;
}

static inline int GET_ALLOC( void *p  ) {
  return GET(p) & 0x1;
}

//
// Given block ptr bp, compute address of its header and footer
// The remaining macros operate on block pointers (denoted bp) that point to the first payload byte
// Given a block pointer bp, the HDRP and FTRP macros (lines 20–21) return pointers to the block header and footer, respectively
// 
static inline void *HDRP(void *bp) {

  return ( (char *)bp) - WSIZE;
}
static inline void *FTRP(void *bp) {
  return ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE);
}

//
// Given block ptr bp, compute address of next and previous blocks
//
static inline void *NEXT_BLKP(void *bp) {
  return  ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)));
}

static inline void* PREV_BLKP(void *bp){
  return  ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)));
}

/////////////////////////////////////////////////////////////////////////////
//
// Global Variables
//
// allocator uses a single private (static) global variable (heap_listp) that always points to the prologue block
static char *heap_listp;  /* pointer to first block */  
static char *last_fitbp;  /* pointer to the header of the last found fit block */

//
// function prototypes for internal helper routines
//
static void *extend_heap(uint32_t words);
static void place(void *bp, uint32_t asize);
static void *find_fit(uint32_t asize);
static void *coalesce(void *bp);
static void printblock(void *bp); 
static void checkblock(void *bp);



//
// mm_init - Initialize the memory manager 
// creates a heap with an initial free block

int mm_init(void) 
{
  //
  // You need to provide this
  //
  // mm_init function initializes the allocator, returning 0 if successful and −1 otherwise
  if ((heap_listp = mem_sbrk(4*WSIZE)) == (void *)-1) {
      return -1; 
  }
  
  // initialize last fit block to be start of the heap list, start at first block on heap
  last_fitbp = heap_listp;

  // Page 883, Figure 9.44 - mm_init function gets four words from the memory system
  // initializes them to create the empty free list
  // prologue block, which is an 8-byte allocated block consisting of only a header and a footer
  // The prologue block is created during initialization and is never freed.
  PUT(heap_listp, 0); /* Alignment padding */
  PUT(heap_listp + (1*WSIZE), PACK(DSIZE, 1)); /* Prologue header */ 
  PUT(heap_listp + (2*WSIZE), PACK(DSIZE, 1)); /* Prologue footer */ 
  PUT(heap_listp + (3*WSIZE), PACK(0, 1)); /* Epilogue header */ 
  heap_listp += (2*WSIZE); /* Extend the empty heap with a free block of CHUNKSIZE bytes */ 
    
        
  // calls the extend_heap function (Figure 9.45)
  // extends the heap by CHUNKSIZE bytes and creates the initial free block.
  if (extend_heap(CHUNKSIZE/WSIZE) == NULL) {
      return -1;
  }
  return 0;
}



//
// extend_heap - Extend heap with free block and return its block pointer
/* invoked in two different circumstances: 
 * (1) when the heap is initialized
 * (2) when mm_malloc is unable to find a suitable fit
*/

static void *extend_heap(uint32_t words) 
{
  //
  // Page 883 in book, Figure 9.45
  //
  char *bp;
  size_t size;
  /* Allocate an even number of words to maintain alignment */
  // extend_heap rounds up the requested size to the nearest multiple of 2 words (8 bytes)
  // then requests the additional heap space from the memory system
  size = (words % 2) ? (words+1) * WSIZE : words * WSIZE;
  // mem_srbk returns the start address of the new area
  if ((long)(bp = mem_sbrk(size)) == -1) {
      return NULL;
  }

  /* Initialize free block header/footer and the epilogue header */
  PUT(HDRP(bp), PACK(size, 0)); /* Free block header */
  PUT(FTRP(bp), PACK(size, 0)); /* Free block footer */
  PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); /* New epilogue header */
    
  /* Coalesce if the previous block was free */
  // case that the previous heap was terminated by a free block
  // we call the coalesce function to merge the two free blocks and return the block pointer of the merged blocks 
  return coalesce(bp);
}



//
// Practice problem 9.8
//
// find_fit - Find a fit for a block with asize bytes 
//
static void *find_fit(uint32_t asize)
{
  // Next-fit search
  // initialize a block pointer starting at the last fit block of the heap -> last_fitbp
  void *bp = last_fitbp;

  // iterate over each block after last found fit in the heap until size is less than 0
  // incr to the next block in the heap each time
  for (last_fitbp = bp; GET_SIZE(HDRP(last_fitbp)) > 0; last_fitbp = NEXT_BLKP(last_fitbp)){
      // check to see if curr block is allocated and if it's size if big enough for the desired asize block requested
      if(!GET_ALLOC(HDRP(last_fitbp)) && (asize <= GET_SIZE(HDRP(last_fitbp)))){
          // fit was found for desired block of size asize
          // update last_fitbp to the curr bp that was found as a fit within coalesce
          return last_fitbp;
      }
  }

  // First-fit search up till last_fitbp
  // initialize a block pointer starting at the beginiing of the heap -> heap_listp
  // iterate over each block in the heap until you reach back around at last fit found
  // until size of heap is less than 0
  // incr to the next block in the heap each time
  for (last_fitbp = heap_listp; last_fitbp < (char *)bp; last_fitbp = NEXT_BLKP(last_fitbp)){
      // check to see if curr block is allocated and if it's size if big enough for the desired asize block requested
      if(!GET_ALLOC(HDRP(last_fitbp)) && (asize <= GET_SIZE(HDRP(last_fitbp)))){
          // fit was found for desired block of size asize
          // update last_fitbp to the curr bp that was found as a fit within coalesce
          return last_fitbp;
      }
  }
     
  return NULL; /* no fit */
}



// 
// mm_free - Free a block 
// An application frees a previously allocated block by calling the mm_free function
// Frees the requested block (bp) and then merges adjacent free blocks using the boundary-tags coalescing technique
void mm_free(void *bp)
{
  //
  // You need to provide this
  // For example, given a pointer bp to the current block
  // we could use the following line of code to determine the size of the next block in memory:
  // size_t size = GET_SIZE(HDRP(NEXT_BLKP(bp)));
  // frees the requested block (bp)
  // then merges adjacent free blocks using the boundary-tags coalescing technique
  size_t size = GET_SIZE(HDRP(bp));
  
  PUT(HDRP(bp), PACK(size, 0));
  PUT(FTRP(bp), PACK(size, 0));
  coalesce(bp);
}




//
// coalesce - boundary tag coalescing. Return ptr to coalesced block
//
static void *coalesce(void *bp) 
{
  // Page 885, Figure 9.46
  // get size of previous block from footer 
  // get size of next block from header
  // get size of current block from header
  size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
  size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
  size_t size = GET_SIZE(HDRP(bp));

  // Case 1: prev and next blocks are both allocated
  // Simply return the current blocks pointer
  if (prev_alloc && next_alloc) {
      return bp;
  }
    
  // Case 2: prev block is allocated and next block is free
  // Merge current block and next block
  // Update header of current block and footer of next block
  // New size = combined sizes of the curr and next blocks
  else if (prev_alloc && !next_alloc) {
      // get next blocks header and incr size
      // update header & footer of newly combined block to be unallocated -> 0
      size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
      PUT(HDRP(bp), PACK(size, 0));
      PUT(FTRP(bp), PACK(size,0));
  }
    
  // Case 3: prev block is free and next block is allocated
  // Merge previous and current blocks
  // Update header of previous block and footer of current block
  // New size = combined sizes of the previous and curr blocks
  else if (!prev_alloc && next_alloc) {
      // get previous blocks header and incr size
      // update header & footer of newly combined block to be unallocated -> 0
      // update pointer so it is now at previous block to account for 1 newly combined unallocated block
      size += GET_SIZE(HDRP(PREV_BLKP(bp)));
      PUT(FTRP(bp), PACK(size, 0));
      PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
      bp = PREV_BLKP(bp);
  }
    
  // Case 4: prev block is free and next block is free
  // Merge previous, curr and next blocks
  // Update header of prev block and footer of next block
  // New size = combined sizes of the prev, curr, and next blocks
  else {
      // get previous blocks header & next blocks footer, and incr size to perform merge
      // update header & footer of newly combined block to be unallocated -> 0
      // update pointer so it is now at previous block to account for 1 newly combined unallocated block
      size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(FTRP(NEXT_BLKP(bp)));
      PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
      PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
      bp = PREV_BLKP(bp);
  }

  // Need to confirm that we do not have our last_fitbp within our coalesced block
  if ((last_fitbp >= (char *)bp) && (last_fitbp < (char *)NEXT_BLKP(bp))){
  	// Update last_fitbp to be the beginning of our current coalesced 
    last_fitbp = bp;
  }
      
  // return pointer to current block
  return bp;
}



//
// mm_malloc - Allocate a block with at least size bytes of payload 
// An application requests a block of size bytes of memory by calling the mm_malloc function
void *mm_malloc(uint32_t size) 
{
  //
  // You need to provide this
  //
  size_t asize; // Adjusted block size
  size_t extendsize; // Amount to extend heap if no fit
  char *bp; // block pointer
  // Ignore size extension if trival 
  if (size == 0) {
      return NULL;
  }
    
  /* Adjust block size to include overhead and alignment reqs. */ 
  // minimum block size of 16 bytes: 8 bytes to satisfy the alignment requirement and 8 more bytes for the overhead of the header and footer
  if (size <= DSIZE) {
      asize = 2*DSIZE; 
  }
  
  // requests over 8 bytes, the general rule is to add in the overhead bytes and then round up to the nearest multiple of 8
  else {
      asize = DSIZE * ((size + (DSIZE) + (DSIZE-1)) / DSIZE);
  }
    
  /* Search the free list for a fit */ 
  // If there is a fit, then the allocator places the requested block and optionally splits the excess
  // return block pointer to newly allocated block
  if ((bp = find_fit(asize)) != NULL) {
      place(bp, asize);
      return bp; 
  }
    
  /* No fit found. Get more memory and place the block */ 
  // extends the heap with a new free block, places the requested block in the new free block
  extendsize = MAX(asize, CHUNKSIZE);
  if ((bp = extend_heap(extendsize/WSIZE)) == NULL) {
      return NULL;
  }
  place(bp, asize);
  return bp;
}
    
 

//
//
// Practice problem 9.9
//
// place - Place block of asize bytes at start of free block bp 
//         and split if remainder would be at least minimum block size
//
static void place(void *bp, uint32_t asize)
{
  // initialize size of bp
  size_t csize = GET_SIZE(HDRP(bp));
    
  // first check to see if the size of asize if equal to that of the block size
  // if it's equal simply update the bp to the new size, and update the block to allocated
  if ((csize - asize) >= (2 * DSIZE)) {
      PUT(HDRP(bp), PACK(asize, 1));
      PUT(FTRP(bp), PACK(asize, 1));
      bp = NEXT_BLKP(bp);
      PUT(HDRP(bp), PACK(csize - asize, 0));
      PUT(FTRP(bp), PACK(csize - asize, 0));
  }
  
  // if asize < bp size, then update the curr bp size and update to allocated
  // then move to the next bp and update it's allocation to free, and it's new size
  else {
      PUT(HDRP(bp), PACK(csize, 1));
      PUT(FTRP(bp), PACK(csize, 1));
  }  
}



//
// mm_realloc -- implemented for you
//
void *mm_realloc(void *ptr, uint32_t size)
{
  void *newp;
  uint32_t copySize;

  newp = mm_malloc(size);
  if (newp == NULL) {
    printf("ERROR: mm_malloc failed in mm_realloc\n");
    exit(1);
  }
  copySize = GET_SIZE(HDRP(ptr));
  if (size < copySize) {
    copySize = size;
  }
  memcpy(newp, ptr, copySize);
  mm_free(ptr);
  return newp;
}

//
// mm_checkheap - Check the heap for consistency 
//
void mm_checkheap(int verbose) 
{
  //
  // This provided implementation assumes you're using the structure
  // of the sample solution in the text. If not, omit this code
  // and provide your own mm_checkheap
  //
  void *bp = heap_listp;
  
  if (verbose) {
    printf("Heap (%p):\n", heap_listp);
  }

  if ((GET_SIZE(HDRP(heap_listp)) != DSIZE) || !GET_ALLOC(HDRP(heap_listp))) {
	printf("Bad prologue header\n");
  }
  checkblock(heap_listp);

  for (bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)) {
    if (verbose)  {
      printblock(bp);
    }
    checkblock(bp);
  }
     
  if (verbose) {
    printblock(bp);
  }

  if ((GET_SIZE(HDRP(bp)) != 0) || !(GET_ALLOC(HDRP(bp)))) {
    printf("Bad epilogue header\n");
  }
}

static void printblock(void *bp) 
{
  uint32_t hsize, halloc, fsize, falloc;

  hsize = GET_SIZE(HDRP(bp));
  halloc = GET_ALLOC(HDRP(bp));  
  fsize = GET_SIZE(FTRP(bp));
  falloc = GET_ALLOC(FTRP(bp));  
    
  if (hsize == 0) {
    printf("%p: EOL\n", bp);
    return;
  }

  printf("%p: header: [%d:%c] footer: [%d:%c]\n",
	 bp, 
	 (int) hsize, (halloc ? 'a' : 'f'), 
	 (int) fsize, (falloc ? 'a' : 'f')); 
}

static void checkblock(void *bp) 
{
  if ((uintptr_t)bp % 8) {
    printf("Error: %p is not doubleword aligned\n", bp);
  }
  if (GET(HDRP(bp)) != GET(FTRP(bp))) {
    printf("Error: header does not match footer\n");
  }
}

