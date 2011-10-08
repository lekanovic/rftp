#include <stdlib.h>
#include <string.h>
#include <assert.h>
/* from site:
 * http://stackoverflow.com/questions/852072/simple-c-implementation-to-track-memory-alloc-free */
 

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

		free(psize);
	}
}

