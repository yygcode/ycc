/*
 * slist.h -- Single-Linked List
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

#ifndef __YCC_ALGOS_SLIST_H_
#define __YCC_ALGOS_SLIST_H_	1

#include <ycc/config-ycc.h>

__BEGIN_DECLS

struct slist_head {
	struct slist_head *next;
};

#define SLIST_HEAD_INIT(name) { &(name), }
#define SLIST_HEAD(name)	\
		struct slist_head name = SLIST_HEAD_INIT(name)

static inline void __slist_add(struct slist_head *new, struct slist_head *prev)
{
	new->next = prev->next;
	prev->next = new;
}

static inline void __slist_del(struct slist_head *prev)
{
	prev->next = prev->next->next;
}

static inline struct slist_head *__slist_prev(struct slist_head *entry)
{
	struct slist_head *prev = entry->next;

	while (prev->next != entry)
		prev = prev->next;

	return prev;
}

static inline void slist_add(struct slist_head *new, struct slist_head *head)
{
	__slist_add(new, head);
}

static inline void slist_del(struct slist_head *prev)
{
	__slist_del(prev);
}

static inline void __slist_del_entry(struct slist_head *entry)
{
	__slist_del(__slist_prev(entry));
}

static inline void __slist_add_prev(struct slist_head *new,
				    struct slist_head *next)
{
	__slist_add(new, __slist_prev(next));
}

#define slist_entry(ptr, type, member)			\
	container_of(ptr, type, member)

#define slist_for_each_entry(pos, head, member)				\
	for (pos = slist_entry((head)->next, typeof(*pos), member);	\
	     &pos->member != (head);					\
	     pos = slist_entry(pos->member.next, typeof(*pos), member))

#define slist_for_each_entry_safe(pos, n, head, member)			\
	for (pos = slist_entry((head)->next, typeof(*pos), member),	\
	     n = slist_entry(pos->member.next, typeof(*pos), member);	\
	     &pos->member != (head);					\
	     pos = n, n = slist_entry(n->member.next, typeof(*n), member))
void slist_reverse(struct slist_head *head);
void slist_sort(void *priv, struct slist_head *head,
		int (*cmp)(void *priv, struct slist_head *a,
			   struct slist_head *b));

__END_DECLS

#endif
