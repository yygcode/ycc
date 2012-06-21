/*
 * strbmh.c -- Exact String Matching: Boyer-Moore-Horspool [Hor 80]
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

/*
 * [Hor 80] Practical Fast Searching in Strings,
 *          R. Nigel Horspool,
 *          Softw. Pratt. Exp., 10, 501–506 (1980).
 */

#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <sys/types.h>

#include <config-os.h>

#include <ycc/algos/string.h>

void strbmh_init(const char *needle, size_t n, size_t *table)
{
	size_t i;

	for (i = 0; i <= UCHAR_MAX; ++i)
		table[i] = n;

	--needle;
	for (i = 1; i < n; ++i)
		table[(u_char)needle[i]] = n - i;
}

char *strbmh_find(const char *haystack, const char *needle,
		  size_t h, size_t n,
		  const size_t *table)
{
	size_t i, n1 = n - 1;

	if (!n)
		return (char*)haystack;

	while (h > n1) {
		for (i = n1; i != (size_t)-1 && haystack[i] == needle[i]; --i);

		if (i == (size_t)-1)	/* [0, n1] are equal. */
			return (char*)haystack;

		h -= table[(u_char)haystack[n1]];
		haystack += table[(u_char)haystack[n1]];
	}

	return NULL;
}

/* eof */
