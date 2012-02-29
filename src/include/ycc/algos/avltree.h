/*
 * avltree.h -- Self-balancing Binary Search Trees
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
 * To use avltrees you'll have to implement your own insert and search cores.
 * This will avoid us to use callbacks and to drop drammatically performances.
 *
 * Example of insert and search as follows
 */

#if 0
Example of insert and search routines.
#endif

#ifndef __YCC_ALGOS_AVLTREE_H_
#define __YCC_ALGOS_AVLTREE_H_

#include <ycc/algos/bstree-link.h>

__BEGIN_DECLS

struct avl_node
{
	__BST_LINK_MEMBER(struct avl_node);
	unsigned depth;
} __aligned(sizeof(void*));

struct avl_root
{
	struct avl_node *node;
};

#define avl_depth(n)	((n)->depth)
#define avl_set_depth(n,_depth)	((n)->depth = _depth)
#define avl_entry(ptr, type, member)	container_of(ptr, type, member)

#define AVL_DECLARE(name)	struct avl_root name =  { NULL, }
#define AVL_INIT(name)	do { (name).node = NULL; } while (0)
static inline void avl_init(struct avl_root *avl)
{
	AVL_INIT(*avl);
}

static inline void avl_link_node(struct avl_node *node,
				struct avl_node *parent,
				struct avl_node **pnode)
{
	__BSTLINK_INIT(node, parent, pnode);
	node->depth = 1;
}

static inline struct avl_node *avl_first(const struct avl_root *avl)
{
	return __BSTLINK_FIRST(avl->node, struct avl_node);
}

static inline struct avl_node *avl_last(const struct avl_root *avl)
{
	return __BSTLINK_LAST(avl->node, struct avl_node);
}

static inline struct avl_node *avl_next(const struct avl_node* node)
{
	return __BSTLINK_NEXT(node, struct avl_node);
}

static inline struct avl_node *avl_prev(const struct avl_node* node)
{
	return __BSTLINK_PREV(node, struct avl_node);
}

void avl_insert_rebalance(struct avl_node *node, struct avl_root *avl);
void avl_erase(struct avl_node *node, struct avl_root *avl);


/* helper routine */
static inline struct avl_node *
avl_find(const struct avl_root *avl,
	int (*compare)(const struct avl_node *node,
		       const void *arg),
        const void *arg)
{
	return __BSTLINK_FIND(avl->node, compare, arg, struct avl_node);
}

static inline struct avl_node *
avl_lower_bound(const struct avl_root *avl,
	       int (*compare)(const struct avl_node *node,
			      const void *arg),
	       const void *arg)
{
	return __BSTLINK_LOWER_BOUND(avl->node, compare, arg, struct avl_node);
}

static inline struct avl_node *
avl_upper_bound(const struct avl_root *avl,
	       int (*compare)(const struct avl_node *node,
			      const void *arg),
	       const void *arg)
{
	return __BSTLINK_UPPER_BOUND(avl->node, compare, arg, struct avl_node);
}

static inline void
avl_lower_upper_bound(const struct avl_root *avl,
		     int (*compare)(const struct avl_node *node,
				    const void *arg),
		     const void *arg,
		     struct avl_node **plower,
		     struct avl_node **pupper)
{
	return __BSTLINK_LOWER_UPPER_BOUND(avl->node, compare, arg,
					   plower, pupper);
}

static inline void
__avl_insert_rest(struct avl_node *node, struct avl_root *avl)
{
	avl_set_depth(node, 1);
	avl_insert_rebalance(node, avl);
}

static inline void
avl_insert(struct avl_node *node,
	  struct avl_root *avl,
	  int (*compare_link)(const struct avl_node *node1,
			      const struct avl_node *node2,
			      const void *arg),
	  const void *arg)
{
	(void)__BSTLINK_INSERT(node, &avl->node, compare_link,
				       arg, false);
	__avl_insert_rest(node, avl);
}

static inline bool
avl_insert_unique(struct avl_node *node,
		 struct avl_root *avl,
		 int (*compare_link)(const struct avl_node *node1,
				     const struct avl_node *node2,
				     const void *arg),
		 const void *arg)
{
	
	if (__BSTLINK_INSERT(node, &avl->node, compare_link,
				       arg, false))
	{
		__avl_insert_rest(node, avl);
		return true;
	}

	return false;
}

static inline void
avl_replace(struct avl_node *victim, struct avl_node *new, struct avl_root *avl)
{
	__BSTLINK_REPLACE(victim, new, &avl->node);
	avl_set_depth(new, victim->depth);
}

static inline void avl_erase_range(struct avl_node *beg,
				  struct avl_node *end,
				  struct avl_root *avl)
{
	__BSTLINK_ERASE_RANGE(beg, end, avl_erase, avl);
}

static inline void
avl_erase_equal(struct avl_root *avl,
	       int (*compare)(const struct avl_node *node, const void *arg),
	       void (*destroy)(struct avl_node *node, const void *arg),
	       const void *arg_compare,
	       const void *arg_destroy)
{
	__BSTLINK_ERASE_EQUAL(avl->node, compare, avl_erase, destroy,
			      arg_compare, avl, arg_destroy);
}

static inline size_t
avl_count(const struct avl_root *avl,
	 int (*compare)(const struct avl_node *node1,
			     const void *arg),
	 const void *arg)
{
	return __BSTLINK_COUNT(avl->node, compare, arg);
}

static inline void
avl_clear(struct avl_root *avl,
	 void (*destroy)(struct avl_node *node, const void *arg),
	 const void *arg)
{
	__BSTLINK_DESTROY(avl->node, destroy, arg);
	avl->node = NULL;
}

static inline void
avl_visit(const struct avl_root *avl,
	 void (*visit)(const struct avl_node *node, const void *arg),
	 const void *arg)
{
	__BSTLINK_VISIT(avl->node, visit, arg);
}

static inline bool
avl_visit_cond(const struct avl_root *avl,
	      bool (*visit_cond)(const struct avl_node *node, const void *arg),
	      const void *arg)
{
	return __BSTLINK_VISIT_COND(avl->node, visit_cond, arg);
}

static inline size_t
avl_height(struct avl_root *avl, bool bmax)
{
	return __BSTLINK_HEIGHT(avl->node, bmax);
}
#define avl_height_max(avl)	avl_height(avl, true)
#define avl_height_min(avl)	avl_height(avl, false)

/* valid check */
#ifndef NDEBUG
bool avl_isvalid(struct avl_root *avl);
#else
static inline bool avl_isvalid(struct avl_root *avl)
{
	return true;
}
#endif

__END_DECLS

#endif /* __YCC_ALGOS_AVLTREE_H_ */
