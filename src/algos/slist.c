/*
 * slist.c -- Single-Linked List
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

#include <ycc/algos/slist.h>

void slist_reverse(struct slist_head *head)
{
	struct slist_head *prev, *next, *entry;

	prev = head->next;
	entry = prev->next;
	prev->next = head;

	while (prev != head) {
		next = entry->next;
		entry->next = prev;
		prev = entry;
		entry = next;
	}
	//entry->next = prev;
}

void slist_sort(void *priv, struct slist_head *head,
		int (*cmp)(void *priv, struct slist_head *a,
			   struct slist_head *b))
{
}
