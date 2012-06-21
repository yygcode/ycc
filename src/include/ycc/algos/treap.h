/*
 * treap.h -- Randomized Binary Search Trees
 *
 * Copyright (C) 2012-2013 yanyg (yygcode@gmail.com, cppgp@qq.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING, if not see
 * <http://www.gnu.org/licenses/>.
 */

/*
 * To use treaptrees you'll have to implement your own insert and search cores.
 * This will avoid us to use callbacks and to drop drammatically performances.
 *
 * Example of insert and search as follows
 */

#if 0
Example of insert and search routines.
#endif

#ifndef __YCC_ALGOS_TREAPTREE_H_
#define __YCC_ALGOS_TREAPTREE_H_

#include <stdlib.h>

#include <ycc/algos/bstree-link.h>

__BEGIN_DECLS

struct treap_node
{
	__BST_LINK_MEMBER(struct treap_node);
	unsigned priority;
} __aligned(sizeof(void*));

struct treap_root
{
	struct treap_node *node;
};

#define treap_priority(r)			((r)->priority)
#define treap_set_priority(r, _priority)	((r)->priority = (_priority))
#define treap_init_priority(r)			((r)->priority = (unsigned)rand())

#define treap_entry(ptr, type, member)	container_of(ptr, type, member)

#define TREAP_DECLARE(name)	struct treap_root name =  { NULL, }
#define TREAP_INIT(name)	do { (name).node = NULL; } while (0)
static inline void treap_init(struct treap_root *treap)
{
	TREAP_INIT(*treap);
}

static inline void treap_link_node(struct treap_node *node,
				struct treap_node *parent,
				struct treap_node **pnode)
{
	__BSTLINK_INIT(node, parent, pnode);
	treap_init_priority(node);
}

static inline struct treap_node *treap_first(const struct treap_root *treap)
{
	return __BSTLINK_FIRST(treap->node, struct treap_node);
}

static inline struct treap_node *treap_last(const struct treap_root *treap)
{
	return __BSTLINK_LAST(treap->node, struct treap_node);
}

static inline struct treap_node *treap_next(const struct treap_node* node)
{
	return __BSTLINK_NEXT(node, struct treap_node);
}

static inline struct treap_node *treap_prev(const struct treap_node* node)
{
	return __BSTLINK_PREV(node, struct treap_node);
}

void treap_insert_rebalance(struct treap_node *node, struct treap_root *treap);
void treap_erase(struct treap_node *node, struct treap_root *treap);


/* helper routine */
static inline struct treap_node *
treap_find(const struct treap_root *treap,
	int (*compare)(const struct treap_node *node,
		       const void *arg),
        const void *arg)
{
	return __BSTLINK_FIND(treap->node, compare, arg, struct treap_node);
}

static inline struct treap_node *
treap_lower_bound(const struct treap_root *treap,
	       int (*compare)(const struct treap_node *node,
			      const void *arg),
	       const void *arg)
{
	return __BSTLINK_LOWER_BOUND(treap->node, compare, arg, struct treap_node);
}

static inline struct treap_node *
treap_upper_bound(const struct treap_root *treap,
	       int (*compare)(const struct treap_node *node,
			      const void *arg),
	       const void *arg)
{
	return __BSTLINK_UPPER_BOUND(treap->node, compare, arg, struct treap_node);
}

static inline void
treap_lower_upper_bound(const struct treap_root *treap,
		     int (*compare)(const struct treap_node *node,
				    const void *arg),
		     const void *arg,
		     struct treap_node **plower,
		     struct treap_node **pupper)
{
	return __BSTLINK_LOWER_UPPER_BOUND(treap->node, compare, arg,
					   plower, pupper);
}

static inline void
__treap_insert_rest(struct treap_node *node, struct treap_root *treap)
{
	treap_init_priority(node);
	treap_insert_rebalance(node, treap);
}

static inline void
treap_insert(struct treap_node *node,
	  struct treap_root *treap,
	  int (*compare_link)(const struct treap_node *node1,
			      const struct treap_node *node2,
			      const void *arg),
	  const void *arg)
{
	(void)__BSTLINK_INSERT(node, &treap->node, compare_link,
				       arg, false);
	__treap_insert_rest(node, treap);
}

static inline bool
treap_insert_unique(struct treap_node *node,
		 struct treap_root *treap,
		 int (*compare_link)(const struct treap_node *node1,
				     const struct treap_node *node2,
				     const void *arg),
		 const void *arg)
{
	
	if (__BSTLINK_INSERT(node, &treap->node, compare_link,
				       arg, false))
	{
		__treap_insert_rest(node, treap);
		return true;
	}

	return false;
}

static inline void
treap_replace(struct treap_node *victim, struct treap_node *new, struct treap_root *treap)
{
	__BSTLINK_REPLACE(victim, new, &treap->node);
	treap_set_priority(new, victim->priority);
}

static inline void treap_erase_range(struct treap_node *beg,
				  struct treap_node *end,
				  struct treap_root *treap)
{
	__BSTLINK_ERASE_RANGE(beg, end, treap_erase, treap);
}

static inline void
treap_erase_equal(struct treap_root *treap,
	       int (*compare)(const struct treap_node *node, const void *arg),
	       void (*destroy)(struct treap_node *node, const void *arg),
	       const void *arg_compare,
	       const void *arg_destroy)
{
	__BSTLINK_ERASE_EQUAL(treap->node, compare, treap_erase, destroy,
			      arg_compare, treap, arg_destroy);
}

static inline size_t
treap_count(const struct treap_root *treap,
	 int (*compare)(const struct treap_node *node1,
			     const void *arg),
	 const void *arg)
{
	return __BSTLINK_COUNT(treap->node, compare, arg);
}

static inline void
treap_clear(struct treap_root *treap,
	 void (*destroy)(struct treap_node *node, const void *arg),
	 const void *arg)
{
	__BSTLINK_DESTROY(treap->node, destroy, arg);
	treap->node = NULL;
}

static inline void
treap_visit(const struct treap_root *treap,
	 void (*visit)(const struct treap_node *node, const void *arg),
	 const void *arg)
{
	__BSTLINK_VISIT(treap->node, visit, arg);
}

static inline bool
treap_visit_cond(const struct treap_root *treap,
	      bool (*visit_cond)(const struct treap_node *node, const void *arg),
	      const void *arg)
{
	return __BSTLINK_VISIT_COND(treap->node, visit_cond, arg);
}

static inline size_t
treap_height(struct treap_root *treap, bool bmax)
{
	return __BSTLINK_HEIGHT(treap->node, bmax);
}
#define treap_height_max(treap)	treap_height(treap, true)
#define treap_height_min(treap)	treap_height(treap, false)

/* valid check */
#ifndef NDEBUG
bool treap_isvalid(struct treap_root *treap);
#else
static inline bool treap_isvalid(struct treap_root *treap)
{
	return true;
}
#endif

struct treap_root *treap_alloc(size_t num);
void treap_free(struct treap_root *treap,
	     size_t num,
	     void (*destroy)(struct treap_node *treap, const void *arg),
	     const void *arg);

bool treap_clone(struct treap_root *treap,
	      const struct treap_root *treap_src,
	      struct treap_node *(*node_clone)(const struct treap_node *node,
					    const void *arg),
	      void (*node_destroy)(struct treap_node *node, const void *arg),
	      const void *arg_clone,
	      const void *arg_destroy);

/* clone range: [beg, end) */
bool treap_clone_range(struct treap_root *treap,
		    const struct treap_node *beg,
		    const struct treap_node *end,
		    struct treap_node *(*node_clone)(const struct treap_node *node,
						  const void *arg),
		    void (*node_destroy)(struct treap_node *node,
					 const void *arg),
		    const void *arg_clone,
		    const void *arg_destroy);

static inline void treap_swap(struct treap_root *treap1, struct treap_root *treap2)
{
	struct treap_node *node = treap1->node;
	treap1->node = treap2->node;
	treap2->node = node;
}

__END_DECLS

#endif /* __YCC_ALGOS_TREAP_H_ */
