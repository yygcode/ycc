/*
 * string.h -- Exact String Matching
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

#ifndef __YCC_ALGOS_STRING_H_
#define __YCC_ALGOS_STRING_H_

#include <stddef.h>

#include <ycc/config-ycc.h>

__BEGIN_DECLS

/* table size: strlen(patn) */
void strkmp_init(const char *patn, size_t *table);
char *strkmp_find(const char *text, const char *patn, const size_t *table);

/* table size: UCHAR_MAX+1 (256) */
void strbmh_init(const char *needle, size_t n, size_t *table);
char *strbmh_find(const char *haystack, const char *needle,
		  size_t h, size_t n,
		  const size_t *table);

/*
 * table_sgs size: strlen(needle)*2
 * table_ebc size: UCHAR_MAX+1 (256)
 */
void strbm_init(const char *needle, size_t n,
		size_t *table_sgs, size_t *table_ebc);
char *strbm_find(const char *haystack, const char *needle,
		 size_t h, size_t n,
		 const size_t *table_sgs, const size_t *table_ebc);

__END_DECLS

#endif /* __YCC_ALGOS_STRING_H_ */
