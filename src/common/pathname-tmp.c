/*
 * pathname-tmp.c  -- create directory recursively
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

#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <config-os.h>

#include <ycc/common/unistd.h>
#include <ycc/common/debug.h>

char *pathname_tmp(const char *pathname, char *pathname_dest, size_t size)
{
	char *suffix, *name_tmp = pathname_dest;
	const char *p;
	size_t n, offset;
	struct stat sb;

	n = strlen(pathname)+12;
	if (name_tmp && n > size) {
		DBGP("name_tmp but too small (%zu, %zu)", n, size);
		return NULL;
	}

	if (!name_tmp && !(name_tmp = (char*)malloc(n))) {
		DBGE("name_tmp malloc(%zu) failed", n);
		return NULL;
	}

	if ((p = strrchr(pathname, '/'))) {
		++p;
		offset = p - pathname;
		memcpy(name_tmp, pathname, offset);
	} else {
		p = pathname;
		offset = 0;
	}

	offset += sprintf(name_tmp + offset, ".%s.swp", *p ? p : "non");

	if (stat(name_tmp, &sb))
		return name_tmp;

	suffix = name_tmp + offset;
	for (n = 0; n < 1000; ++n) {
		sprintf(suffix, ".%04zu", n);
		if (stat(name_tmp, &sb))
			return name_tmp;
	}

	if (!pathname_dest)
		free(name_tmp);

	return NULL;
}

/* eof */
