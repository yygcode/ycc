/*
 * raidx-tree.h -- Radix Tree
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

#ifndef __YCC_ALGOS_RAIDX_TREE_H_
#define __YCC_ALGOS_RAIDX_TREE_H_	1

#include <ycc/config-ycc.h>

__BEGIN_DECLS

struct radix_tree_root {
	size_t height;
	struct radix_tree_node *rnode;
};

ssize_t raidx_tree_insert(struct radix_tree_root *root, size_t index, void *item);

__END_DECLS

#endif
