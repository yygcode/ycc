/*
 * sptree.h -- Splay Trees
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

#ifndef __YCC_ALGOS_SPTREE_H_
#define __YCC_ALGOS_SPTREE_H_

#include <ycc/algos/bstree-link.h>

__BEGIN_DECLS

struct spt_node
{
	__BST_LINK_MEMBER(struct spt_node);
} __aligned(sizeof(void*));

struct spt_root
{
	struct spt_node *node;
};

#define spt_entry(ptr, type, member)	container_of(ptr, type, member)

#define SPT_DECLARE(name)	struct spt_root name =  { NULL, }
#define SPT_INIT(name)	do { (name).node = NULL; } while (0)
static inline void spt_init(struct spt_root *spt)
{
	SPT_INIT(*spt);
}

static inline void spt_link_node(struct spt_node *node,
				struct spt_node *parent,
				struct spt_node **pnode)
{
	__BSTLINK_INIT(node, parent, pnode);
}

static inline struct spt_node *spt_first(const struct spt_root *spt)
{
	return __BSTLINK_FIRST(spt->node, struct spt_node);
}

static inline struct spt_node *spt_last(const struct spt_root *spt)
{
	return __BSTLINK_LAST(spt->node, struct spt_node);
}

static inline struct spt_node *spt_next(const struct spt_node* node)
{
	return __BSTLINK_NEXT(node, struct spt_node);
}

static inline struct spt_node *spt_prev(const struct spt_node* node)
{
	return __BSTLINK_PREV(node, struct spt_node);
}

void __spt_splay(struct spt_node *node, struct spt_node **proot);

void spt_erase(struct spt_node *node, struct spt_root *spt);

/* helper routine */
static inline struct spt_node *
spt_find(const struct spt_root *spt,
	int (*compare)(const struct spt_node *node,
		       const void *arg),
        const void *arg)
{
	struct spt_node *p = __BSTLINK_FIND(spt->node, compare, arg,
					    struct spt_node);

	if (p)
		__spt_splay(p, (struct spt_node**)&spt->node);

	return p;
}

static inline struct spt_node *
spt_lower_bound(const struct spt_root *spt,
	       int (*compare)(const struct spt_node *node,
			      const void *arg),
	       const void *arg)
{
	return __BSTLINK_LOWER_BOUND(spt->node, compare, arg, struct spt_node);
}

static inline struct spt_node *
spt_upper_bound(const struct spt_root *spt,
	       int (*compare)(const struct spt_node *node,
			      const void *arg),
	       const void *arg)
{
	return __BSTLINK_UPPER_BOUND(spt->node, compare, arg, struct spt_node);
}

static inline void
spt_lower_upper_bound(const struct spt_root *spt,
		     int (*compare)(const struct spt_node *node,
				    const void *arg),
		     const void *arg,
		     struct spt_node **plower,
		     struct spt_node **pupper)
{
	return __BSTLINK_LOWER_UPPER_BOUND(spt->node, compare, arg,
					   plower, pupper);
}

static inline void
spt_insert(struct spt_node *node,
	  struct spt_root *spt,
	  int (*compare_link)(const struct spt_node *node1,
			      const struct spt_node *node2,
			      const void *arg),
	  const void *arg)
{
	(void)__BSTLINK_INSERT(node, &spt->node, compare_link, arg, false);
	__spt_splay(node, &spt->node);
}

static inline bool
spt_insert_unique(struct spt_node *node,
		 struct spt_root *spt,
		 int (*compare_link)(const struct spt_node *node1,
				     const struct spt_node *node2,
				     const void *arg),
		 const void *arg)
{
	
	if (__BSTLINK_INSERT(node, &spt->node, compare_link, arg, false)) {
		__spt_splay(node, &spt->node);
		return true;
	}

	return false;
}

static inline void
spt_replace(struct spt_node *victim, struct spt_node *new, struct spt_root *spt)
{
	__BSTLINK_REPLACE(victim, new, &spt->node);
	__spt_splay(new, &spt->node);
}

static inline void spt_erase_range(struct spt_node *beg,
				  struct spt_node *end,
				  struct spt_root *spt)
{
	__BSTLINK_ERASE_RANGE(beg, end, spt_erase, spt);
}

static inline void
spt_erase_equal(struct spt_root *spt,
	       int (*compare)(const struct spt_node *node, const void *arg),
	       void (*destroy)(struct spt_node *node, const void *arg),
	       const void *arg_compare,
	       const void *arg_destroy)
{
	__BSTLINK_ERASE_EQUAL(spt->node, compare, spt_erase, destroy,
			      arg_compare, spt, arg_destroy);
}

static inline size_t
spt_count(const struct spt_root *spt,
	 int (*compare)(const struct spt_node *node1,
			     const void *arg),
	 const void *arg)
{
	return __BSTLINK_COUNT(spt->node, compare, arg);
}

static inline void
spt_clear(struct spt_root *spt,
	 void (*destroy)(struct spt_node *node, const void *arg),
	 const void *arg)
{
	__BSTLINK_DESTROY(spt->node, destroy, arg);
	spt->node = NULL;
}

static inline void
spt_visit(const struct spt_root *spt,
	 void (*visit)(const struct spt_node *node, const void *arg),
	 const void *arg)
{
	__BSTLINK_VISIT(spt->node, visit, arg);
}

static inline bool
spt_visit_cond(const struct spt_root *spt,
	      bool (*visit_cond)(const struct spt_node *node, const void *arg),
	      const void *arg)
{
	return __BSTLINK_VISIT_COND(spt->node, visit_cond, arg);
}

static inline size_t
spt_height(struct spt_root *spt, bool bmax)
{
	return __BSTLINK_HEIGHT(spt->node, bmax);
}
#define spt_height_max(spt)	spt_height(spt, true)
#define spt_height_min(spt)	spt_height(spt, false)

struct spt_root *spt_alloc(size_t num);
void spt_free(struct spt_root *spt,
	     size_t num,
	     void (*destroy)(struct spt_node *spt, const void *arg),
	     const void *arg);

bool spt_clone(struct spt_root *spt,
	      const struct spt_root *spt_src,
	      struct spt_node *(*node_clone)(const struct spt_node *node,
					    const void *arg),
	      void (*node_destroy)(struct spt_node *node, const void *arg),
	      const void *arg_clone,
	      const void *arg_destroy);

/* clone range: [beg, end) */
bool spt_clone_range(struct spt_root *spt,
		    const struct spt_node *beg,
		    const struct spt_node *end,
		    struct spt_node *(*node_clone)(const struct spt_node *node,
						  const void *arg),
		    void (*node_destroy)(struct spt_node *node,
					 const void *arg),
		    const void *arg_clone,
		    const void *arg_destroy);

static inline void spt_swap(struct spt_root *spt1, struct spt_root *spt2)
{
	struct spt_node *node = spt1->node;
	spt1->node = spt2->node;
	spt2->node = node;
}

__END_DECLS

#endif /* __YCC_ALGOS_SPTREE_H_ */
