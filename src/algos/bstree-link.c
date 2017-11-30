/*
 * bstree-link.c -- Binary-Search-Trees Common Routines
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

#include <assert.h>
#include <stddef.h>

#include <config-os.h>

#include <ycc/algos/bstree-link.h>

/*
 * bstlink_rotate_left
 *
 *  Illustration
 *      |               |
 *      X               Y
 *     / \             / \
 *    T1  Y  ---->    X   T3
 *       / \         / \
 *      T2 T3      T1  T2
 *
 *  Prerequisites
 *    X and Y(the right child of X) MUST not be NULL
 *    T1, T2, and T3 are subtrees which can be empty or non-empty
 */
void bstlink_rotate_left(struct bst_link *link, struct bst_link **proot)
{
	struct bst_link *right, *parent;

	assert(link);
	assert(proot);
	assert(link->right);

	right = link->right;
	parent = link->parent;

	if ((link->right = right->left))
		right->left->parent = link;
	right->left = link;

	right->parent = parent;

	if (parent) {
		if (link == parent->left)
			parent->left = right;
		else
			parent->right = right;
	} else
		*proot = right;

	link->parent = right;
}

/*
 * bstlink_rotate_right
 *
 *  Illustration
 *        |            |
 *        X            Y
 *       / \          / \
 *      Y  T1  --->  T2  X
 *     / \              / \
 *    T2 T3            T3  T1
 *
 *  Prerequisites
 *    X and Y(the left child of X) MUST not be NULL
 *    T1, T2, and T3 are subtrees which can be empty or non-empty
 *
 */
void bstlink_rotate_right(struct bst_link *link, struct bst_link **proot)
{
	struct bst_link *left, *parent;

	assert(link);
	assert(proot);
	assert(link->left);

	left = link->left;
	parent = link->parent;

	if ((link->left=left->right))
		left->right->parent = link;
	left->right = link;

	left->parent = parent;

	if (parent) {
		if (link == parent->right)
			parent->right = left;
		else
			parent->left = left;
	} else
		*proot = left;

	link->parent = left;
}

struct bst_link *bstlink_first(const struct bst_link *link)
{
	if (!link)
		return NULL;

	while (link->left)
		link = link->left;

	return (struct bst_link*)link;
}

struct bst_link *bstlink_last(const struct bst_link *link)
{
	if (!link)
		return NULL;

	while (link->right)
		link = link->right;

	return (struct bst_link*)link;
}

struct bst_link *bstlink_next(const struct bst_link *link)
{
	struct bst_link *parent;

	/*
	 * If we have a right-child, go down and then
	 * left as far as we can.
	 */
	if (link->right) {
		link = link->right;
		while (link->left)
			link = link->left;
		return (struct bst_link*)link;
	}

	/*
	 * No right-child, go up and find the first ancestor
	 * which right-subtree does not include link.
	 * Otherwise return NULL.
	 */
	while ((parent=link->parent) && link == parent->right)
		link = parent;

	return (struct bst_link*)parent;
}

struct bst_link *bstlink_prev(const struct bst_link *link)
{
	struct bst_link *parent;

	/*
	 * If we have a left-child, go down and then
	 * right as far as we can.
	 */
	if (link->left) {
		link = link->left;
		while (link->right)
			link = link->right;

		return (struct bst_link*)link;
	}

	/*
	 * No right-child, go up and find the first ancestor
	 * which left-subtree does not include link.
	 * Otherwise return NULL.
	 */
	while ((parent=link->parent) && link == parent->left)
		link = parent;

	return (struct bst_link*)parent;
}

void bstlink_replace(struct bst_link *victim,
		     struct bst_link *new,
		     struct bst_link **proot)
{
	struct bst_link *parent = victim->parent;

	if (parent) {
		if (victim == parent->left)
			parent->left = new;
		else
			parent->right = new;
	} else
		*proot = new;

	if (victim->left)
		victim->left->parent = new;
	if (victim->right)
		victim->right->parent = new;

	*new = *victim;
}

struct bst_link *
bstlink_find(const struct bst_link *link,
	     bstlink_compare_t compare,
	     const void *arg)
{
	struct bst_link *r = NULL;

	while(link) {
		int icmp = compare(link, arg);

		if (icmp < 0)
			link = link->right;
		else {
			if (icmp == 0)
				r = (struct bst_link*)link;
			else if (r)
				break;

			link = link->left;
		}
	}

	return r;
}

struct bst_link *
bstlink_lower_bound(const struct bst_link *link,
		    bstlink_compare_t compare,
		    const void *arg)
{
	struct bst_link *lb = NULL;

	while (link) {
		if (compare(link, arg) >= 0) {
			lb = (struct bst_link*)link;
			link = link->left;
		} else
			link = link->right;
	}

	return lb;
}

struct bst_link *
bstlink_upper_bound(const struct bst_link *link,
		    bstlink_compare_t compare,
		    const void *arg)
{
	struct bst_link *ub = NULL;

	while (link) {
		if (compare(link, arg) > 0) {
			ub = (struct bst_link*)link;
			link = link->left;
		} else
			link = link->right;
	}

	return ub;
}

void bstlink_lower_upper_bound(const struct bst_link *link,
			       bstlink_compare_t compare,
			       const void *arg,
			       struct bst_link **plb,
			       struct bst_link **pub)
{
	*plb = *pub = NULL;

	while (link) {
		int icmp = compare(link, arg);
		if (icmp >= 0) {
			*plb = (struct bst_link*)link;
			if (icmp)
				*pub = (struct bst_link*)link;
			link = link->left;
		} else
		link = link->right;
	}
}

bool bstlink_insert(struct bst_link *link,
		    struct bst_link **proot,
		    bstlink_compare_link_t compare_link,
		    const void *arg,
		    bool bunique)
{
	struct bst_link *parent = NULL;

	while (*proot) {
		int icmp = compare_link(*proot, link, arg);

		parent = *proot;

		if (icmp > 0)
			proot = &(*proot)->left;
		else {
			/* For bunique == true,
			 * if rbtree had have a node which value equals to
			 * the insert-node, the insert-operation fail.
			 */
			 if (!icmp && bunique)
				 return false;

			proot = &(*proot)->right;
		}
	}

	bstlink_init(link, parent, proot);

	return true;
}

#if 0
void bstlink_erase(struct bst_link *link, struct bst_link **proot)
{
	struct bst_link *child, *parent;
#ifdef __BSTLINK_ERASE_SPECIALIZE_DECLARE
	__BSTLINK_ERASE_SPECIALIZE_DECLARE();
#endif

	if (!link->left || !link->right) {
		if (!link->left)
			child = link->right;
		else
			child = link->left;

		parent = link->parent;
		if (child)
			child->parent = parent;
		if (parent) {
			if (parent->left == link)
				parent->left = child;
			else
				parent->right = child;
		} else
			*proot = child;
#ifdef __BSTLINK_ERASE_SPECIALIZE_SINGLE
	__BSTLINK_ERASE_SPECIALIZE_SINGLE();
#endif
	} else {
		struct bst_link *scor;	/* scor: successor */
		
		scor = link->right;
		while (scor->left)
			scor = scor->left;

		if (link->parent) {
			if (link->parent->left == link)
				link->parent->left = scor;
			else
				link->parent->right = scor;
		} else
			*proot = scor;

		child = scor->right;
		parent = scor->parent;

		if (parent == link)
			parent = scor;
		else {
			if (child)
				child->parent = parent;

			parent->left = child;
			scor->right = link->right;
			link->right->parent = scor;
		}

		scor->parent = link->parent;
		scor->left = link->left;
		link->left->parent = scor;
#ifdef __BSTLINK_ERASE_SPECIALIZE_BOTH
	__BSTLINK_ERASE_SPECIALIZE_BOTH();
#endif
	}

#ifdef __BSTLINK_ERASE_SPECIALIZE_DO
	__BSTLINK_ERASE_SPECIALIZE_DO(child, parent, proot);
#endif
}
#endif

size_t bstlink_count(const struct bst_link *link,
		     bstlink_compare_t compare,
		     const void *arg)
{
	size_t c = 0;
	struct bst_link *lb, *ub;

	bstlink_lower_upper_bound(link, compare, arg, &lb, &ub);

	while (lb != ub) {
		++c;
		lb = bstlink_next(lb);
	}

	return c;
}

void bstlink_destroy(struct bst_link *link,
		     bstlink_destroy_t destroy,
		     const void *arg)
{
	while (link) {
		struct bst_link *right = link->right;
		bstlink_destroy(link->left, destroy, arg);
		destroy(link, arg);
		link = right;
	}
}

/* inorder-traverse */
void bstlink_visit(struct bst_link *link,
		   bstlink_visit_t visit,
		   const void *arg)
{
	while (link) {
		bstlink_visit(link->left, visit, arg);
		visit(link, arg);
		link = link->right;
	}
}

bool bstlink_visit_cond(struct bst_link *link,
			bstlink_visit_cond_t visit_cond,
			const void *arg)
{
	while (link) {
		if (!bstlink_visit_cond(link->left, visit_cond, arg) ||
		    !visit_cond(link, arg))
			return false;

		link = link->right;
	}

	return true;
}

size_t bstlink_height(const struct bst_link *link, bool bmax)
{
	size_t h = 0;

	if (link) {
		size_t left = bstlink_height(link->left, bmax);
		size_t right = bstlink_height(link->right, bmax);
#define __BSTLINK_MAX(x, y)	( (x) > (y) ? (x) : (y) )
#define __BSTLINK_MIN(x, y)	( (x) < (y) ? (x) : (y) )
		if (bmax)
			h = __BSTLINK_MAX(left, right);
		else
			h = __BSTLINK_MIN(left, right);

		++h;
	}

	return h;
}

/* eof */
