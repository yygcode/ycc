/*
 * sptree.c -- Splay Trees
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

#include <config-os.h>

#include <ycc/algos/sptree.h>

void __spt_splay(struct spt_node *node, struct spt_node **proot)
{
	while (node != *proot) {
		struct spt_node *parent = node->parent;
		struct spt_node *gparent = parent->parent;

		if (node == parent->left) {
			if (!gparent) {
				__BSTLINK_ROTATE_RIGHT(parent, proot);
				continue;
			}

			if (parent == gparent->left) {
				__BSTLINK_ROTATE_RIGHT(gparent, proot);
				__BSTLINK_ROTATE_RIGHT(parent, proot);
			} else {
				__BSTLINK_ROTATE_RIGHT(parent, proot);
				__BSTLINK_ROTATE_LEFT(gparent, proot);
			}
		} else {
			if (!gparent) {
				__BSTLINK_ROTATE_LEFT(parent, proot);
				continue;
			}

			if (parent == gparent->right) {
				__BSTLINK_ROTATE_LEFT(gparent, proot);
				__BSTLINK_ROTATE_LEFT(parent, proot);
			} else {
				__BSTLINK_ROTATE_LEFT(parent, proot);
				__BSTLINK_ROTATE_RIGHT(gparent, proot);
			}
		}
	}
}

#define __BSTLINK_TYPE struct spt_node
#define __BSTLINK_ERASE_SPECIALIZE_DO(child, parent, proot)	\
	if (parent)						\
		__spt_splay(parent, proot);
#include "bstree-internal.h"
void spt_erase(struct spt_node *node, struct spt_root *spt)
{
	__BSTLINK_ERASE(node, &spt->node);
}

/* eof */
