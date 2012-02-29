/*
 * bstree-link.h -- Binary-Search-Trees Common Routines
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
 * You always need not to include this file directly.
 * This file is designed to provide basic binary search tree routines,
 * And the interface is volatile 
 */

#ifndef __YCC_YCALGOS_BSTREE_LINK_H_
#define __YCC_YCALGOS_BSTREE_LINK_H_

#include <stddef.h>
#include <stdbool.h>

#include <ycc/config-ycc.h>

__BEGIN_DECLS

/* bstlink: Binary Search Tree Link */
#define __BST_LINK_MEMBER(type)	\
	type *parent, *left, *right
struct bst_link
{
	__BST_LINK_MEMBER(struct bst_link);
}__aligned(sizeof(void*));

void bstlink_rotate_left(struct bst_link *link, struct bst_link **pproot);
void bstlink_rotate_right(struct bst_link *link, struct bst_link **proot);

struct bst_link *bstlink_first(const struct bst_link *link);
struct bst_link *bstlink_last(const struct bst_link *link);
struct bst_link *bstlink_next(const struct bst_link *link);
struct bst_link *bstlink_prev(const struct bst_link *link);

void bstlink_replace(struct bst_link *victim,
		     struct bst_link *new,
		     struct bst_link **proot);

/* auto-convert macros */
#define __BSTLINK_ROTATE_LEFT(link, pproot)				\
		bstlink_rotate_left					\
		(							\
			(struct bst_link*)link,				\
			(struct bst_link**)pproot			\
		)

#define __BSTLINK_ROTATE_RIGHT(link, pproot)				\
		bstlink_rotate_right					\
		(							\
			(struct bst_link*)link,				\
			(struct bst_link**)pproot			\
		)

#define __BSTLINK_FIRST(link, type)					\
		(type*)							\
		bstlink_first						\
		(							\
			(const struct bst_link*)(link)			\
		)

#define __BSTLINK_LAST(link, type)					\
		(type*)							\
		bstlink_last						\
		(							\
			(const struct bst_link*)(link)			\
		)

#define __BSTLINK_NEXT(link, type)					\
		(type*)							\
		bstlink_next						\
		(							\
			(const struct bst_link*)(link)			\
		)

#define __BSTLINK_PREV(link, type)					\
		(type*)							\
		bstlink_prev						\
		(							\
			(const struct bst_link*)(link)			\
		)

#define __BSTLINK_REPLACE(victim, new, proot)				\
		bstlink_replace						\
		(							\
			(struct bst_link*)(victim),			\
			(struct bst_link*)(new),			\
			(struct bst_link**)(proot)			\
		)

/*
 * helper-routine
 * You may need to implement the follows by yourself to
 * ensure some performances
 */
typedef void (*bstlink_erase_t)(struct bst_link *link, const void *arg);
typedef void (*bstlink_destroy_t)(struct bst_link *link, const void *arg);
typedef int (*bstlink_compare_t)(const struct bst_link *link, const void *arg);
typedef int (*bstlink_compare_link_t)(const struct bst_link *link1,
				      const struct bst_link *link2,
				      const void *arg);
typedef void (*bstlink_visit_t)(const struct bst_link *link, const void *arg);
typedef bool (*bstlink_visit_cond_t)(const struct bst_link *link,
				     const void *arg);

static inline void bstlink_init(struct bst_link *link,
				struct bst_link *parent,
				struct bst_link **plink)
{
	link->parent = parent;
	link->left = link->right = NULL;
	*plink = link;
}

/*
 * bstlink_find  --  find the left-most node
 *
 * Description
 *	The function finds the left-most node of the 'link' subtree,
 *	the node's value is equals to 'arg' which judged by routine
 *	'compare'
 *
 * Parameters
 *	link	: the root node of search-tree
 *	compare	: pointer of value compares routine
 *	arg	: transfer to 'compare' transparently
 *
 * Return value
 *	The left-most node which value is equals to 'arg' or
 *	NULL if no-match found.
 */
struct bst_link *bstlink_find(const struct bst_link *link,
			      bstlink_compare_t compare,
			      const void *arg);

/*
 * bstlink_lower_bound
 *
 * Description
 *	The function finds the first node of the 'link' subtree,
 *	the node's value is equals to or greater than 'arg' which
 *	judged by routine 'compare'
 *
 * Parameters
 *	link	: the root node of search-tree
 *	compare	: pointer of value compares routine
 *	arg	: transfer to 'compare' transparently
 *
 * Return value
 *	The first node which value is equals to or greater than 'arg' or
 *	NULL if no-match found.
 */
struct bst_link *bstlink_lower_bound(const struct bst_link *link,
				     bstlink_compare_t compare,
				     const void *arg);

/*
 * bstlink_upper_bound
 *
 * Description
 *	The function finds the first node of the 'link' subtree,
 *	the node's value is greater than 'arg' which judged by
 *	routine 'compare'
 *
 * Parameters
 *	link	: the root node of search-tree
 *	compare	: pointer of value compares routine
 *	arg	: transfer to 'compare' transparently
 *
 * Return value
 *	The first node which value is greater than 'arg' or
 *	NULL if no-match found.
 */
struct bst_link *bstlink_upper_bound(const struct bst_link *link,
				     bstlink_compare_t compare,
				     const void *arg);

/* bstlink_lower_upper_bound -- the merge of two routines above */
void bstlink_lower_upper_bound(const struct bst_link *link,
			       bstlink_compare_t compare,
			       const void *arg,
			       struct bst_link **plb,
			       struct bst_link **pub);

bool bstlink_insert(struct bst_link *link,
		    struct bst_link **proot,
		    bstlink_compare_link_t compare_link,
		    const void *arg,
		    bool bunique);

static inline void bstlink_erase_range(struct bst_link *beg,
				       struct bst_link *end,
				       bstlink_erase_t erase,
				       const void *arg
				       )
{
	while (beg != end) {
		struct bst_link *del = beg;
		beg = bstlink_next(beg);
		erase(del, arg);
	}
}

static inline void bstlink_erase_equal(struct bst_link *root,
				       bstlink_compare_t compare,
				       bstlink_erase_t erase,
				       bstlink_destroy_t destroy,
				       const void *arg_compare,
				       const void *arg_erase,
				       const void *arg_destroy)
{
	struct bst_link *lb, *ub;

	bstlink_lower_upper_bound(root, compare, arg_compare, &lb, &ub);

	while (lb != ub) {
		struct bst_link *del = lb;
		lb = bstlink_next(lb);
		erase(del, arg_erase);
		destroy(del, arg_destroy);
	}
}

/*
 * bstlink_count  --  count of nodes which value equal to
 */
size_t bstlink_count(const struct bst_link *link,
		   bstlink_compare_t compare,
		   const void *arg);

/* destroy all link and its descendant */
void bstlink_destroy(struct bst_link *link,
		   bstlink_destroy_t destroy,
		   const void *arg);

/* inorder-traverse */
void bstlink_visit(struct bst_link *link,
		 bstlink_visit_t visit,
		 const void *arg);
bool bstlink_visit_cond(struct bst_link *link,
		      bstlink_visit_cond_t visit_cond,
		      const void *arg);

size_t bstlink_height(const struct bst_link *link, bool bmax);

#define __BSTLINK_INIT(link, parent, plink)				\
		bstlink_init						\
		(							\
			(struct bst_link*)(link),			\
			(struct bst_link*)(parent),			\
			(struct bst_link**)(plink)			\
		)

#define __BSTLINK_FIND(link, compare, arg, type)			\
		(type*)							\
		bstlink_find						\
		(							\
			(const struct bst_link*)(link),			\
			(bstlink_compare_t)(compare),			\
			(const void*)(arg)				\
		)

#define __BSTLINK_LOWER_BOUND(link, compare, arg, type)			\
		(type*)							\
		bstlink_lower_bound					\
		(							\
			(struct bst_link*)(link),			\
			(bstlink_compare_t)(compare),			\
			(const void*)(arg)				\
		)

#define __BSTLINK_UPPER_BOUND(link, compare, arg, type)			\
		(type*)							\
		bstlink_upper_bound					\
		(							\
			(struct bst_link*)(link),			\
			(bstlink_compare_t)(compare),			\
			(const void*)(arg)				\
		)

#define __BSTLINK_LOWER_UPPER_BOUND(link, compare, arg, plower, pupper)	\
		bstlink_lower_upper_bound				\
		(							\
			(const struct bst_link*)(link),			\
			(bstlink_compare_t)(compare),			\
			(const void*)(arg),				\
			(struct bst_link**)(plower),			\
			(struct bst_link**)(pupper)			\
		)

#define __BSTLINK_INSERT(link, proot,				\
				 compare_link,arg, bunique)		\
		bstlink_insert						\
		(							\
			(struct bst_link*)(link),			\
			(struct bst_link**)(proot),			\
			(bstlink_compare_link_t)(compare_link),		\
			(const void*)(arg),				\
			(bunique)					\
		)

#define __BSTLINK_ERASE_RANGE(beg, end, erase, arg)			\
		bstlink_erase_range					\
		(							\
			(struct bst_link*)(beg),			\
			(struct bst_link*)(end),			\
			(bstlink_erase_t)(erase),			\
			(const void*)(arg)				\
		)

#define __BSTLINK_ERASE_EQUAL(root, compare, erase, destroy,		\
			      arg_compare, arg_erase, arg_destroy)	\
		bstlink_erase_equal					\
		(							\
			(struct bst_link*)(root),			\
			(bstlink_compare_t)(compare),			\
			(bstlink_erase_t)(erase),			\
			(bstlink_destroy_t)(destroy),			\
			(const void*)(arg_compare),			\
			(const void*)(arg_erase),			\
			(const void*)(arg_destroy)			\
		)

#define __BSTLINK_COUNT(link, compare, arg)				\
		bstlink_count						\
		(							\
			(struct bst_link*)(link),			\
			(bstlink_compare_t)(compare),			\
			(const void*)(arg)				\
		)

#define __BSTLINK_DESTROY(link, destroy, arg)				\
		bstlink_destroy						\
		(							\
			(struct bst_link*)(link),			\
			(bstlink_destroy_t)(destroy),			\
			(const void*)(arg)				\
		)

#define __BSTLINK_VISIT(link, visit, arg)				\
		bstlink_visit						\
		(							\
			(struct bst_link*)(link),			\
			(bstlink_visit_t)(visit),			\
			(const void*)(arg)				\
		)

#define __BSTLINK_VISIT_COND(link, visit_cond, arg)			\
		bstlink_visit_cond					\
		(							\
			(struct bst_link*)(link),			\
			(bstlink_visit_cond_t)(visit_cond),		\
			(const void*)(arg)				\
		)

#define __BSTLINK_HEIGHT(link, bmax)					\
		bstlink_height						\
		(							\
			(const struct bst_link*)(link),			\
			(bmax)						\
		)

__END_DECLS

#endif	/* __YCC_YCALGOS_BSTREE_LINK_H_ */
