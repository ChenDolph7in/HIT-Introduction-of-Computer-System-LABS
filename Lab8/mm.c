/*
 * 学号 学生姓名
 * 双向空闲链表 + 基于边界标签的空闲快合并 + 首次适配
 * 基于CSAPP提供的implicit版本代码进行更改
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/* Team structure */
team_t team = {
    "双向空闲链表&基于边界标签的空闲快合并&首次适配",
    "Dave OHallaron", "droh",
    "", ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/* $begin mallocmacros */
/* Basic constants and macros */
#define WSIZE       4       /* word size (bytes) */
#define DSIZE       8       /* doubleword size (bytes) */
#define CHUNKSIZE  (1<<12)  /* initial heap size (bytes) */
#define OVERHEAD    8       /* overhead of header and footer (bytes) */

#define MAX(x, y) ((x) > (y)? (x) : (y))

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc)  ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p)       (*(size_t *)(p))
#define PUT(p, val)  (*(size_t *)(p) = (val))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p)  (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp)       ((char *)(bp) - WSIZE)
#define FTRP(bp)       ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp)  ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp)  ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

/* 额外添加 */
#define PREV_FB_P(bp)  ((char *)(bp))					//空闲块中指向上一个空闲块的指针地址
#define NEXT_FB_P(bp)	((char *)(bp) + WSIZE)				//空闲块中指向下一个空闲块的指针地址

#define PREV_FB(bp)  	((char *)(GET(PREV_FB_P(bp))))		//空闲链表里当前块上一个空闲块地址
#define NEXT_FB(bp)	((char *)(GET(NEXT_FB_P(bp))))		//空闲链表里当前块下一个空闲块地址

#define SET_P(p1,p2) 	(*(size_t *)(p1) = (size_t)(p2))   		//令地址p1作为指针指向地址p2

#define ROOT(list_p) 	((char *)(list_p)-DSIZE)			//空闲链表头指针，指向第一个空闲块
#define FIRST_FB(list_p) ((char *)(GET(ROOT(list_p))))		//第一个空闲块

/* $end mallocmacros */

/* Global variables */
static char *heap_listp;  /* pointer to first block */

/* function prototypes for internal helper routines */
static void *extend_heap(size_t words);
static void place(void *bp, size_t asize);
static void *find_fit(size_t asize);
static void *coalesce(void *bp);
static void printblock(void *bp);
static void checkblock(void *bp);
static void fbcheckout(void *bp);
static void fbcheckin(void *bp);
/*
 * mm_init - Initialize the memory manager
 */
/* $begin mminit */
int mm_init(void)
{
    /* create the initial empty heap */
    if ((heap_listp = mem_sbrk(4*WSIZE)) == NULL)
	return -1;
    SET_P(heap_listp, NULL);                        /* alignment padding */
    PUT(heap_listp+WSIZE, PACK(OVERHEAD, 1));  /* prologue header */
    PUT(heap_listp+DSIZE, PACK(OVERHEAD, 1));  /* prologue footer */
    PUT(heap_listp+WSIZE+DSIZE, PACK(0, 1));   /* epilogue header */
    heap_listp += DSIZE;
	
    /* Extend the empty heap with a free block of CHUNKSIZE bytes */
    if (extend_heap(CHUNKSIZE/WSIZE) == NULL)
	return -1;
    return 0;
}
/* $end mminit */

/*
 * mm_malloc - Allocate a block with at least size bytes of payload
 */
/* $begin mmmalloc */
void *mm_malloc(size_t size)
{
    size_t asize;      /* adjusted block size */
    size_t extendsize; /* amount to extend heap if no fit */
    char *bp;

    /* Ignore spurious requests */
    if (size <= 0)
	return NULL;

    /* Adjust block size to include overhead and alignment reqs. */
    if (size <= DSIZE)
	asize = DSIZE + OVERHEAD;
    else
	asize = DSIZE * ((size + (OVERHEAD) + (DSIZE-1)) / DSIZE);

    /* Search the free list for a fit */
    if ((bp = find_fit(asize)) != NULL) {
	place(bp, asize);
	return bp;
    }

    /* No fit found. Get more memory and place the block */
    extendsize = MAX(asize,CHUNKSIZE);
    if ((bp = extend_heap(extendsize/WSIZE)) == NULL)
	return NULL;
    place(bp, asize);
    return bp;
}
/* $end mmmalloc */

/*
 * mm_free - Free a block
 */
/* $begin mmfree */
void mm_free(void *bp)
{
    size_t size = GET_SIZE(HDRP(bp));

    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    coalesce(bp);
}

/* $end mmfree */

/*
 * mm_realloc - naive implementation of mm_realloc
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *newp;
    size_t copySize;

    if ((newp = mm_malloc(size)) == NULL) {
	printf("ERROR: mm_malloc failed in mm_realloc\n");
	exit(1);
    }
    copySize = GET_SIZE(HDRP(ptr));
    if (size < copySize)
      copySize = size;
    memcpy(newp, ptr, copySize);
    mm_free(ptr);
    return newp;
}

/*
 * mm_checkheap - Check the heap for consistency
 */
void mm_checkheap(int verbose)
{
    char *bp = heap_listp;

    if (verbose)
	printf("Heap (%p):\n", heap_listp);

    if ((GET_SIZE(HDRP(heap_listp)) != DSIZE) || !GET_ALLOC(HDRP(heap_listp)))
	printf("Bad prologue header\n");
    checkblock(heap_listp);

    for (bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)) {
	if (verbose) 
	    printblock(bp);
	checkblock(bp);
    }
     
    if (verbose)
	printblock(bp);
    if ((GET_SIZE(HDRP(bp)) != 0) || !(GET_ALLOC(HDRP(bp))))
	printf("Bad epilogue header\n");
}

/* The remaining routines are internal helper routines */

/*
 * extend_heap - Extend heap with free block and return its block pointer
 */
/* $begin mmextendheap */
static void *extend_heap(size_t words)
{
    char *bp;
    size_t size;

    /* Allocate an even number of words to maintain alignment */
    size = (words % 2) ? (words+1) * WSIZE : words * WSIZE;
    if ((bp = mem_sbrk(size)) == (void *)-1)
	return NULL;

    /* Initialize free block header/footer and the epilogue header */
    PUT(HDRP(bp), PACK(size, 0));         /* free block header */
    PUT(FTRP(bp), PACK(size, 0));         /* free block footer */
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); /* new epilogue header */
    
    /* Coalesce if the previous block was free */
    return coalesce(bp);
}
/* $end mmextendheap */

/*
 * place - Place block of asize bytes at start of free block bp
 *         and split if remainder would be at least minimum block size
 */
/* $begin mmplace */
/* $begin mmplace-proto */
static void place(void *bp, size_t asize)
/* $end mmplace-proto */
{
    size_t csize = GET_SIZE(HDRP(bp));

    if ((csize - asize) >= (DSIZE + OVERHEAD)) {
	PUT(HDRP(bp), PACK(asize, 1));
	PUT(FTRP(bp), PACK(asize, 1));
 
	char *bp_rest = NEXT_BLKP(bp);
	PUT(HDRP(bp_rest), PACK(csize-asize, 0));
	PUT(FTRP(bp_rest), PACK(csize-asize, 0));
	
	char *prev = PREV_FB(bp);
	char *next = NEXT_FB(bp);
	
	if(prev!=NULL&&next!=NULL){
	    SET_P(PREV_FB_P(bp_rest), prev);
    	    SET_P(NEXT_FB_P(bp_rest), next);
    	    SET_P(NEXT_FB_P(prev), bp_rest);
    	    SET_P(PREV_FB_P(next), bp_rest);
	}
	else if(prev!=NULL&&next==NULL){
	    SET_P(PREV_FB_P(bp_rest), prev);
    	    SET_P(NEXT_FB_P(bp_rest), NULL);
    	    SET_P(NEXT_FB_P(prev), bp_rest);
	}
	else if(prev==NULL&&next!=NULL){
	    SET_P(PREV_FB_P(bp_rest), NULL);
    	    SET_P(NEXT_FB_P(bp_rest), next);
    	    SET_P(PREV_FB_P(next), bp_rest);
    	    SET_P(ROOT(heap_listp), bp_rest);
	}
	else{
	    SET_P(ROOT(heap_listp), bp_rest);
	    SET_P(PREV_FB_P(bp_rest), NULL);
    	    SET_P(NEXT_FB_P(bp_rest), NULL);
	}
    }
    else {
	PUT(HDRP(bp), PACK(csize, 1));
	PUT(FTRP(bp), PACK(csize, 1));
	/* 将空闲块直接脱离空闲链表 */
    	fbcheckout(bp);
    }
}
/* $end mmplace */

/*
 * find_fit - Find a fit for a block with asize bytes
 */
static void *find_fit(size_t asize)
{
    /* first fit search */
    void *bp;

    if(FIRST_FB(heap_listp)==NULL)
    	return NULL;
    for (bp = FIRST_FB(heap_listp); bp != NULL; bp = NEXT_FB(bp)) {
	if (asize <= GET_SIZE(HDRP(bp))) {
	    return bp;
	}
    }
    return NULL; /* no fit */
}

/*
 * coalesce - boundary tag coalescing. Return ptr to coalesced block
 */
static void *coalesce(void *bp)
{
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));

    if (prev_alloc && next_alloc) {            /* Case 1 */
    	fbcheckin(bp);
    }

    else if (prev_alloc && !next_alloc) {      /* Case 2 */
    	/* 将后一个空闲块脱离空闲链表 */
    	fbcheckout(NEXT_BLKP(bp));
    	/* 合并空闲块 */
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
        /* 将合并后的空闲块插到空闲链表头*/
        fbcheckin(bp);
    }

    else if (!prev_alloc && next_alloc) {      /* Case 3 */
    	/* 将前一个空闲块脱离空闲链表 */
    	fbcheckout(PREV_BLKP(bp));
    	/* 合并空闲块 */
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(PREV_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
	/* 将合并后的空闲块插到空闲链表头*/
	fbcheckin(bp);
    }

    else {                                     /* Case 4 */
    	/* 将前一个空闲块脱离空闲链表 */
    	fbcheckout(PREV_BLKP(bp));
    	/* 将后一个空闲块脱离空闲链表 */
    	fbcheckout(NEXT_BLKP(bp));
    	/* 合并空闲块 */
	size += GET_SIZE(HDRP(PREV_BLKP(bp))) +
	    GET_SIZE(FTRP(NEXT_BLKP(bp)));
	SET_P(HDRP(PREV_BLKP(bp)), PACK(size, 0));
	SET_P(FTRP(PREV_BLKP(bp)), PACK(size, 0));
	bp = PREV_BLKP(bp);
	/* 将合并后的空闲块插到空闲链表头*/
	fbcheckin(bp);
    }
    return bp;
}



static void printblock(void *bp)
{
    size_t hsize, halloc, fsize, falloc;

    hsize = GET_SIZE(HDRP(bp));
    halloc = GET_ALLOC(HDRP(bp));
    fsize = GET_SIZE(FTRP(bp));
    falloc = GET_ALLOC(FTRP(bp));

    if (hsize == 0) {
	printf("%p: EOL\n", bp);
	return;
    }

    printf("%p: header: [%d:%c] footer: [%d:%c]\n", bp,
	   hsize, (halloc ? 'a' : 'f'),
	   fsize, (falloc ? 'a' : 'f'));
}

static void checkblock(void *bp)
{
    if ((size_t)bp % 8)
	printf("Error: %p is not doubleword aligned\n", bp);
    if (GET(HDRP(bp)) != GET(FTRP(bp)))
	printf("Error: header does not match footer\n");
}

static void fbcheckout(void *bp)/* 将空闲块脱离空闲链表 */
{
    char *prev = PREV_FB(bp);
    char *next = NEXT_FB(bp);
    if(prev!=NULL&&next!=NULL){
    	SET_P(NEXT_FB_P(prev),next);
    	SET_P(PREV_FB_P(next),prev);
    }
    else if(prev==NULL&&next!=NULL){
    	SET_P(PREV_FB_P(next),NULL);
    	SET_P(ROOT(heap_listp),next);
    }
    else if(prev!=NULL&&next==NULL){
    	SET_P(NEXT_FB_P(prev),NULL);
    }
    else{
    	SET_P(ROOT(heap_listp),NULL);
    }
}

static void fbcheckin(void *bp)/* 将空闲块插入空闲链表表头 */
{
    char *origin_first = FIRST_FB(heap_listp);/* 原表头 */
    	
    if(origin_first == NULL){	/* 原表删除刚刚删除的空闲块后为空 */
    	SET_P(ROOT(heap_listp), bp);
    	SET_P(PREV_FB_P(bp), NULL);
    	SET_P(NEXT_FB_P(bp), NULL);
    }
    else{			/* 原表删除刚刚删除的空闲块后非空 */
    	SET_P(ROOT(heap_listp), bp);
    	SET_P(PREV_FB_P(bp), NULL);
    	SET_P(NEXT_FB_P(bp), origin_first);
    	SET_P(PREV_FB_P(origin_first), bp);
    }
}











