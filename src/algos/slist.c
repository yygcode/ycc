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

#include <config-os.h>

#include <ycc/algos/slist.h>

void slist_reverse(struct slist_head *head)
{
	struct slist_head *prev, *next, *pos;

	prev = head;
	pos = prev->next;

	while (pos != head) {
		next = pos->next;
		pos->next = prev;
		prev = pos;
		pos = next;
	}
	pos->next = prev;
}

void slist_sort(void *priv, struct slist_head *head,
		int (*cmp)(void *priv, struct slist_head *a,
			   struct slist_head *b))
{
	size_t mloop, msize = 1;

	/*
	 * mloop: merge-loop count
	 * msize: the merge-loop maximum merge-size
	 */

	do {
		/* partial1 (beg1) begins with first node. */
		struct slist_head *beg1 = head->next, *pos = head;

		mloop = 0;

		do {
			size_t n1, n2;
			struct slist_head *beg2, *end1, *end2;

			for (n1 = 1, end1 = beg1;
			     n1 < msize && end1->next != head;
			     ++n1, end1 = end1->next);
			beg2 = end2 = end1->next;
			for (n2 = 0;
			     n2 < msize && end2 != head;
			     ++n2, end2 = end2->next);

			while (n1 && n2) {
				if (cmp(priv, beg1, beg2) <= 0) {
					pos->next = beg1;
					beg1 = beg1->next;
					--n1;
				} else {
					pos->next = beg2;
					beg2 = beg2->next;
					--n2;
				}
				pos = pos->next;
			}

			if (n1) pos = end1;
			else pos = end2;
			pos->next = head;
			beg1 = end2->next;
			++mloop;
		} while (beg1 != head);
		msize <<= 1;	/* msize x2 */
	} while (mloop > 1);
}
