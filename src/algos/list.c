/*
 * list.c -- Doubly-Linked List
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

#include <ycc/algos/list.h>

void list_reverse(struct list_head *head)
{
	struct list_head *next, *pos = head;

	do {
		next = pos->next;
		pos->next = pos->prev;
		pos->prev = next;
		pos = next;
	} while (pos != head);

	while (next != head) {
		*next = *head->next;

	}
}

void list_sort(void *priv, struct list_head *head,
	       int (*cmp)(void *priv, struct list_head *a,
			  struct list_head *b))
{
	struct list_head *p;

	size_t mloop, msize = 1;

	/*
	 * mloop: merge-loop count
	 * msize: the merge-loop maximum merge-size
	 */

	do {
		/* partial1 (beg1) begins with first node. */
		struct list_head *beg1 = head->next, *pos = head;

		mloop = 0;

		do {
			size_t n1, n2;
			struct list_head *beg2, *dummy;

			for (n1 = 1, beg2 = beg1->next;
			     n1 < msize && beg2 != head;
			     ++n1, beg2 = beg2->next);
			for (n2 = 0, dummy = beg2;
			     n2 < msize && dummy != head;
			     ++n2, dummy = dummy->next);

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

			if (n1) {
				pos->next = beg1;
				while (--n1) beg1 = beg1->next;
				pos = beg1;
			} else {
				pos->next = beg2;
				while (--n2) beg2 = beg2->next;
				pos = beg2;
				beg2 = beg2->next;
			}
			pos->next = head;
			beg1 = beg2;
			++mloop;
		} while (beg1 != head);
		msize <<= 1;	/* msize x2 */
	} while (mloop > 1);

	p = head;
	do {
		p->next->prev = p;
		p = p->next;
	} while (p != head);
}
