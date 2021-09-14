/* SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2010-2015 Intel Corporation
 * Copyright (c) 2007,2008 Kip Macy kmacy@freebsd.org
 * All rights reserved.
 * Derived from FreeBSD's bufring.h
 * Used as BSD-3 Licensed with permission from Kip Macy.
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/queue.h>

#include "rte_ring.h"
#include "rte_ring_elem.h"

/* true if x is a power of 2 */
#define POWEROF2(x) ((((x)-1) & (x)) == 0)

/* by default set head/tail distance as 1/8 of ring capacity */
#define HTD_MAX_DEF	8

//#define rte_errno RTE_PER_LCORE(_rte_errno)
static int rte_errno = 0;


#define RTE_LOG_ERR(fmt...) do{printf(fmt);assert(0);}while(0)

/**
 * @internal
 * DPDK-specific version of strlcpy for systems without
 * libc or libbsd copies of the function
 */
static inline size_t
rte_strlcpy(char *dst, const char *src, size_t size)
{
	return (size_t)snprintf(dst, size, "%s", src);
}

static void*
rte_malloc(size_t size)
{
	return malloc(size);
}
static void
rte_free(void *addr)
{
	return free(addr);
}


/**
 * Combines 32b inputs most significant set bits into the least
 * significant bits to construct a value with the same MSBs as x
 * but all 1's under it.
 *
 * @param x
 *    The integer whose MSBs need to be combined with its LSBs
 * @return
 *    The combined value.
 */
static inline uint32_t
rte_combine32ms1b(uint32_t x)
{
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;

	return x;
}


/**
 * Aligns input parameter to the next power of 2
 *
 * @param x
 *   The integer value to align
 *
 * @return
 *   Input parameter aligned to the next power of 2
 */
static inline uint32_t
rte_align32pow2(uint32_t x)
{
	x--;
	x = rte_combine32ms1b(x);

	return x + 1;
}


/* return the size of memory occupied by a ring */
ssize_t
rte_ring_get_memsize_elem(unsigned int esize, unsigned int count)
{
	ssize_t sz;

	/* Check if element size is a multiple of 4B */
	if (esize % 4 != 0) {
		RTE_LOG_ERR("element size is not a multiple of 4\n");

		return -EINVAL;
	}

	/* count must be a power of 2 */
	if ((!POWEROF2(count)) || (count > RTE_RING_SZ_MASK )) {
		RTE_LOG_ERR("Requested number of elements is invalid, must be power of 2, and not exceed %u\n",
			RTE_RING_SZ_MASK);

		return -EINVAL;
	}

	sz = sizeof(struct rte_ring) + count * esize;
	sz = RTE_ALIGN(sz, RTE_CACHE_LINE_SIZE);
	return sz;
}

/* return the size of memory occupied by a ring */
ssize_t
rte_ring_get_memsize(unsigned int count)
{
	return rte_ring_get_memsize_elem(sizeof(void *), count);
}

/*
 * internal helper function to reset prod/cons head-tail values.
 */
static void
reset_headtail(void *p)
{
	struct rte_ring_headtail *ht;
	struct rte_ring_hts_headtail *ht_hts;
	struct rte_ring_rts_headtail *ht_rts;

	ht = p;
	ht_hts = p;
	ht_rts = p;

	switch (ht->sync_type) {
	case RTE_RING_SYNC_MT:
	case RTE_RING_SYNC_ST:
		ht->head = 0;
		ht->tail = 0;
		break;
#ifdef ALLOW_EXPERIMENTAL_API
	case RTE_RING_SYNC_MT_RTS:
		ht_rts->head.raw = 0;
		ht_rts->tail.raw = 0;
		break;
	case RTE_RING_SYNC_MT_HTS:
		ht_hts->ht.raw = 0;
		break;
#endif    
	default:
		/* unknown sync mode */
		RTE_ASSERT(0);
	}
}

void
rte_ring_reset(struct rte_ring *r)
{
	reset_headtail(&r->prod);
	reset_headtail(&r->cons);
}

/*
 * helper function, calculates sync_type values for prod and cons
 * based on input flags. Returns zero at success or negative
 * errno value otherwise.
 */
static int
get_sync_type(uint32_t flags, enum rte_ring_sync_type *prod_st,
	enum rte_ring_sync_type *cons_st)
{
	static const uint32_t prod_st_flags =
		(RING_F_SP_ENQ | RING_F_MP_RTS_ENQ | RING_F_MP_HTS_ENQ);
	static const uint32_t cons_st_flags =
		(RING_F_SC_DEQ | RING_F_MC_RTS_DEQ | RING_F_MC_HTS_DEQ);

	switch (flags & prod_st_flags) {
	case 0:
		*prod_st = RTE_RING_SYNC_MT;
		break;
	case RING_F_SP_ENQ:
		*prod_st = RTE_RING_SYNC_ST;
		break;
#ifdef ALLOW_EXPERIMENTAL_API
	case RING_F_MP_RTS_ENQ:
		*prod_st = RTE_RING_SYNC_MT_RTS;
		break;
	case RING_F_MP_HTS_ENQ:
		*prod_st = RTE_RING_SYNC_MT_HTS;
		break;
#endif    
	default:
		return -EINVAL;
	}

	switch (flags & cons_st_flags) {
	case 0:
		*cons_st = RTE_RING_SYNC_MT;
		break;
	case RING_F_SC_DEQ:
		*cons_st = RTE_RING_SYNC_ST;
		break;
#ifdef ALLOW_EXPERIMENTAL_API
	case RING_F_MC_RTS_DEQ:
		*cons_st = RTE_RING_SYNC_MT_RTS;
		break;
	case RING_F_MC_HTS_DEQ:
		*cons_st = RTE_RING_SYNC_MT_HTS;
		break;
#endif    
	default:
		return -EINVAL;
	}

	return 0;
}

int
rte_ring_init(struct rte_ring *r, const char *name, unsigned count,
	unsigned flags)
{
	int ret;
/**
 * Triggers an error at compilation time if the condition is true.
 */
#define RTE_BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))

	/* compilation-time checks */
	RTE_BUILD_BUG_ON((sizeof(struct rte_ring) &
			  RTE_CACHE_LINE_MASK) != 0);
	RTE_BUILD_BUG_ON((offsetof(struct rte_ring, cons) &
			  RTE_CACHE_LINE_MASK) != 0);
	RTE_BUILD_BUG_ON((offsetof(struct rte_ring, prod) &
			  RTE_CACHE_LINE_MASK) != 0);

	RTE_BUILD_BUG_ON(offsetof(struct rte_ring_headtail, sync_type) !=
		offsetof(struct rte_ring_hts_headtail, sync_type));
	RTE_BUILD_BUG_ON(offsetof(struct rte_ring_headtail, tail) !=
		offsetof(struct rte_ring_hts_headtail, ht.pos.tail));

	RTE_BUILD_BUG_ON(offsetof(struct rte_ring_headtail, sync_type) !=
		offsetof(struct rte_ring_rts_headtail, sync_type));
	RTE_BUILD_BUG_ON(offsetof(struct rte_ring_headtail, tail) !=
		offsetof(struct rte_ring_rts_headtail, tail.val.pos));

	/* init the ring structure */
	memset(r, 0, sizeof(*r));
	ret = rte_strlcpy(r->name, name, sizeof(r->name));
	if (ret < 0 || ret >= (int)sizeof(r->name))
		return -ENAMETOOLONG;
	r->flags = flags;
	ret = get_sync_type(flags, &r->prod.sync_type, &r->cons.sync_type);
	if (ret != 0)
		return ret;

	if (flags & RING_F_EXACT_SZ) {
		r->size = rte_align32pow2(count + 1);
		r->mask = r->size - 1;
		r->capacity = count;
	} else {
		if ((!POWEROF2(count)) || (count > RTE_RING_SZ_MASK)) {
			RTE_LOG_ERR("Requested size is invalid, must be power of 2, and not exceed the size limit %u\n",
				RTE_RING_SZ_MASK);
			return -EINVAL;
		}
		r->size = count;
		r->mask = count - 1;
		r->capacity = r->mask;
	}
    
#ifdef ALLOW_EXPERIMENTAL_API
	/* set default values for head-tail distance */
	if (flags & RING_F_MP_RTS_ENQ)
		rte_ring_set_prod_htd_max(r, r->capacity / HTD_MAX_DEF);
	if (flags & RING_F_MC_RTS_DEQ)
		rte_ring_set_cons_htd_max(r, r->capacity / HTD_MAX_DEF);
#endif
	return 0;
}

/* create the ring for a given element size */
struct rte_ring *
rte_ring_create_elem(const char *name, unsigned int esize, unsigned int count,
		int socket_id, unsigned int flags)
{
	char mz_name[RTE_MEMZONE_NAMESIZE];
	struct rte_ring *r = NULL;
//	struct rte_tailq_entry *te;
//	const struct rte_memzone *mz;
	ssize_t ring_size;
	int mz_flags = 0;
//	struct rte_ring_list* ring_list = NULL;
	const unsigned int requested_count = count;
	int ret;

//	ring_list = RTE_TAILQ_CAST(rte_ring_tailq.head, rte_ring_list);

	/* for an exact size ring, round up from count to a power of two */
	if (flags & RING_F_EXACT_SZ)
		count = rte_align32pow2(count + 1);

	ring_size = rte_ring_get_memsize_elem(esize, count);
	if (ring_size < 0) {
		rte_errno = ring_size;
		return NULL;
	}

	ret = snprintf(mz_name, sizeof(mz_name), "%s%s",
		RTE_RING_MZ_PREFIX, name);
	if (ret < 0 || ret >= (int)sizeof(mz_name)) {
		rte_errno = ENAMETOOLONG;
		return NULL;
	}

//    r = rte_zmalloc("rte-ring", 64, sizeof(struct rte_ring));
    r = (struct rte_ring *)rte_malloc(ring_size);
    
    /* no need to check return value here, we already checked the
     * arguments above */
    rte_ring_init(r, name, requested_count, flags);

	return r;
}

/* create the ring */
struct rte_ring *
rte_ring_create(const char *name, unsigned int count, int socket_id,
		unsigned int flags)
{
	return rte_ring_create_elem(name, sizeof(void *), count, socket_id,
		flags);
}

/* free the ring */
void
rte_ring_free(struct rte_ring *r)
{
//    printf("++++++++++++++++++++++++++++++++++\n");

	if (r == NULL)
		return;

	rte_free(r);
    r = NULL;
}

/* dump the status of the ring on the console */
void
rte_ring_dump(FILE *f, const struct rte_ring *r)
{
	fprintf(f, "ring <%s>@%p\n", r->name, r);
	fprintf(f, "  flags=%x\n", r->flags);
	fprintf(f, "  size=%"PRIu32"\n", r->size);
	fprintf(f, "  capacity=%"PRIu32"\n", r->capacity);
	fprintf(f, "  ct=%"PRIu32"\n", r->cons.tail);
	fprintf(f, "  ch=%"PRIu32"\n", r->cons.head);
	fprintf(f, "  pt=%"PRIu32"\n", r->prod.tail);
	fprintf(f, "  ph=%"PRIu32"\n", r->prod.head);
	fprintf(f, "  used=%u\n", rte_ring_count(r));
	fprintf(f, "  avail=%u\n", rte_ring_free_count(r));
}

