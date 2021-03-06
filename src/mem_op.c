#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "mem_op.h"
/* from site:
 * http://stackoverflow.com/questions/852072/simple-c-implementation-to-track-memory-alloc-free */

extern int debug_mode;

#define debug_print(args ...) \
	if (debug_mode) { \
		fprintf(stderr, args); \
		printf("total mem usage: %d\n",(int)get_used_mem()); \
	}


static size_t currentmemory	= 0;
static size_t peakmemory	= 0;

size_t get_used_mem()
{
	return currentmemory;
}
size_t peak_mem()
{
	return peakmemory;
}

void *ftp_calloc(size_t nmemb, size_t size)
{
	int s;
	void* pmem = calloc(sizeof(size_t) + nmemb, size);

	s = (nmemb * size);

	if (pmem) {
		size_t *psize = (size_t *)pmem;

		memcpy(psize,&s,sizeof(size));

		currentmemory += (nmemb * size);

		if (currentmemory > peakmemory) {
			peakmemory = currentmemory;
		}
		debug_print("calloc: size %d addr %p\n",s,(psize+1));
		return (psize + 1);
	}

	return NULL;
}

void *ftp_alloc(size_t size)
{
	void *pmem = malloc(sizeof(size_t) + size);

	if (pmem) {
		size_t *psize = (size_t *)pmem;

		memcpy(psize, &size, sizeof(size));

		currentmemory += size;

		if (currentmemory > peakmemory) {
			peakmemory = currentmemory;
		}
		debug_print("alloc: size %d addr %p\n",(int)size,(psize+1));
		return(psize + 1);
	}

	return NULL;
}

void  ftp_free(void *pmem)
{
	if(pmem) {
		size_t *psize = (size_t *)pmem;
		// Get the size
		--psize;

		//assert(currentmemory >= *psize);

		currentmemory -= *psize;
		debug_print("free: size %d addr %p\n",(int)*psize,psize);
		free(psize);
	}
}

