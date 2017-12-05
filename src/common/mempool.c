/*
 *
 * Copyright (C) 2008 , 2009 yanyg
 *
 * submodule : general-purpose component of application
 * filename  : mempool.h
 * depict    : memory pool implementation
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

#include <pthread.h>  /* for pthread_mutex_*, etc. */
#include <string.h>   /* for memset route */

#include <ycc/common/mempool.h>

typedef struct mempool
{
  /* please do not modify the item anytime ! */
  void* s_pData;
}
struct_mempool;

#define MEMPOOL_NUMOBJ (10)
#define MEMPOOL_ALIGN (4)
#define MEMPOOL_LIST_NUM (256)
#define MEMPOOL_BYTE_MAX (MEMPOOL_ALIGN*MEMPOOL_LIST_NUM)

typedef union mempool_obj
{
  union mempool_obj* link;
  char data[1];
}
union_mempool_obj;

#define MEMPOOL_ALIGN_SIZE(x)  (((x)+((size_t)MEMPOOL_ALIGN-1))&(~((size_t)MEMPOOL_ALIGN-1)))
#define MEMPOOL_LIST_INDEX(x)  ((MEMPOOL_ALIGN_SIZE(x))/((size_t)MEMPOOL_ALIGN)-1)
#define MEMPOOL_OBJ_POINTER(x) ((union_mempool_obj*)(x))

#define MAX_SIZE(x,y)   (((x)>(y))?(x):(y))

static union_mempool_obj* theMPL[MEMPOOL_LIST_NUM] =
{
    /* 0 ~ 128 */
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    /* 129 ~ 256 */
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

#define mempool_mutex_t pthread_mutex_t
#define mempool_mutex_init(plock) pthread_mutex_init(plock,NULL)
#define mempool_mutex_lock(plock) pthread_mutex_lock(plock)
#define mempool_mutex_unlock(plock) pthread_mutex_unlock(plock)
#define mempool_mutex_destroy(plock) pthread_mutex_destroy(plock)

/* global memory pool file-level route */
static void* mempool_refill(size_t);
static char* mempool_chunk_alloc(size_t,size_t*);

// this parameters just used for mempool_allocate/mempool_deallocate
// please take attention about it !!!
static mempool_mutex_t mplock = PTHREAD_MUTEX_INITIALIZER;

/* multi threads, global memory pool */
void* mempool_alloc_mt(size_t size)
{
  void* pRtn = 0;

  /* the 4 bytes of buffer always save the buffer size */

  if ( size <= MEMPOOL_BYTE_MAX )
  {
    union_mempool_obj** pList = theMPL+MEMPOOL_LIST_INDEX(size);

    mempool_mutex_lock(&mplock);

    union_mempool_obj* pResult = *pList;
    if ( pResult )
    {
      *pList = pResult->link;
      pRtn = pResult;
    }
    else
    {
      pRtn = mempool_refill(MEMPOOL_ALIGN_SIZE(size));
    }

    mempool_mutex_unlock(&mplock);
  }
  else
  {
    pRtn = malloc(size);
  }

  return pRtn;
}

void* mempool_calloc_mt(size_t size)
{
  void* pRtn = mempool_alloc_mt(size);

  if ( pRtn ) memset(pRtn, 0, size);

  return pRtn;
}

void mempool_dealloc_mt(void* p, size_t size)
{
  if ( p )
  {
    if ( size <= MEMPOOL_BYTE_MAX )
    {
      union_mempool_obj** pList = theMPL+MEMPOOL_LIST_INDEX(size);

      mempool_mutex_lock(&mplock);

      MEMPOOL_OBJ_POINTER(p)->link = *pList;
      *pList = MEMPOOL_OBJ_POINTER(p);

      mempool_mutex_unlock(&mplock);
    }
    else
    {
      free(p);
    }
  }
}

/* single thread, global memory pool */
void* mempool_alloc_st(size_t size)
{
  void* pRtn = 0;

  if ( size <= MEMPOOL_BYTE_MAX )
  {
    union_mempool_obj** pList = theMPL+MEMPOOL_LIST_INDEX(size);
    union_mempool_obj* pResult = *pList;
    if ( pResult )
    {
      *pList = pResult->link;
      pRtn = pResult;
    }
    else
    {
      pRtn = mempool_refill(MEMPOOL_ALIGN_SIZE(size));
    }
  }
  else
  {
    pRtn = malloc(size);
  }

  return pRtn;
}


void* mempool_calloc_st(size_t size)
{
  void* pRtn = mempool_alloc_st(size);

  if ( pRtn ) memset(pRtn, 0, size);

  return pRtn;
}

void mempool_dealloc_st(void* p, size_t size)
{
  if ( p )
  {
    if ( size <= MEMPOOL_BYTE_MAX )
    {
      union_mempool_obj** pList = theMPL+MEMPOOL_LIST_INDEX(size);
      MEMPOOL_OBJ_POINTER(p)->link = *pList;
      *pList = MEMPOOL_OBJ_POINTER(p);
    }
    else
    {
      free(p);
    }
  }
}

void* mempool_alloc_no(size_t size)
{
  return malloc(size);
}

void* mempool_calloc_no(size_t size)
{
  return calloc(1,size);
}

void mempool_dealloc_no(void* p, size_t size)
{
  free(p);
}

/* global memory pool, file-level route */
void* mempool_refill(size_t size)
{
  void* pRtn = 0;

  size_t szNumObjs = MEMPOOL_NUMOBJ;

  char* pChunk = mempool_chunk_alloc(size,&szNumObjs);

  if ( pChunk )
  {
    pRtn = pChunk;

    /* build free list in chunk */
    if ( 1 < szNumObjs )
    {
      union_mempool_obj *pCur, *pNext;

      pNext = MEMPOOL_OBJ_POINTER(pChunk+size);
      theMPL[MEMPOOL_LIST_INDEX(size)] = pNext;

      int idx;
      for ( idx=1,--szNumObjs; idx < szNumObjs; ++idx )
      {
        pCur = pNext;
        pNext = MEMPOOL_OBJ_POINTER( ((char*)pNext)+size );
        pCur->link = pNext;
      }
      pNext->link = 0;
    }
  }

  return pRtn;
}

static char* mempool_chunk_alloc(size_t size, size_t* pszNumObjs)
{
  static char* pMPBegFree = 0;
  static char* pMPEndFree = 0;
  static size_t szHeapSize = 0;

  char* pRtn = 0;

  size_t szAllBytes = size*(*pszNumObjs);
  size_t szLeftBytes  = pMPEndFree - pMPBegFree;

  if ( szLeftBytes >= szAllBytes )
  {
    pRtn = pMPBegFree;
    pMPBegFree += szAllBytes;
  }
  else if ( szLeftBytes > size )
  {
    *pszNumObjs = (size_t)(szLeftBytes/size);
    szAllBytes = size*(*pszNumObjs);
    pRtn = pMPBegFree;
    pMPBegFree += szAllBytes;
  }
  else
  {
    size_t szToGetBytes = 2*szAllBytes+MEMPOOL_ALIGN_SIZE(szHeapSize>>4);

    if ( szLeftBytes > 0 )
    {
      union_mempool_obj** pList = theMPL+MEMPOOL_LIST_INDEX(szLeftBytes);
      MEMPOOL_OBJ_POINTER(pMPBegFree)->link = *pList;
      *pList = MEMPOOL_OBJ_POINTER(pMPBegFree);
    }

    pMPBegFree = malloc(szToGetBytes);

    if ( pMPBegFree )
    {
      szHeapSize += szToGetBytes;
      pMPEndFree = pMPBegFree + szToGetBytes;
      pRtn = mempool_chunk_alloc(size,pszNumObjs);
    }
    else
    {
      size_t szCur;
      union_mempool_obj** pList;
      union_mempool_obj* pObj;

      for ( szCur = size; szCur <= MEMPOOL_BYTE_MAX; szCur += MEMPOOL_ALIGN )
      {
        pList = theMPL+MEMPOOL_LIST_INDEX(szCur);
        pObj = *pList;
        if (pObj)
        {
          *pList = pObj->link;
          pMPBegFree = (char*)pObj;
          pMPEndFree = pMPBegFree+szCur;
          pRtn = mempool_chunk_alloc(size,pszNumObjs);
          break;
        }
      }

      if ( 0 == pRtn )
      {
        *pszNumObjs = 1;
        pRtn = malloc(size);
        pMPEndFree = 0;
      }
    }
  }

  return pRtn;
}

/* struct mempool */

/* multi threads structure memory pool */

typedef struct {
  size_t s_szPool;
  union_mempool_obj* s_pObj;
  mempool_mutex_t s_mtLock;
}
struct_mempool_mt;

/* single thread structure memory pool */
typedef struct
{
  size_t s_szPool;
  union_mempool_obj* s_pObj;
}
struct_mempool_st;

#define MPT2MT(thisPool)  ((struct_mempool_mt*)thisPool)
#define MPT2ST(thisPool)  ((struct_mempool_st*)thisPool)

/* Note: closed mempool use literal value */

/* structure memory pool operator route */
/* multi-threads */
static bool struct_mempool_init_mt(size_t szPool, mempool_t* pPool);
static void* struct_mempool_alloc_mt(mempool_t thisPool);
static void struct_mempool_dealloc_mt(mempool_t thisPool, void* p);
static void struct_mempool_clean_mt(mempool_t thisPool);
static void struct_mempool_destroy_mt(mempool_t thisPool);
/* single-thread */
static bool struct_mempool_init_st(size_t szPool, mempool_t* pPool);
static void* struct_mempool_alloc_st(mempool_t thisPool);
static void struct_mempool_dealloc_st(mempool_t thisPool, void* p);
static void struct_mempool_clean_st(mempool_t thisPool);
static void struct_mempool_destroy_st(mempool_t thisPool);
/* closed */
static bool struct_mempool_init_no(size_t szPool, mempool_t* pPool);
static void* struct_mempool_allocate_no(mempool_t thisPool);
static void struct_mempool_deallocate_no(mempool_t thisPool, void* p);
static void struct_mempool_clean_no(mempool_t thisPool);
static void struct_mempool_destroy_no(mempool_t thisPool);

const mempool_op_t g_mop_mt =
{
  .init = struct_mempool_init_mt,
  .clean = struct_mempool_clean_mt,
  .destroy = struct_mempool_destroy_mt,
  .alloc = struct_mempool_alloc_mt,
  .dealloc = struct_mempool_dealloc_mt
};

const mempool_op_t g_mop_st =
{
  .init = struct_mempool_init_st,
  .clean = struct_mempool_clean_st,
  .destroy = struct_mempool_destroy_st,
  .alloc = struct_mempool_alloc_st,
  .dealloc = struct_mempool_dealloc_st
};

const mempool_op_t g_mop_no =
{
  .init = struct_mempool_init_no,
  .clean = struct_mempool_clean_no,
  .destroy = struct_mempool_destroy_no,
  .alloc = struct_mempool_allocate_no,
  .dealloc = struct_mempool_deallocate_no
};

/* multi-thread */
bool struct_mempool_init_mt(size_t szPool, mempool_t* pPool)
{
  mempool_t thisPool = mempool_alloc_mt(sizeof(*MPT2MT(0)));

  if ( MPT2MT(thisPool) )
  {
    MPT2MT(thisPool)->s_szPool = MAX_SIZE(szPool, sizeof(union_mempool_obj));
    MPT2MT(thisPool)->s_pObj    = NULL;
    mempool_mutex_init(&MPT2MT(thisPool)->s_mtLock);

    *pPool = thisPool;
  }

  return NULL != thisPool;
}

void struct_mempool_clean_mt(mempool_t thisPool)
{
  mempool_mutex_lock(&MPT2MT(thisPool)->s_mtLock);

  if ( MPT2MT(thisPool)->s_pObj )
  {
    union_mempool_obj *pFree, *pObj = MPT2MT(thisPool)->s_pObj;

    do
    {
      pFree = pObj;
      pObj  = pFree->link;
      free(pFree);
    }
    while( pObj );

    MPT2MT(thisPool)->s_pObj = NULL;
  }

  mempool_mutex_unlock(&MPT2MT(thisPool)->s_mtLock);
}

void struct_mempool_destroy_mt(mempool_t thisPool)
{
  struct_mempool_clean_mt(thisPool);
  mempool_mutex_destroy(&MPT2MT(thisPool)->s_mtLock);
  mempool_dealloc_mt(thisPool, sizeof(*MPT2MT(thisPool)));
}

void* struct_mempool_alloc_mt(mempool_t thisPool)
{
  void* pRtn;

  mempool_mutex_lock(&MPT2MT(thisPool)->s_mtLock);

  union_mempool_obj* pMem = MPT2MT(thisPool)->s_pObj;
  if ( pMem )
  {
    pRtn = pMem;
    MPT2MT(thisPool)->s_pObj = pMem->link;
  }
  else
  {
    pRtn = malloc(MPT2MT(thisPool)->s_szPool);
  }

  mempool_mutex_unlock(&MPT2MT(thisPool)->s_mtLock);

  return pRtn;
}

void struct_mempool_dealloc_mt(mempool_t thisPool, void* p)
{
  if ( p )
  {
    mempool_mutex_lock(&MPT2MT(thisPool)->s_mtLock);

    union_mempool_obj* pHead = MPT2MT(thisPool)->s_pObj;
    MEMPOOL_OBJ_POINTER(p)->link = pHead;
    pHead = MEMPOOL_OBJ_POINTER(p);

    mempool_mutex_unlock(&MPT2MT(thisPool)->s_mtLock);
  }
}

/* single-thread */
bool struct_mempool_init_st(size_t szPool, mempool_t* pPool)
{
  mempool_t thisPool = mempool_alloc_mt(sizeof(*MPT2ST(0)));

  if ( MPT2ST(thisPool) )
  {
    MPT2ST(thisPool)->s_szPool = MAX_SIZE(szPool, sizeof(union_mempool_obj));
    MPT2ST(thisPool)->s_pObj    = NULL;

    *pPool = thisPool;
  }

  return NULL != thisPool;
}

void struct_mempool_clean_st(mempool_t thisPool)
{
  if ( MPT2ST(thisPool)->s_pObj )
  {
    union_mempool_obj *pFree, *pObj = MPT2ST(thisPool)->s_pObj;

    do
    {
      pFree = pObj;
      pObj  = pFree->link;
      free(pFree);
    }
    while( pObj );

    MPT2ST(thisPool)->s_pObj = NULL;
  }
}

void struct_mempool_destroy_st(mempool_t thisPool)
{
  struct_mempool_clean_st(thisPool);
  mempool_dealloc_mt(thisPool, sizeof(*MPT2ST(thisPool)));
}

void* struct_mempool_alloc_st(mempool_t thisPool)
{
  void* pRtn;

  union_mempool_obj* pMem = MPT2ST(thisPool)->s_pObj;
  if ( pMem )
  {
    pRtn = pMem;
    MPT2ST(thisPool)->s_pObj = pMem->link;
  }
  else
  {
    pRtn = malloc(MPT2ST(thisPool)->s_szPool);
  }

  return pRtn;
}

void struct_mempool_dealloc_st(mempool_t thisPool, void* p)
{
  if ( p )
  {
    union_mempool_obj* pHead = MPT2ST(thisPool)->s_pObj;
    MEMPOOL_OBJ_POINTER(p)->link = pHead;
    pHead = MEMPOOL_OBJ_POINTER(p);
  }
}

/* closed mempool */
bool struct_mempool_init_no(size_t szPool, mempool_t* pPool)
{
  *pPool = (mempool_t)szPool;
  return true;
}

void* struct_mempool_allocate_no(mempool_t thisPool)
{
  return malloc((size_t)thisPool);
}

void struct_mempool_deallocate_no(mempool_t thisPool, void* p)
{
  free(p);
}

void struct_mempool_clean_no(mempool_t thisPool)
{
}

void struct_mempool_destroy_no(mempool_t thisPool)
{
}

/*
 * end
 */
