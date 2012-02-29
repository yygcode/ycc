/*
 * treap.c -- Randomized Binary Search Trees
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

#include <ycc/algos/treap.h>

void treap_insert_rebalance(struct treap_node *node, struct treap_root *treap)
{
	struct treap_node **proot = &treap->node;

	while (node != *proot) {
		struct treap_node *parent = node->parent;

		if (node->priority >= parent->priority)
			return;

		if (node == parent->left)
			__BSTLINK_ROTATE_RIGHT(parent, proot);
		else
			__BSTLINK_ROTATE_LEFT(parent, proot);
	}
}

#include "bstree-internal.h"
#define __BSTLINK_TYPE struct treap_node
#define __BSTLINK_ERASE_SPECIALIZE_BOTH(node, scor)		\
		treap_set_priority(scor, treap_priority(node))
void treap_erase(struct treap_node *node, struct treap_root *treap)
{
	__BSTLINK_ERASE(node, &treap->node);
}

#ifndef NDEBUG
static bool __treap_isvalid(struct treap_node *root)
{
	unsigned priority = (unsigned)-1;

	if (!root)
		return true;

	if (root->left)
		priority = root->left->priority;

	if (root->right && root->right->priority < priority)
		priority = root->right->priority;

	if (priority < root->priority)
		return false;

	if (!root->left)
		return __treap_isvalid(root->right);
	else if (!root->right)
		return __treap_isvalid(root->left);
	else
		return __treap_isvalid(root->left) &&
		       __treap_isvalid(root->right);
}

bool treap_isvalid(struct treap_root *treap)
{
	return __treap_isvalid(treap->node);
}
#endif

/* eof */
