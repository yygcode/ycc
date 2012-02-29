/*
 * strkmp.c -- Exact String Matching: Knuth–Morris–Pratt [KMP 77]
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
 * [KMP 77] Fast pattern matching in strings,
 *          D. E. Knuth, J. H. Morris, Jr and V. B. Pratt, SIAM J.
 *          Computing, 6, 1977, pp. 323–350.
 */

#include <assert.h>
#include <stddef.h>

#include <config-os.h>

#include <ycc/algos/string.h>

void strkmp_init(const char *patn, size_t *table)
{
	size_t i, *t = table;
	const char *s = patn + 1;

	assert(patn && table);

	*t++ = 0;
	for(i = 0; *s; ++s) {
		while (i && patn[i] != *s)
			i = table[i-1];

		if (patn[i] == *s)
			++i;

		*t++ = i;
	}
}

char *strkmp_find(const char *text, const char *patn, const size_t *table)
{
	size_t i;
	const char *s;

	assert(text && patn && table);

	if (!*patn)
		return (char*)text;

	for (i = 0, s = text; *s; ++s) {
		while (i && *s != patn[i])
			i = table[i-1];

		if (*s == patn[i]) {
			if (!patn[i+1])
				return (char*)s - i;
			++i;
		}
	}

	return NULL;
}

/* eof */
