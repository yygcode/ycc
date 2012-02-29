/*
 * bstree.h -- Trival Binary Search Trees
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

#ifndef __YCC_ALGOS_BSTREE_H_
#define __YCC_ALGOS_BSTREE_H_

#include <ycc/algos/bstree-link.h>

__BEGIN_DECLS

struct bst_node
{
	__BST_LINK_MEMBER(struct bst_node);
} __aligned(sizeof(void*));

struct bst_root
{
	struct bst_node *node;
};

#define bst_entry(ptr, type, member)	container_of(ptr, type, member)

#define BST_DECLARE(name)	struct bst_root name =  { NULL, }
#define BST_INIT(name)	do { (name).node = NULL; } while (0)
static inline void bst_init(struct bst_root *bst)
{
	BST_INIT(*bst);
}

static inline void bst_link_node(struct bst_node *node,
				struct bst_node *parent,
				struct bst_node **pnode)
{
	__BSTLINK_INIT(node, parent, pnode);
}

static inline struct bst_node *bst_first(const struct bst_root *bst)
{
	return __BSTLINK_FIRST(bst->node, struct bst_node);
}

static inline struct bst_node *bst_last(const struct bst_root *bst)
{
	return __BSTLINK_LAST(bst->node, struct bst_node);
}

static inline struct bst_node *bst_next(const struct bst_node* node)
{
	return __BSTLINK_NEXT(node, struct bst_node);
}

static inline struct bst_node *bst_prev(const struct bst_node* node)
{
	return __BSTLINK_PREV(node, struct bst_node);
}

void bst_erase(struct bst_node *node, struct bst_root *bst);

/* helper routine */
static inline struct bst_node *
bst_find(const struct bst_root *bst,
	int (*compare)(const struct bst_node *node,
		       const void *arg),
        const void *arg)
{
	return __BSTLINK_FIND(bst->node, compare, arg, struct bst_node);
}

static inline struct bst_node *
bst_lower_bound(const struct bst_root *bst,
	       int (*compare)(const struct bst_node *node,
			      const void *arg),
	       const void *arg)
{
	return __BSTLINK_LOWER_BOUND(bst->node, compare, arg, struct bst_node);
}

static inline struct bst_node *
bst_upper_bound(const struct bst_root *bst,
	       int (*compare)(const struct bst_node *node,
			      const void *arg),
	       const void *arg)
{
	return __BSTLINK_UPPER_BOUND(bst->node, compare, arg, struct bst_node);
}

static inline void
bst_lower_upper_bound(const struct bst_root *bst,
		     int (*compare)(const struct bst_node *node,
				    const void *arg),
		     const void *arg,
		     struct bst_node **plower,
		     struct bst_node **pupper)
{
	return __BSTLINK_LOWER_UPPER_BOUND(bst->node, compare, arg,
					   plower, pupper);
}

static inline void
bst_insert(struct bst_node *node,
	  struct bst_root *bst,
	  int (*compare_link)(const struct bst_node *node1,
			      const struct bst_node *node2,
			      const void *arg),
	  const void *arg)
{
	(void)__BSTLINK_INSERT(node, &bst->node, compare_link,
				       arg, false);
}

static inline bool
bst_insert_unique(struct bst_node *node,
		 struct bst_root *bst,
		 int (*compare_link)(const struct bst_node *node1,
				     const struct bst_node *node2,
				     const void *arg),
		 const void *arg)
{
	
	return __BSTLINK_INSERT(node, &bst->node, compare_link,
					arg, false);
}

static inline void
bst_replace(struct bst_node *victim, struct bst_node *new, struct bst_root *bst)
{
	__BSTLINK_REPLACE(victim, new, &bst->node);
}

static inline void bst_erase_range(struct bst_node *beg,
				  struct bst_node *end,
				  struct bst_root *bst)
{
	__BSTLINK_ERASE_RANGE(beg, end, bst_erase, bst);
}

static inline void
bst_erase_equal(struct bst_root *bst,
	       int (*compare)(const struct bst_node *node, const void *arg),
	       void (*destroy)(struct bst_node *node, const void *arg),
	       const void *arg_compare,
	       const void *arg_destroy)
{
	__BSTLINK_ERASE_EQUAL(bst->node, compare, bst_erase, destroy,
			      arg_compare, bst, arg_destroy);
}

static inline size_t
bst_count(const struct bst_root *bst,
	 int (*compare)(const struct bst_node *node1,
			     const void *arg),
	 const void *arg)
{
	return __BSTLINK_COUNT(bst->node, compare, arg);
}

static inline void
bst_clear(struct bst_root *bst,
	 void (*destroy)(struct bst_node *node, const void *arg),
	 const void *arg)
{
	__BSTLINK_DESTROY(bst->node, destroy, arg);
	bst->node = NULL;
}

static inline void
bst_visit(const struct bst_root *bst,
	 void (*visit)(const struct bst_node *node, const void *arg),
	 const void *arg)
{
	__BSTLINK_VISIT(bst->node, visit, arg);
}

static inline bool
bst_visit_cond(const struct bst_root *bst,
	      bool (*visit_cond)(const struct bst_node *node, const void *arg),
	      const void *arg)
{
	return __BSTLINK_VISIT_COND(bst->node, visit_cond, arg);
}

static inline size_t
bst_height(struct bst_root *bst, bool bmax)
{
	return __BSTLINK_HEIGHT(bst->node, bmax);
}
#define bst_height_max(bst)	bst_height(bst, true)
#define bst_height_min(bst)	bst_height(bst, false)

struct bst_root *bst_alloc(size_t num);
void bst_free(struct bst_root *bst,
	     size_t num,
	     void (*destroy)(struct bst_node *bst, const void *arg),
	     const void *arg);

bool bst_clone(struct bst_root *bst,
	      const struct bst_root *bst_src,
	      struct bst_node *(*node_clone)(const struct bst_node *node,
					    const void *arg),
	      void (*node_destroy)(struct bst_node *node, const void *arg),
	      const void *arg_clone,
	      const void *arg_destroy);

/* clone range: [beg, end) */
bool bst_clone_range(struct bst_root *bst,
		    const struct bst_node *beg,
		    const struct bst_node *end,
		    struct bst_node *(*node_clone)(const struct bst_node *node,
						  const void *arg),
		    void (*node_destroy)(struct bst_node *node,
					 const void *arg),
		    const void *arg_clone,
		    const void *arg_destroy);

static inline void bst_swap(struct bst_root *bst1, struct bst_root *bst2)
{
	struct bst_node *node = bst1->node;
	bst1->node = bst2->node;
	bst2->node = node;
}

__END_DECLS

#endif /* __YCC_ALGOS_BSTREE_H_ */
