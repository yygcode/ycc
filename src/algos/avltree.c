/*
 * avltree.c -- Self-balancing Binary Search Trees
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

#include <config-os.h>

#include <ycc/algos/avltree.h>

void avl_insert_rebalance(struct avl_node *node, struct avl_root *avl)
{
	struct avl_node *parent, *gparent;
	struct avl_node ** const proot = &avl->node;

	/* do rebalance */
	while ((parent=node->parent) && node->depth == parent->depth) {
		++parent->depth;

		if (!(gparent=parent->parent))
			break;

		if (parent == gparent->left) {
			register struct avl_node *uncle = gparent->right;

			if ((uncle && parent->depth - uncle->depth > 1) ||
			    !uncle) {
				if (node == parent->right) {
					++node->depth;
					--parent->depth;
					__BSTLINK_ROTATE_LEFT(parent, proot);
				}

				--gparent->depth;
				__BSTLINK_ROTATE_RIGHT(gparent, proot);
			}
			else
				node = parent;
		} else {
			register struct avl_node *uncle = gparent->left;

			if ((uncle && parent->depth - uncle->depth > 1) ||
			    !uncle) {
				if (node == parent->left) {
					++node->depth;
					--parent->depth;
					__BSTLINK_ROTATE_RIGHT(parent, proot);
				}

				--gparent->depth;
				__BSTLINK_ROTATE_LEFT(gparent, proot);
			}
			else
				node = parent;
		}
	}
}

static inline void
__avl_erase_rebalance(struct avl_node *node,
		      struct avl_node *parent,
		      struct avl_node **proot)
{
	struct avl_node *other;

	/* empty tree */
	assert(parent);

	while (node != *proot) {
		unsigned depth = node ? node->depth : 1;

		if (depth + 1 == parent->depth)
			return;

		if (parent->left == node) {
			other = parent->right;

			if (other->depth + 1 != parent->depth) {
				--parent->depth;
				goto down;
			}

			if (other->depth == depth + 1)
				return;

			if (other->left &&
			    (!other->right ||
			     other->left->depth > other->right->depth)) {
				++other->left->depth;
				--other->depth;
				__BSTLINK_ROTATE_RIGHT(other, proot);
				other = other->parent;
			}

			if (other->left && other->left->depth > depth) {
				--parent->depth;
				++other->depth;
			} else
				parent->depth -= 2;

			__BSTLINK_ROTATE_LEFT(parent, proot);
			parent = other;
		} else {
			other = parent->left;

			if (other->depth + 1 != parent->depth) {
				--parent->depth;
				goto down;
			}

			if (other->depth == depth + 1)
				return;

			if (other->right &&
			    (!other->left ||
			     other->right->depth > other->left->depth)) {
					++other->right->depth;
					--other->depth;
					__BSTLINK_ROTATE_LEFT(other, proot);
					other = other->parent;
			}

			if (other->right && other->right->depth > depth) {
				--parent->depth;
				++other->depth;
			} else
				parent->depth -= 2;

			__BSTLINK_ROTATE_RIGHT(parent, proot);
			parent = other;
		}

		down:
			node = parent;
			parent = parent->parent;
	}
}

/* erase specialized */
#define __BSTLINK_TYPE struct avl_node
#define __BSTLINK_ERASE_SPECIALIZE_BOTH(node, scor)		\
	avl_set_depth(scor, avl_depth(node))
#define __BSTLINK_ERASE_SPECIALIZE_DO(child, parent, proot)	\
	if (parent)						\
		__avl_erase_rebalance(child, parent, proot);

#include "bstree-internal.h"
void avl_erase(struct avl_node *node, struct avl_root *avl)
{
	__BSTLINK_ERASE(node, &avl->node);
}

#ifndef NDEBUG
#include <ycc/common/debug.h>
static bool __avl_isvalid(struct avl_node *root)
{
	size_t droot, dleft, dright, dmax = 0;

	if (!root)
		return true;

	droot = root->depth;

	if (!root->left && !root->right) {
		if (droot != 1) {
			DBG_PR("droot = %zu\n", droot);
			return false;
		}
		return true;
	}

	if (!root->left) {
		dright = root->right->depth;
		if (dright != 1 || droot != 2) {
			DBG_PR("droot = %zu, dright = %zu\n", droot, dright);
			return false;
		}

		return __avl_isvalid(root->right);
	}

	if (!root->right) {
		dleft = root->left->depth;
		if (dleft != 1 || droot != 2) {
			DBG_PR("droot = %zu, dleft = %zu\n", dleft, dleft);
			return false;
		}

		return __avl_isvalid(root->left);
	}

	dleft = root->left->depth;
	dright = root->right->depth;

	if (dleft > dright)
		dmax = dleft;
	else
		dmax = dright;

	if (dmax - dleft > 1 || dmax - dright > 1) {
		DBG_PR("dleft = %zu, dright = %zu\n", dleft, dright);
		return false;
	}

	if (droot != dmax + 1) {
		DBG_PR("droot = %zu, dleft = %zu, dright = %zu\n",
			droot, dleft, dright);
		return false;
	}

	return __avl_isvalid(root->left) && __avl_isvalid(root->right);
}

bool avl_isvalid(struct avl_root *avl)
{
	return __avl_isvalid(avl->node);
}
#endif

/* eof */
