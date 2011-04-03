/* Alternative malloc implementation for multiple threads without
lock contention based on dlmalloc. (C) 2005-2006 Niall Douglas

Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#ifdef _MSC_VER
/* Enable full aliasing on MSVC */
/*#pragma optimize("a", on)*/
#endif

/*#define FULLSANITYCHECKS*/

#if defined(__GNUC__)
 #include <limits.h>
#endif
#include "nedmalloc.h"
#ifdef WIN32
 #include <malloc.h>
#endif
#define MSPACES 1
#define ONLY_MSPACES 1
#ifndef USE_LOCKS
 #define USE_LOCKS 1
#endif
#define FOOTERS 1           /* Need to enable footers so frees lock the right mspace */
#undef DEBUG				/* dlmalloc wants DEBUG either 0 or 1 */
#ifdef _DEBUG
 #define DEBUG 1
#else
 #define DEBUG 0
#endif
#ifdef NDEBUG               /* Disable assert checking on release builds */
 #undef DEBUG
#endif
/* The default of 64Kb means we spend too much time kernel-side */
#ifndef DEFAULT_GRANULARITY
#define DEFAULT_GRANULARITY (1*1024*1024)
#endif
/*#define USE_SPIN_LOCKS 0*/


/*#define FORCEINLINE*/
#include "malloc.c.h"
#ifdef NDEBUG               /* Disable assert checking on release builds */
 #undef DEBUG
#endif

/* The maximum concurrent threads in a pool possible */
#ifndef MAXTHREADSINPOOL
#define MAXTHREADSINPOOL 16
#endif
/* The maximum number of threadcaches which can be allocated */
#ifndef THREADCACHEMAXCACHES
#define THREADCACHEMAXCACHES 256
#endif
/* The maximum size to be allocated from the thread cache */
#ifndef THREADCACHEMAX
#define THREADCACHEMAX 8192
#endif
#if 0
/* The number of cache entries for finer grained bins. This is (topbitpos(THREADCACHEMAX)-4)*2 */
#define THREADCACHEMAXBINS ((13-4)*2)
#else
/* The number of cache entries. This is (topbitpos(THREADCACHEMAX)-4) */
#define THREADCACHEMAXBINS (13-4)
#endif
/* Point at which the free space in a thread cache is garbage collected */
#ifndef THREADCACHEMAXFREESPACE
#define THREADCACHEMAXFREESPACE (512*1024)
#endif


#ifdef WIN32
 #define TLSVAR			DWORD
 #define TLSALLOC(k)	(*(k)=TlsAlloc(), TLS_OUT_OF_INDEXES==*(k))
 #define TLSFREE(k)		(!TlsFree(k))
 #define TLSGET(k)		TlsGetValue(k)
 #define TLSSET(k, a)	(!TlsSetValue(k, a))
 #ifdef DEBUG
static LPVOID ChkedTlsGetValue(DWORD idx)
{
	LPVOID ret=TlsGetValue(idx);
	assert(S_OK==GetLastError());
	return ret;
}
  #undef TLSGET
  #define TLSGET(k) ChkedTlsGetValue(k)
 #endif
#else
 #define TLSVAR			pthread_key_t
 #define TLSALLOC(k)	pthread_key_create(k, 0)
 #define TLSFREE(k)		pthread_key_delete(k)
 #define TLSGET(k)		pthread_getspecific(k)
 #define TLSSET(k, a)	pthread_setspecific(k, a)
#endif

#if 0
/* Only enable if testing with valgrind. Causes misoperation */
#define mspace_malloc(p, s) malloc(s)
#define mspace_realloc(p, m, s) realloc(m, s)
#define mspace_calloc(p, n, s) calloc(n, s)
#define mspace_free(p, m) free(m)
#endif


#if defined(__cplusplus)
#if !defined(NO_NED_NAMESPACE)
namespace nedalloc {
#else
extern "C" {
#endif
#endif

size_t nedblksize(void *mem) THROWSPEC
{
#if 0
	/* Only enable if testing with valgrind. Causes misoperation */
	return THREADCACHEMAX;
#else
	if(mem)
	{
		mchunkptr p=mem2chunk(mem);
		assert(cinuse(p));	/* If this fails, someone tried to free a block twice */
		if(cinuse(p))
			return chunksize(p)-overhead_for(p);
	}
	return 0;
#endif
}

void nedsetvalue(void *v) THROWSPEC					{ nedpsetvalue(0, v); }
void * nedmalloc(size_t size) THROWSPEC				{ return nedpmalloc(0, size); }
void * nedcalloc(size_t no, size_t size) THROWSPEC	{ return nedpcalloc(0, no, size); }
void * nedrealloc(void *mem, size_t size) THROWSPEC	{ return nedprealloc(0, mem, size); }
void   nedfree(void *mem) THROWSPEC					{ nedpfree(0, mem); }
void * nedmemalign(size_t alignment, size_t bytes) THROWSPEC { return nedpmemalign(0, alignment, bytes); }
#if !NO_MALLINFO
struct mallinfo nedmallinfo(void) THROWSPEC			{ return nedpmallinfo(0); }
#endif
int    nedmallopt(int parno, int value) THROWSPEC	{ return nedpmallopt(0, parno, value); }
int    nedmalloc_trim(size_t pad) THROWSPEC			{ return nedpmalloc_trim(0, pad); }
void   nedmalloc_stats() THROWSPEC					{ nedpmalloc_stats(0); }
size_t nedmalloc_footprint() THROWSPEC				{ return nedpmalloc_footprint(0); }
void **nedindependent_calloc(size_t elemsno, size_t elemsize, void **chunks) THROWSPEC	{ return nedpindependent_calloc(0, elemsno, elemsize, chunks); }
void **nedindependent_comalloc(size_t elems, size_t *sizes, void **chunks) THROWSPEC	{ return nedpindependent_comalloc(0, elems, sizes, chunks); }

struct threadcacheblk_t;
typedef struct threadcacheblk_t threadcacheblk;
struct threadcacheblk_t
{	/* Keep less than 16 bytes on 32 bit systems and 32 bytes on 64 bit systems */
#ifdef FULLSANITYCHECKS
	unsigned int magic;
#endif
	unsigned int lastUsed, size;
	threadcacheblk *next, *prev;
};
typedef struct threadcache_t
{
#ifdef FULLSANITYCHECKS
	unsigned int magic1;
#endif
	int mymspace;						/* Last mspace entry this thread used */
	long threadid;
	unsigned int mallocs, frees, successes;
	size_t freeInCache;					/* How much free space is stored in this cache */
	threadcacheblk *bins[(THREADCACHEMAXBINS+1)*2];
#ifdef FULLSANITYCHECKS
	unsigned int magic2;
#endif
} threadcache;
struct nedpool_t
{
	MLOCK_T mutex;
	void *uservalue;
	int threads;						/* Max entries in m to use */
	threadcache *caches[THREADCACHEMAXCACHES];
	TLSVAR mycache;						/* Thread cache for this thread. 0 for unset, negative for use mspace-1 directly, otherwise is cache-1 */
	mstate m[MAXTHREADSINPOOL+1];		/* mspace entries for this pool */
};
static nedpool syspool;

static FORCEINLINE unsigned int size2binidx(size_t _size) THROWSPEC
{	/* 8=1000	16=10000	20=10100	24=11000	32=100000	48=110000	4096=1000000000000 */
	unsigned int topbit, size=(unsigned int)(_size>>4);
	/* 16=1		20=1	24=1	32=10	48=11	64=100	96=110	128=1000	4096=100000000 */

#if defined(__GNUC__)
        topbit = sizeof(size)*CHAR_BIT - 1 - __builtin_clz(size);
#elif defined(_MSC_VER) && _MSC_VER>=1300
	{
            unsigned long bsrTopBit;

            _BitScanReverse(&bsrTopBit, size);

            topbit = bsrTopBit;
        }
#else
#if 0
	union {
		unsigned asInt[2];
		double asDouble;
	};
	int n;

	asDouble = (double)size + 0.5;
	topbit = (asInt[!FOX_BIGENDIAN] >> 20) - 1023;
#else
	{
		unsigned int x=size;
		x = x | (x >> 1);
		x = x | (x >> 2);
		x = x | (x >> 4);
		x = x | (x >> 8);
		x = x | (x >>16);
		x = ~x;
		x = x - ((x >> 1) & 0x55555555);
		x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
		x = (x + (x >> 4)) & 0x0F0F0F0F;
		x = x + (x << 8);
		x = x + (x << 16);
		topbit=31 - (x >> 24);
	}
#endif
#endif
	return topbit;
}


#ifdef FULLSANITYCHECKS
static void tcsanitycheck(threadcacheblk **ptr) THROWSPEC
{
	assert((ptr[0] && ptr[1]) || (!ptr[0] && !ptr[1]));
	if(ptr[0] && ptr[1])
	{
		assert(nedblksize(ptr[0])>=sizeof(threadcacheblk));
		assert(nedblksize(ptr[1])>=sizeof(threadcacheblk));
		assert(*(unsigned int *) "NEDN"==ptr[0]->magic);
		assert(*(unsigned int *) "NEDN"==ptr[1]->magic);
		assert(!ptr[0]->prev);
		assert(!ptr[1]->next);
		if(ptr[0]==ptr[1])
		{
			assert(!ptr[0]->next);
			assert(!ptr[1]->prev);
		}
	}
}
static void tcfullsanitycheck(threadcache *tc) THROWSPEC
{
	threadcacheblk **tcbptr=tc->bins;
	int n;
	for(n=0; n<=THREADCACHEMAXBINS; n++, tcbptr+=2)
	{
		threadcacheblk *b, *ob=0;
		tcsanitycheck(tcbptr);
		for(b=tcbptr[0]; b; ob=b, b=b->next)
		{
			assert(*(unsigned int *) "NEDN"==b->magic);
			assert(!ob || ob->next==b);
			assert(!ob || b->prev==ob);
		}
	}
}
#endif

static NOINLINE void RemoveCacheEntries(nedpool *p, threadcache *tc, unsigned int age) THROWSPEC
{
#ifdef FULLSANITYCHECKS
	tcfullsanitycheck(tc);
#endif
	if(tc->freeInCache)
	{
		threadcacheblk **tcbptr=tc->bins;
		int n;
		for(n=0; n<=THREADCACHEMAXBINS; n++, tcbptr+=2)
		{
			threadcacheblk **tcb=tcbptr+1;		/* come from oldest end of list */
			/*tcsanitycheck(tcbptr);*/
			for(; *tcb && tc->frees-(*tcb)->lastUsed>=age; )
			{
				threadcacheblk *f=*tcb;
				size_t blksize=f->size; /*nedblksize(f);*/
				assert(blksize<=nedblksize(f));
				assert(blksize);
#ifdef FULLSANITYCHECKS
				assert(*(unsigned int *) "NEDN"==(*tcb)->magic);
#endif
				*tcb=(*tcb)->prev;
				if(*tcb)
					(*tcb)->next=0;
				else
					*tcbptr=0;
				tc->freeInCache-=blksize;
				assert((long) tc->freeInCache>=0);
				mspace_free(0, f);
				/*tcsanitycheck(tcbptr);*/
			}
		}
	}
#ifdef FULLSANITYCHECKS
	tcfullsanitycheck(tc);
#endif
}
static void DestroyCaches(nedpool *p) THROWSPEC
{
	if(p->caches)
	{
		threadcache *tc;
		int n;
		for(n=0; n<THREADCACHEMAXCACHES; n++)
		{
			if((tc=p->caches[n]))
			{
				tc->frees++;
				RemoveCacheEntries(p, tc, 0);
				assert(!tc->freeInCache);
				tc->mymspace=-1;
				tc->threadid=0;
				mspace_free(0, tc);
				p->caches[n]=0;
			}
		}
	}
}

static NOINLINE threadcache *AllocCache(nedpool *p) THROWSPEC
{
	threadcache *tc=0;
	int n, end;
	ACQUIRE_LOCK(&p->mutex);
	for(n=0; n<THREADCACHEMAXCACHES && p->caches[n]; n++);
	if(THREADCACHEMAXCACHES==n)
	{	/* List exhausted, so disable for this thread */
		RELEASE_LOCK(&p->mutex);
		return 0;
	}
	tc=p->caches[n]=(threadcache *) mspace_calloc(p->m[0], 1, sizeof(threadcache));
	if(!tc)
	{
		RELEASE_LOCK(&p->mutex);
		return 0;
	}
#ifdef FULLSANITYCHECKS
	tc->magic1=*(unsigned int *)"NEDMALC1";
	tc->magic2=*(unsigned int *)"NEDMALC2";
#endif
	tc->threadid=(long)(size_t)CURRENT_THREAD;
	for(end=0; p->m[end]; end++);
	tc->mymspace=tc->threadid % end;
	RELEASE_LOCK(&p->mutex);
	if(TLSSET(p->mycache, (void *)(size_t)(n+1))) abort();
	return tc;
}

static void *threadcache_malloc(nedpool *p, threadcache *tc, size_t *size) THROWSPEC
{
	void *ret=0;
	unsigned int bestsize;
	unsigned int idx=size2binidx(*size);
	size_t blksize=0;
	threadcacheblk *blk, **binsptr;
#ifdef FULLSANITYCHECKS
	tcfullsanitycheck(tc);
#endif
	/* Calculate best fit bin size */
	bestsize=1<<(idx+4);
#if 0
	/* Finer grained bin fit */
	idx<<=1;
	if(*size>bestsize)
	{
		idx++;
		bestsize+=bestsize>>1;
	}
	if(*size>bestsize)
	{
		idx++;
		bestsize=1<<(4+(idx>>1));
	}
#else
	if(*size>bestsize)
	{
		idx++;
		bestsize<<=1;
	}
#endif
	assert(bestsize>=*size);
	if(*size<bestsize) *size=bestsize;
	assert(*size<=THREADCACHEMAX);
	assert(idx<=THREADCACHEMAXBINS);
	binsptr=&tc->bins[idx*2];
	/* Try to match close, but move up a bin if necessary */
	blk=*binsptr;
	if(!blk || blk->size<*size)
	{	/* Bump it up a bin */
		if(idx<THREADCACHEMAXBINS)
		{
			idx++;
			binsptr+=2;
			blk=*binsptr;
		}
	}
	if(blk)
	{
		blksize=blk->size; /*nedblksize(blk);*/
		assert(nedblksize(blk)>=blksize);
		assert(blksize>=*size);
		if(blk->next)
			blk->next->prev=0;
		*binsptr=blk->next;
		if(!*binsptr)
			binsptr[1]=0;
#ifdef FULLSANITYCHECKS
		blk->magic=0;
#endif
		assert(binsptr[0]!=blk && binsptr[1]!=blk);
		assert(nedblksize(blk)>=sizeof(threadcacheblk) && nedblksize(blk)<=THREADCACHEMAX+CHUNK_OVERHEAD);
		/*printf("malloc: %p, %p, %p, %lu\n", p, tc, blk, (long) size);*/
		ret=(void *) blk;
	}
	++tc->mallocs;
	if(ret)
	{
		assert(blksize>=*size);
		++tc->successes;
		tc->freeInCache-=blksize;
		assert((long) tc->freeInCache>=0);
	}
#if defined(DEBUG) && 0
	if(!(tc->mallocs & 0xfff))
	{
		printf("*** threadcache=%u, mallocs=%u (%f), free=%u (%f), freeInCache=%u\n", (unsigned int) tc->threadid, tc->mallocs,
			(float) tc->successes/tc->mallocs, tc->frees, (float) tc->successes/tc->frees, (unsigned int) tc->freeInCache);
	}
#endif
#ifdef FULLSANITYCHECKS
	tcfullsanitycheck(tc);
#endif
	return ret;
}
static NOINLINE void ReleaseFreeInCache(nedpool *p, threadcache *tc, int mymspace) THROWSPEC
{
	unsigned int age=THREADCACHEMAXFREESPACE/8192;
	/*ACQUIRE_LOCK(&p->m[mymspace]->mutex);*/
	while(age && tc->freeInCache>=THREADCACHEMAXFREESPACE)
	{
		RemoveCacheEntries(p, tc, age);
		/*printf("*** Removing cache entries older than %u (%u)\n", age, (unsigned int) tc->freeInCache);*/
		age>>=1;
	}
	/*RELEASE_LOCK(&p->m[mymspace]->mutex);*/
}
static void threadcache_free(nedpool *p, threadcache *tc, int mymspace, void *mem, size_t size) THROWSPEC
{
	unsigned int bestsize;
	unsigned int idx=size2binidx(size);
	threadcacheblk **binsptr, *tck=(threadcacheblk *) mem;
	assert(size>=sizeof(threadcacheblk) && size<=THREADCACHEMAX+CHUNK_OVERHEAD);
#ifdef DEBUG
	{	/* Make sure this is a valid memory block */
	    mchunkptr p  = mem2chunk(mem);
	    mstate fm = get_mstate_for(p);
	    if (!ok_magic(fm)) {
	      USAGE_ERROR_ACTION(fm, p);
	      return;
	    }
	}
#endif
#ifdef FULLSANITYCHECKS
	tcfullsanitycheck(tc);
#endif
	/* Calculate best fit bin size */
	bestsize=1<<(idx+4);
#if 0
	/* Finer grained bin fit */
	idx<<=1;
	if(size>bestsize)
	{
		unsigned int biggerbestsize=bestsize+bestsize<<1;
		if(size>=biggerbestsize)
		{
			idx++;
			bestsize=biggerbestsize;
		}
	}
#endif
	if(bestsize!=size)	/* dlmalloc can round up, so we round down to preserve indexing */
		size=bestsize;
	binsptr=&tc->bins[idx*2];
	assert(idx<=THREADCACHEMAXBINS);
	if(tck==*binsptr)
	{
		fprintf(stderr, "Attempt to free already freed memory block %p - aborting!\n", tck);
		abort();
	}
#ifdef FULLSANITYCHECKS
	tck->magic=*(unsigned int *) "NEDN";
#endif
	tck->lastUsed=++tc->frees;
	tck->size=(unsigned int) size;
	tck->next=*binsptr;
	tck->prev=0;
	if(tck->next)
		tck->next->prev=tck;
	else
		binsptr[1]=tck;
	assert(!*binsptr || (*binsptr)->size==tck->size);
	*binsptr=tck;
	assert(tck==tc->bins[idx*2]);
	assert(tc->bins[idx*2+1]==tck || binsptr[0]->next->prev==tck);
	/*printf("free: %p, %p, %p, %lu\n", p, tc, mem, (long) size);*/
	tc->freeInCache+=size;
#ifdef FULLSANITYCHECKS
	tcfullsanitycheck(tc);
#endif
#if 1
	if(tc->freeInCache>=THREADCACHEMAXFREESPACE)
		ReleaseFreeInCache(p, tc, mymspace);
#endif
}




static NOINLINE int InitPool(nedpool *p, size_t capacity, int threads) THROWSPEC
{	/* threads is -1 for system pool */
	ensure_initialization();
	ACQUIRE_MALLOC_GLOBAL_LOCK();
	if(p->threads) goto done;
	if(INITIAL_LOCK(&p->mutex)) goto err;
	if(TLSALLOC(&p->mycache)) goto err;
	if(!(p->m[0]=(mstate) create_mspace(capacity, 1))) goto err;
	p->m[0]->extp=p;
	p->threads=(threads<1 || threads>MAXTHREADSINPOOL) ? MAXTHREADSINPOOL : threads;
done:
	RELEASE_MALLOC_GLOBAL_LOCK();
	return 1;
err:
	if(threads<0)
		abort();			/* If you can't allocate for system pool, we're screwed */
	DestroyCaches(p);
	if(p->m[0])
	{
		destroy_mspace(p->m[0]);
		p->m[0]=0;
	}
	if(p->mycache)
	{
		if(TLSFREE(p->mycache)) abort();
		p->mycache=0;
	}
	RELEASE_MALLOC_GLOBAL_LOCK();
	return 0;
}
static NOINLINE mstate FindMSpace(nedpool *p, threadcache *tc, int *lastUsed, size_t size) THROWSPEC
{	/* Gets called when thread's last used mspace is in use. The strategy
	is to run through the list of all available mspaces looking for an
	unlocked one and if we fail, we create a new one so long as we don't
	exceed p->threads */
	int n, end;
	for(n=end=*lastUsed+1; p->m[n]; end=++n)
	{
		if(TRY_LOCK(&p->m[n]->mutex)) goto found;
	}
	for(n=0; n<*lastUsed && p->m[n]; n++)
	{
		if(TRY_LOCK(&p->m[n]->mutex)) goto found;
	}
	if(end<p->threads)
	{
		mstate temp;
		if(!(temp=(mstate) create_mspace(size, 1)))
			goto badexit;
		/* Now we're ready to modify the lists, we lock */
		ACQUIRE_LOCK(&p->mutex);
		while(p->m[end] && end<p->threads)
			end++;
		if(end>=p->threads)
		{	/* Drat, must destroy it now */
			RELEASE_LOCK(&p->mutex);
			destroy_mspace((mspace) temp);
			goto badexit;
		}
		/* We really want to make sure this goes into memory now but we
		have to be careful of breaking aliasing rules, so write it twice */
		*((volatile struct malloc_state **) &p->m[end])=p->m[end]=temp;
		ACQUIRE_LOCK(&p->m[end]->mutex);
		/*printf("Created mspace idx %d\n", end);*/
		RELEASE_LOCK(&p->mutex);
		n=end;
		goto found;
	}
	/* Let it lock on the last one it used */
badexit:
	ACQUIRE_LOCK(&p->m[*lastUsed]->mutex);
	return p->m[*lastUsed];
found:
	*lastUsed=n;
	if(tc)
		tc->mymspace=n;
	else
	{
		if(TLSSET(p->mycache, (void *)(size_t)(-(n+1)))) abort();
	}
	return p->m[n];
}

nedpool *nedcreatepool(size_t capacity, int threads) THROWSPEC
{
	nedpool *ret;
	if(!(ret=(nedpool *) nedpcalloc(0, 1, sizeof(nedpool)))) return 0;
	if(!InitPool(ret, capacity, threads))
	{
		nedpfree(0, ret);
		return 0;
	}
	return ret;
}
void neddestroypool(nedpool *p) THROWSPEC
{
	int n;
	ACQUIRE_LOCK(&p->mutex);
	DestroyCaches(p);
	for(n=0; p->m[n]; n++)
	{
		destroy_mspace(p->m[n]);
		p->m[n]=0;
	}
	RELEASE_LOCK(&p->mutex);
	if(TLSFREE(p->mycache)) abort();
	nedpfree(0, p);
}

void nedpsetvalue(nedpool *p, void *v) THROWSPEC
{
	if(!p) { p=&syspool; if(!syspool.threads) InitPool(&syspool, 0, -1); }
	p->uservalue=v;
}
void *nedgetvalue(nedpool **p, void *mem) THROWSPEC
{
	nedpool *np=0;
	mchunkptr mcp=mem2chunk(mem);
	mstate fm;
	if(!(is_aligned(chunk2mem(mcp))) && mcp->head != FENCEPOST_HEAD) return 0;
	if(!cinuse(mcp)) return 0;
	if(!next_pinuse(mcp)) return 0;
	if(!is_mmapped(mcp) && !pinuse(mcp))
	{
		if(next_chunk(prev_chunk(mcp))!=mcp) return 0;
	}
	fm=get_mstate_for(mcp);
	if(!ok_magic(fm)) return 0;
	if(!ok_address(fm, mcp)) return 0;
	if(!fm->extp) return 0;
	np=(nedpool *) fm->extp;
	if(p) *p=np;
	return np->uservalue;
}

void neddisablethreadcache(nedpool *p) THROWSPEC
{
	int mycache;
	if(!p)
	{
		p=&syspool;
		if(!syspool.threads) InitPool(&syspool, 0, -1);
	}
	mycache=(int)(size_t) TLSGET(p->mycache);
	if(!mycache)
	{	/* Set to mspace 0 */
		if(TLSSET(p->mycache, (void *)-1)) abort();
	}
	else if(mycache>0)
	{	/* Set to last used mspace */
		threadcache *tc=p->caches[mycache-1];
#if defined(DEBUG)
		printf("Threadcache utilisation: %lf%% in cache with %lf%% lost to other threads\n",
			100.0*tc->successes/tc->mallocs, 100.0*((double) tc->mallocs-tc->frees)/tc->mallocs);
#endif
		if(TLSSET(p->mycache, (void *)(size_t)(-tc->mymspace))) abort();
		tc->frees++;
		RemoveCacheEntries(p, tc, 0);
		assert(!tc->freeInCache);
		tc->mymspace=-1;
		tc->threadid=0;
		mspace_free(0, p->caches[mycache-1]);
		p->caches[mycache-1]=0;
	}
}

#define GETMSPACE(m,p,tc,ms,s,action)           \
  do                                            \
  {                                             \
    mstate m = GetMSpace((p),(tc),(ms),(s));    \
    action;                                     \
    RELEASE_LOCK(&m->mutex);                    \
  } while (0)

static FORCEINLINE mstate GetMSpace(nedpool *p, threadcache *tc, int mymspace, size_t size) THROWSPEC
{	/* Returns a locked and ready for use mspace */
	mstate m=p->m[mymspace];
	assert(m);
	if(!TRY_LOCK(&p->m[mymspace]->mutex)) m=FindMSpace(p, tc, &mymspace, size);\
	/*assert(IS_LOCKED(&p->m[mymspace]->mutex));*/
	return m;
}
static FORCEINLINE void GetThreadCache(nedpool **p, threadcache **tc, int *mymspace, size_t *size) THROWSPEC
{
	int mycache;
	if(size && *size<sizeof(threadcacheblk)) *size=sizeof(threadcacheblk);
	if(!*p)
	{
		*p=&syspool;
		if(!syspool.threads) InitPool(&syspool, 0, -1);
	}
	mycache=(int)(size_t) TLSGET((*p)->mycache);
	if(mycache>0)
	{
		*tc=(*p)->caches[mycache-1];
		*mymspace=(*tc)->mymspace;
	}
	else if(!mycache)
	{
		*tc=AllocCache(*p);
		if(!*tc)
		{	/* Disable */
			if(TLSSET((*p)->mycache, (void *)-1)) abort();
			*mymspace=0;
		}
		else
			*mymspace=(*tc)->mymspace;
	}
	else
	{
		*tc=0;
		*mymspace=-mycache-1;
	}
	assert(*mymspace>=0);
	assert((long)(size_t)CURRENT_THREAD==(*tc)->threadid);
#ifdef FULLSANITYCHECKS
	if(*tc)
	{
		if(*(unsigned int *)"NEDMALC1"!=(*tc)->magic1 || *(unsigned int *)"NEDMALC2"!=(*tc)->magic2)
		{
			abort();
		}
	}
#endif
}

void * nedpmalloc(nedpool *p, size_t size) THROWSPEC
{
	void *ret=0;
	threadcache *tc;
	int mymspace;
	GetThreadCache(&p, &tc, &mymspace, &size);
#if THREADCACHEMAX
	if(tc && size<=THREADCACHEMAX)
	{	/* Use the thread cache */
		ret=threadcache_malloc(p, tc, &size);
	}
#endif
	if(!ret)
	{	/* Use this thread's mspace */
        GETMSPACE(m, p, tc, mymspace, size,
                  ret=mspace_malloc(m, size));
	}
	return ret;
}
void * nedpcalloc(nedpool *p, size_t no, size_t size) THROWSPEC
{
	size_t rsize=size*no;
	void *ret=0;
	threadcache *tc;
	int mymspace;
	GetThreadCache(&p, &tc, &mymspace, &rsize);
#if THREADCACHEMAX
	if(tc && rsize<=THREADCACHEMAX)
	{	/* Use the thread cache */
		if((ret=threadcache_malloc(p, tc, &rsize)))
			memset(ret, 0, rsize);
	}
#endif
	if(!ret)
	{	/* Use this thread's mspace */
        GETMSPACE(m, p, tc, mymspace, rsize,
                  ret=mspace_calloc(m, 1, rsize));
	}
	return ret;
}
void * nedprealloc(nedpool *p, void *mem, size_t size) THROWSPEC
{
	void *ret=0;
	threadcache *tc;
	int mymspace;
	if(!mem) return nedpmalloc(p, size);
	GetThreadCache(&p, &tc, &mymspace, &size);
#if THREADCACHEMAX
	if(tc && size && size<=THREADCACHEMAX)
	{	/* Use the thread cache */
		size_t memsize=nedblksize(mem);
		assert(memsize);
		if((ret=threadcache_malloc(p, tc, &size)))
		{
			memcpy(ret, mem, memsize<size ? memsize : size);
			if(memsize<=THREADCACHEMAX)
				threadcache_free(p, tc, mymspace, mem, memsize);
			else
				mspace_free(0, mem);
		}
	}
#endif
	if(!ret)
	{	/* Reallocs always happen in the mspace they happened in, so skip
		locking the preferred mspace for this thread */
		ret=mspace_realloc(0, mem, size);
	}
	return ret;
}
void   nedpfree(nedpool *p, void *mem) THROWSPEC
{	/* Frees always happen in the mspace they happened in, so skip
	locking the preferred mspace for this thread */
	threadcache *tc;
	int mymspace;
	size_t memsize;
	assert(mem);
	GetThreadCache(&p, &tc, &mymspace, 0);
#if THREADCACHEMAX
	memsize=nedblksize(mem);
	assert(memsize);
	if(mem && tc && memsize<=(THREADCACHEMAX+CHUNK_OVERHEAD))
		threadcache_free(p, tc, mymspace, mem, memsize);
	else
#endif
		mspace_free(0, mem);
}
void * nedpmemalign(nedpool *p, size_t alignment, size_t bytes) THROWSPEC
{
	void *ret;
	threadcache *tc;
	int mymspace;
	GetThreadCache(&p, &tc, &mymspace, &bytes);
	{	/* Use this thread's mspace */
        GETMSPACE(m, p, tc, mymspace, bytes,
                  ret=mspace_memalign(m, alignment, bytes));
	}
	return ret;
}
#if !NO_MALLINFO
struct mallinfo nedpmallinfo(nedpool *p) THROWSPEC
{
	int n;
	struct mallinfo ret={0};
	if(!p) { p=&syspool; if(!syspool.threads) InitPool(&syspool, 0, -1); }
	for(n=0; p->m[n]; n++)
	{
		struct mallinfo t=mspace_mallinfo(p->m[n]);
		ret.arena+=t.arena;
		ret.ordblks+=t.ordblks;
		ret.hblkhd+=t.hblkhd;
		ret.usmblks+=t.usmblks;
		ret.uordblks+=t.uordblks;
		ret.fordblks+=t.fordblks;
		ret.keepcost+=t.keepcost;
	}
	return ret;
}
#endif
int    nedpmallopt(nedpool *p, int parno, int value) THROWSPEC
{
	return mspace_mallopt(parno, value);
}
int    nedpmalloc_trim(nedpool *p, size_t pad) THROWSPEC
{
	int n, ret=0;
	if(!p) { p=&syspool; if(!syspool.threads) InitPool(&syspool, 0, -1); }
	for(n=0; p->m[n]; n++)
	{
		ret+=mspace_trim(p->m[n], pad);
	}
	return ret;
}
void   nedpmalloc_stats(nedpool *p) THROWSPEC
{
	int n;
	if(!p) { p=&syspool; if(!syspool.threads) InitPool(&syspool, 0, -1); }
	for(n=0; p->m[n]; n++)
	{
		mspace_malloc_stats(p->m[n]);
	}
}
size_t nedpmalloc_footprint(nedpool *p) THROWSPEC
{
	size_t ret=0;
	int n;
	if(!p) { p=&syspool; if(!syspool.threads) InitPool(&syspool, 0, -1); }
	for(n=0; p->m[n]; n++)
	{
		ret+=mspace_footprint(p->m[n]);
	}
	return ret;
}
void **nedpindependent_calloc(nedpool *p, size_t elemsno, size_t elemsize, void **chunks) THROWSPEC
{
	void **ret;
	threadcache *tc;
	int mymspace;
	GetThreadCache(&p, &tc, &mymspace, &elemsize);
    GETMSPACE(m, p, tc, mymspace, elemsno*elemsize,
              ret=mspace_independent_calloc(m, elemsno, elemsize, chunks));
	return ret;
}
void **nedpindependent_comalloc(nedpool *p, size_t elems, size_t *sizes, void **chunks) THROWSPEC
{
	void **ret;
	threadcache *tc;
	int mymspace;
    size_t i, *adjustedsizes=(size_t *) alloca(elems*sizeof(size_t));
    if(!adjustedsizes) return 0;
    for(i=0; i<elems; i++)
        adjustedsizes[i]=sizes[i]<sizeof(threadcacheblk) ? sizeof(threadcacheblk) : sizes[i];
	GetThreadCache(&p, &tc, &mymspace, 0);
	GETMSPACE(m, p, tc, mymspace, 0,
              ret=mspace_independent_comalloc(m, elems, adjustedsizes, chunks));
	return ret;
}

#if defined(__cplusplus)
}
#endif
