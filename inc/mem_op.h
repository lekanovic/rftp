#ifndef __MEM_OP_H__
#define __MEM_OP_H__
size_t get_used_mem();
size_t peak_mem();
void* ftp_alloc(size_t);
void  ftp_free(void*);
#endif
