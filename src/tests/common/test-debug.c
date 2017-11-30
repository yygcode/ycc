/*
 * test-debug.c
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

#include <errno.h>
#include <stdlib.h>

#ifdef YCC_TEST_NDEBUG
#  ifndef NDEBUG
#  define NDEBUG
#  endif
#else
#  ifdef NDEBUG
#  undef NDEBUG
#  endif
#endif

#include <config-os.h>
#include <ycc/common/debug.h>

void test_debug()
{
	int i, j;

	i = 10;
	j = 20;
	DBG_INSERT(
		++i, ++i;
		j = i;
	);

	if (i != 12 || i !=j) {
		fprintf(stderr, "failed: %s %s %d\n",
			__FILE__, __func__, __LINE__);
		exit(1);
	}

	DBG_PR("debug print\n");
}

void test_ndebug()
{
	int i, j;

	i = 10;
	j = 20;
	DBG_INSERT(
		++i, ++i;
		j = i;
	);

	if (i != 10 || j != 20) {
		fprintf(stderr, "failed(%d, %d): %s %s %d\n",
			i, j, __FILE__, __func__, __LINE__);
		exit(1);
	}

	DBG_PR("debug print\n");
}

int main()
{
#ifdef NDEBUG
	test_ndebug();
#else
	test_debug();
#endif

	return 0;
}

/* eof */
