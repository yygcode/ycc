/*
 * bstree-internal.h -- Binary-Search-Trees Internal Routines
 *
 * Copyright (C) 2012-2013 yanyg (cppgp@qq.com)
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

#ifndef __YCALGOS_BSTREE_INTERNAL_H_
#define __YCALGOS_BSTREE_INTERNAL_H_

#include <stddef.h>
#include <stdbool.h>

#include <ycc/algos/bstree-link.h>

__BEGIN_DECLS

#define __BSTLINK_ERASE(link, proot)					\
		_bstlink_erase						\
		(							\
			(struct bst_link*)(link),			\
			(struct bst_link**)(proot)			\
		)

static inline void
_bstlink_erase(struct bst_link *link, struct bst_link **proot)
{
	struct bst_link *child, *parent;
#ifdef	__BSTLINK_ERASE_SPECIALIZE_DECLARE
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
#ifdef	__BSTLINK_ERASE_SPECIALIZE_SINGLE
	__BSTLINK_ERASE_SPECIALIZE_SINGLE((__BSTLINK_TYPE*)link);
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
#ifdef	__BSTLINK_ERASE_SPECIALIZE_BOTH
	__BSTLINK_ERASE_SPECIALIZE_BOTH((__BSTLINK_TYPE*)link,
					(__BSTLINK_TYPE*)scor);
#endif
	}

#ifdef	__BSTLINK_ERASE_SPECIALIZE_DO
	__BSTLINK_ERASE_SPECIALIZE_DO((__BSTLINK_TYPE*)child,
				      (__BSTLINK_TYPE*)parent,
				      (__BSTLINK_TYPE**)proot);
#endif
}

__END_DECLS

#endif	/* __YCALGOS_BSTREE_LINK_H_ */
