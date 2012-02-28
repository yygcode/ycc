/*
 * test-unistd.c
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

#include <config-os.h>

#include <ycc/common/unistd.h>
#include <ycc/common/log.h>

int main()
{
	const char *pathname = "/tmp/xxx";
	char pathname_dest[PATH_MAX], *p;

	if (!pathname_tmp(pathname, pathname_dest, PATH_MAX)) {
		OUT_ERR("pathname_tmp %s failed", pathname);
		return 1;
	}
	OUT_DEBUG("pathname_tmp: %s, %s", pathname, pathname_dest);

	pathname = "/";
	if (!pathname_tmp(pathname, pathname_dest, PATH_MAX)) {
		OUT_ERR("pathname_tmp %s failed", pathname);
		return 1;
	}
	OUT_DEBUG("pathname_tmp: %s, %s", pathname, pathname_dest);

	pathname = "/etc/init.d/";
	if (!(p = pathname_tmp(pathname, NULL, -1))) {
		OUT_ERR("pathname_tmp %s failed", pathname);
		return 1;
	}
	OUT_DEBUG("pathname_tmp: %s, %s", pathname, p);

	return 0;
}

/* eof */
