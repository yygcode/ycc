/*
 *
 * Copyright (C) yanyg/cppgp
 *
 * submodule : general-purpose component of application
 * filename  : mempool.h
 * depict    : memory pool interface
 * language  : ANSI C99
 * edition   : 3.0
 * compat    : linux
 * compile   : make(gnu gcc)
 * author    : yanyg
 * time      : 2008/04/25 ~ 2008/04/30 (1st edition)
 * modify    : 2008/06/06 (2nd edition)
 * modify    : 2009/10/12 (3nd edition)
 *
 */

#ifndef MEMPOOL_H_GPCA_CPPGP
#define MEMPOOL_H_GPCA_CPPGP

/*
 * notes
 *    compile your programe with -DOPEN_MEMPOOL_MT or -DOPEN_MEMPOOL_ST option(GNU GCC: -Dmacro[=defn])
 * the memory pool will be used
 *    else , it will be replaced by malloc/free
 *    and the memory pool have two mode :
 *     (1) afford memory which is less than 1020 bytes.
 *         please used mempool_allocate/mempool_deallocate.
 *     (2) afford any bytes memory.( and in normal, you should
 *         used when memory needs greater than 1024 bytes)
 *         please used struct mempool.(must call init function)
 *
 */

#include <stdbool.h>      /* for bool/true/false definition */
#include <stdlib.h>       /* for malloc/free router */
#include <sys/types.h>    /* for definition of size_t, etc. */

#ifdef __cplusplus  // for c++ language
extern "C"
{
#endif

/* mt means multi threads, st means single thread, and no means close the mempool */

#ifdef OPEN_MEMPOOL_MT
/* multi-thread memory pool */
#define mempool_alloc(size) mempool_alloc_mt(size)
#define mempool_calloc(size) mempool_calloc_mt(size)
#define mempool_dealloc(p,size) mempool_dealloc_mt(p,size)
#define mempool_alloc_fun mempool_alloc_mt
#define mempool_calloc_fun mempool_calloc_mt
#define mempool_dealloc_fun mempool_dealloc_mt
#define g_mop  g_mop_mt
#elif OPEN_MEMPOOL_ST
/* single-thread memory pool */
#define mempool_alloc(size) mempool_alloc_st(size)
#define mempool_calloc(size) mempool_calloc_st(size)
#define mempool_dealloc(p,size) mempool_dealloc_st(p,size)
#define mempool_alloc_fun mempool_alloc_st
#define mempool_calloc_fun mempool_calloc_st
#define mempool_dealloc_fun mempool_dealloc_st
#define g_mop  g_mop_st
#else
#define mempool_alloc(size) malloc(size)
#define mempool_calloc(size) calloc(1,size);
#define mempool_dealloc(p,size) free(p)
#define mempool_alloc_fun mempool_alloc_no
#define mempool_calloc_fun mempool_calloc_no
#define mempool_dealloc_fun mempool_dealloc_no
#define g_mop  g_mop_no
#endif

void* mempool_alloc_mt(size_t size);
void* mempool_calloc_mt(size_t size);
void mempool_dealloc_mt(void* p, size_t size);

void* mempool_alloc_st(size_t size);
void* mempool_calloc_st(size_t size);
void mempool_dealloc_st(void* p, size_t size);

void* mempool_alloc_no(size_t size);
void* mempool_calloc_no(size_t size);
void mempool_dealloc_no(void* p, size_t size);

/*
 * definition structure that prevents automatic type conversion
 * compiler alert 'incompatible type conversion or other')
 */
typedef struct{}* mempool_t;

/* encapsulation memory pool operator(op) route */
typedef struct {
  bool (*init)(size_t szPool, mempool_t* pPool);
  void (*clean)(mempool_t thisPool);
  void (*destroy)(mempool_t thisPool);

  void* (*alloc)(mempool_t thisPool);
  void (*dealloc)(mempool_t thisPool, void* p);
}
mempool_op_t;

extern const mempool_op_t g_mop_mt;
extern const mempool_op_t g_mop_st;
extern const mempool_op_t g_mop_no;

#ifdef __cplusplus  // for c++ language
}
#endif

#endif
