/*
 * raidx-tree.c -- Radix Tree
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

#include <ycc/algos/radix-tree.h>

struct raidx_tree_node {
	size_t height;	/* height from the root */
	size_t count;
	struct radix_tree_node *parent;
};

ssize_t raidx_tree_insert(struct radix_tree_root *root,
			  size_t index, void *item)
{
	struct radix_tree_node *node, *slot;
	size_t height, shift;
}
