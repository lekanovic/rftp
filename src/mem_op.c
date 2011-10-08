#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
/* from site:
 * http://stackoverflow.com/questions/852072/simple-c-implementation-to-track-memory-alloc-free */

#define DEBUG 0

#define debug_print(args ...) if (DEBUG) fprintf(stderr, args)

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
		debug_print("alloc: %p\n",(psize + 1));
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

		assert(currentmemory >= *psize);

		currentmemory -= *psize;
		debug_print("free: %p\n",psize);
		free(psize);
	}
}

