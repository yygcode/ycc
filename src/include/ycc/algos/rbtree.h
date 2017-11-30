/*
 * rbtree.h -- Red Black Trees
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
 * To use rbtrees you'll have to implement your own insert and search cores.
 * This will avoid us to use callbacks and to drop drammatically performances.
 *
 * Example of insert and search as follows
 */

#if 0
Example of insert and search routines.
#endif

#ifndef __YCC_ALGOS_RBTREE_H_
#define __YCC_ALGOS_RBTREE_H_

#include <ycc/algos/bstree-link.h>

__BEGIN_DECLS

struct rb_node
{
	__BST_LINK_MEMBER(struct rb_node);
	unsigned color;
#define RB_COLOR_RED 0
#define RB_COLOR_BLACK 1
} __aligned(sizeof(void*));

struct rb_root
{
	struct rb_node *node;
};

#define rb_color(r)	((r)->color)
#define rb_is_red(r)	(!rb_color(r))
#define rb_is_black(r)	rb_color(r)
#define rb_set_color(r, val)	do { (r)->color = (val); } while(0)
#define rb_set_red(r)	rb_set_color((r), RB_COLOR_RED)
#define rb_set_black(r)	rb_set_color((r), RB_COLOR_BLACK)

#define rb_entry(ptr, type, member)	container_of(ptr, type, member)

#define RB_DECLARE(name)	struct rb_root name =  { NULL, }
#define RB_INIT(name)	do { (name).node = NULL; } while (0)
static inline void rb_init(struct rb_root *rb)
{
	RB_INIT(*rb);
}

static inline void rb_link_node(struct rb_node *node,
				struct rb_node *parent,
				struct rb_node **pnode)
{
	__BSTLINK_INIT(node, parent, pnode);
	node->color = RB_COLOR_RED;
}

static inline struct rb_node *rb_first(const struct rb_root *rb)
{
	return __BSTLINK_FIRST(rb->node, struct rb_node);
}

static inline struct rb_node *rb_last(const struct rb_root *rb)
{
	return __BSTLINK_LAST(rb->node, struct rb_node);
}

static inline struct rb_node *rb_next(const struct rb_node* node)
{
	return __BSTLINK_NEXT(node, struct rb_node);
}

static inline struct rb_node *rb_prev(const struct rb_node* node)
{
	return __BSTLINK_PREV(node, struct rb_node);
}

void rb_insert_rebalance(struct rb_node *node, struct rb_root *rb);
void rb_erase(struct rb_node *node, struct rb_root *rb);


/* helper routine */
static inline struct rb_node *
rb_find(const struct rb_root *rb,
	int (*compare)(const struct rb_node *node,
		       const void *arg),
        const void *arg)
{
	return __BSTLINK_FIND(rb->node, compare, arg, struct rb_node);
}

static inline struct rb_node *
rb_lower_bound(const struct rb_root *rb,
	       int (*compare)(const struct rb_node *node,
			      const void *arg),
	       const void *arg)
{
	return __BSTLINK_LOWER_BOUND(rb->node, compare, arg, struct rb_node);
}

static inline struct rb_node *
rb_upper_bound(const struct rb_root *rb,
	       int (*compare)(const struct rb_node *node,
			      const void *arg),
	       const void *arg)
{
	return __BSTLINK_UPPER_BOUND(rb->node, compare, arg, struct rb_node);
}

static inline void
rb_lower_upper_bound(const struct rb_root *rb,
		     int (*compare)(const struct rb_node *node,
				    const void *arg),
		     const void *arg,
		     struct rb_node **plower,
		     struct rb_node **pupper)
{
	return __BSTLINK_LOWER_UPPER_BOUND(rb->node, compare, arg,
					   plower, pupper);
}

static inline void
__rb_insert_rest(struct rb_node *node, struct rb_root *rb)
{
	rb_set_red(node);
	rb_insert_rebalance(node, rb);
}

static inline void
rb_insert(struct rb_node *node,
	  struct rb_root *rb,
	  int (*compare_link)(const struct rb_node *node1,
			      const struct rb_node *node2,
			      const void *arg),
	  const void *arg)
{
	(void)__BSTLINK_INSERT(node, &rb->node, compare_link,
				       arg, false);
	__rb_insert_rest(node, rb);
}

static inline bool
rb_insert_unique(struct rb_node *node,
		 struct rb_root *rb,
		 int (*compare_link)(const struct rb_node *node1,
				     const struct rb_node *node2,
				     const void *arg),
		 const void *arg)
{
	
	if (__BSTLINK_INSERT(node, &rb->node, compare_link,
				       arg, false))
	{
		__rb_insert_rest(node, rb);
		return true;
	}

	return false;
}

static inline void
rb_replace(struct rb_node *victim, struct rb_node *new, struct rb_root *rb)
{
	__BSTLINK_REPLACE(victim, new, &rb->node);
	rb_set_color(new, victim->color);
}

static inline void rb_erase_range(struct rb_node *beg,
				  struct rb_node *end,
				  struct rb_root *rb)
{
	__BSTLINK_ERASE_RANGE(beg, end, rb_erase, rb);
}

static inline void
rb_erase_equal(struct rb_root *rb,
	       int (*compare)(const struct rb_node *node, const void *arg),
	       void (*destroy)(struct rb_node *node, const void *arg),
	       const void *arg_compare,
	       const void *arg_destroy)
{
	__BSTLINK_ERASE_EQUAL(rb->node, compare, rb_erase, destroy,
			      arg_compare, rb, arg_destroy);
}

static inline size_t
rb_count(const struct rb_root *rb,
	 int (*compare)(const struct rb_node *node1,
			     const void *arg),
	 const void *arg)
{
	return __BSTLINK_COUNT(rb->node, compare, arg);
}

static inline void
rb_clear(struct rb_root *rb,
	 void (*destroy)(struct rb_node *node, const void *arg),
	 const void *arg)
{
	__BSTLINK_DESTROY(rb->node, destroy, arg);
	rb->node = NULL;
}

static inline void
rb_visit(const struct rb_root *rb,
	 void (*visit)(const struct rb_node *node, const void *arg),
	 const void *arg)
{
	__BSTLINK_VISIT(rb->node, visit, arg);
}

static inline bool
rb_visit_cond(const struct rb_root *rb,
	      bool (*visit_cond)(const struct rb_node *node, const void *arg),
	      const void *arg)
{
	return __BSTLINK_VISIT_COND(rb->node, visit_cond, arg);
}

static inline size_t
rb_height(struct rb_root *rb, bool bmax)
{
	return __BSTLINK_HEIGHT(rb->node, bmax);
}
#define rb_height_max(rb)	rb_height(rb, true)
#define rb_height_min(rb)	rb_height(rb, false)

/* valid check */
#ifndef NDEBUG
bool rb_isvalid(struct rb_root *rb);
#else
static inline bool rb_isvalid(struct rb_root *rb)
{
	return true;
}
#endif

struct rb_root *rb_alloc(size_t num);
void rb_free(struct rb_root *rb,
	     size_t num,
	     void (*destroy)(struct rb_node *rb, const void *arg),
	     const void *arg);

bool rb_clone(struct rb_root *rb,
	      const struct rb_root *rb_src,
	      struct rb_node *(*node_clone)(const struct rb_node *node,
					    const void *arg),
	      void (*node_destroy)(struct rb_node *node, const void *arg),
	      const void *arg_clone,
	      const void *arg_destroy);

/* clone range: [beg, end) */
bool rb_clone_range(struct rb_root *rb,
		    const struct rb_node *beg,
		    const struct rb_node *end,
		    struct rb_node *(*node_clone)(const struct rb_node *node,
						  const void *arg),
		    void (*node_destroy)(struct rb_node *node,
					 const void *arg),
		    const void *arg_clone,
		    const void *arg_destroy);

static inline void rb_swap(struct rb_root *rb1, struct rb_root *rb2)
{
	struct rb_node *node = rb1->node;
	rb1->node = rb2->node;
	rb2->node = node;
}

__END_DECLS

#endif /* __YCC_ALGOS_RBTREE_H_ */
