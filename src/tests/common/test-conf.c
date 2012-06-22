/*
 * test-conf.c
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

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <config-os.h>

#include <ycc/common/debug.h>
#include <ycc/common/conf.h>

struct conf_item {
	char s[64];
	int i;
	long long ll;
};

const char *file = "/tmp/test-conf.conf";

void test_common(int num)
{
	ssize_t i, rtn;
	char key[8];
	struct conf_item item_w, item_r;

	for (i = 0; i < num; ++i) {
		sprintf(item_w.s, "test-item-%03zd", i);
		item_w.i = (int)i;
		item_w.ll = (long long)i*100;

		sprintf(key, "KEY-%zd", i);

		rtn = conf_write(file, key, "%s %d %lld",
				 item_w.s, item_w.i, item_w.ll);
		if (rtn <= 0) {
			fprintf(stderr, "conf_write failed: %s, %s, %d -- %s\n",
				__FILE__, __func__, __LINE__,
				strerror(errno));
			exit(1);
		}
		rtn = conf_read(file, key, "%s %d %lld",
				item_r.s, &item_r.i, &item_r.ll);
		if (3 != rtn) {
			fprintf(stderr,
				"conf_read failed: %s, %s, %d -- %zd %s\n",
				__FILE__, __func__, __LINE__,
				rtn, strerror(errno));
			exit(1);
		}

		if (strcmp(item_r.s, item_w.s) ||
		    item_r.i != item_w.i ||
		    item_r.ll != item_w.ll) {
			fprintf(stderr, "check failed: %s, %s, %d\n",
				__FILE__, __func__, __LINE__);
			fprintf(stderr,
				"value(r-w): %s-%s, %d-%d, %lld-%lld\n",
				item_r.s, item_w.s,
				item_r.i, item_w.i,
				item_r.ll, item_w.ll);
			exit(1);
		}
	}

}

void test_type(int num)
{
	/* char, short, int, long, long long, size_t, ssize_t, string */
	char ch_r, ch_w;
	short sh_r, sh_w;
	int i_r, i_w;
	long l_r, l_w;
	long long ll_r, ll_w;
	char key[16];

	int idx;
	ssize_t rtn;

	for (idx = 0; idx < num; ++idx) {
#define __GENERATE_KEY(type)	sprintf(key, "KEY-%s", #type)
		__GENERATE_KEY(char);
		ch_w = (char)(idx%255+1);
		while (isspace(ch_w))
			++ch_w;
		rtn = conf_write_char(file, key, ch_w);
		if (rtn <= 0) {
			fprintf(stderr,
				"conf_write_char failed: %s, %s, %d -- %s\n",
				__FILE__, __func__, __LINE__,
				strerror(errno));
			exit(1);
		}
		rtn = conf_read_char(file, key, &ch_r);
		if (1 != rtn) {
			fprintf(stderr, "conf_read failed: %s, %s, %d -- %s\n",
				__FILE__, __func__, __LINE__,
				strerror(errno));
			fprintf(stderr, "rtn = %zd, r-w: %x-%x\n",
				rtn, (int)ch_r, (int)ch_w);
			exit(1);
		}

		if (ch_r != ch_w) {
			fprintf(stderr, "check failed: %s, %s, %d\n",
				__FILE__, __func__, __LINE__);
			fprintf(stderr, "value(r-w): %x-%x\n",
				(int)ch_r, (int)ch_w);
			exit(1);
		}

		/* short */
		__GENERATE_KEY(short);
		sh_w = (short)(idx%65535);
		rtn = conf_write_short(file, key, sh_w);
		if (rtn <= 0) {
			fprintf(stderr,
				"conf_write_short failed: %s, %s, %d -- %s\n",
				__FILE__, __func__, __LINE__,
				strerror(errno));
			exit(1);
		}
		rtn = conf_read_short(file, key, &sh_r);
		if (1 != rtn) {
			fprintf(stderr, "conf_read failed: %s, %s, %d -- %s\n",
				__FILE__, __func__, __LINE__,
				strerror(errno));
			fprintf(stderr, "rtn = %zd, r-w: %x-%x\n",
				rtn, (int)sh_r, (int)sh_w);
			exit(1);
		}

		if (sh_r != sh_w) {
			fprintf(stderr, "check failed: %s, %s, %d\n",
				__FILE__, __func__, __LINE__);
			fprintf(stderr, "value(r-w): %x-%x\n",
				(int)sh_r, (int)sh_w);
			exit(1);
		}

		/* int */
		__GENERATE_KEY(int);
		i_w = (int)(idx*100);
		rtn = conf_write_long(file, key, i_w);
		if (rtn <= 0) {
			fprintf(stderr,
				"conf_write_short failed: %s, %s, %d -- %s\n",
				__FILE__, __func__, __LINE__,
				strerror(errno));
			exit(1);
		}
		rtn = conf_read_int(file, key, &i_r);
		if (1 != rtn) {
			fprintf(stderr, "conf_read failed: %s, %s, %d -- %s\n",
				__FILE__, __func__, __LINE__,
				strerror(errno));
			fprintf(stderr, "rtn = %zd, r-w: %x-%x\n",
				rtn, (int)i_r, (int)i_w);
			exit(1);
		}

		if (i_r != i_w) {
			fprintf(stderr, "check failed: %s, %s, %d\n",
				__FILE__, __func__, __LINE__);
			fprintf(stderr, "value(r-w): %x-%x\n",
				(int)i_r, (int)i_w);
			exit(1);
		}

		/* long */
		__GENERATE_KEY(long);
		l_w = (long)(idx*1000);
		rtn = conf_write_int(file, key, l_w);
		if (rtn <= 0) {
			fprintf(stderr,
				"conf_write_short failed: %s, %s, %d -- %s\n",
				__FILE__, __func__, __LINE__,
				strerror(errno));
			exit(1);
		}
		rtn = conf_read_long(file, key, &l_r);
		if (1 != rtn) {
			fprintf(stderr, "conf_read failed: %s, %s, %d -- %s\n",
				__FILE__, __func__, __LINE__,
				strerror(errno));
			fprintf(stderr, "rtn = %zd, r-w: %lx-%lx\n",
				rtn, l_r, l_w);
			exit(1);
		}

		if (l_r != l_w) {
			fprintf(stderr, "check failed: %s, %s, %d\n",
				__FILE__, __func__, __LINE__);
			fprintf(stderr, "value(r-w): %lx-%lx\n",
				l_r, l_w);
			exit(1);
		}

		/* long long */
		__GENERATE_KEY(longlong);
		ll_w = (long long)(idx*1234);
		rtn = conf_write_longlong(file, key, ll_w);
		if (rtn <= 0) {
			fprintf(stderr,
				"conf_write_short failed: %s, %s, %d -- %s\n",
				__FILE__, __func__, __LINE__,
				strerror(errno));
			exit(1);
		}
		rtn = conf_read_longlong(file, key, &ll_r);
		if (1 != rtn) {
			fprintf(stderr, "conf_read failed: %s, %s, %d -- %s\n",
				__FILE__, __func__, __LINE__,
				strerror(errno));
			fprintf(stderr, "rtn = %zd, r-w: %llx-%llx\n",
				rtn, ll_r, ll_w);
			exit(1);
		}

		if (ll_r != ll_w) {
			fprintf(stderr, "check failed: %s, %s, %d\n",
				__FILE__, __func__, __LINE__);
			fprintf(stderr, "value(r-w): %llx-%llx\n",
				ll_r, ll_w);
			exit(1);
		}
	}
}

#if 1
struct conf_bin {
	int i;
	size_t size;
	char str[16];
	long long ll;
};
const char *file_bin = "/tmp/test-conf-bin.conf";

void test_bin(int num)
{
	ssize_t i, rtn;
	char key[8];
	struct conf_bin item_r, item_w;

	for (i = 0; i < num; ++i) {
		item_w.i = (int)i;
		item_w.size = (size_t)i*1234;
		item_w.ll = (long long)i*3456;
		sprintf(item_w.str, "val: %zd", i);

		sprintf(key, "KEY-%zd", i);

		rtn = conf_write_bin(file_bin, key, strlen(key),
				 &item_w, sizeof(item_w));
		if (rtn < 0) {
			fprintf(stderr,
				"conf_write_bin failed: %s, %s, %d -- %s\n",
				__FILE__, __func__, __LINE__,
				strerror(errno));
			exit(1);
		}
		rtn = conf_read_bin(file_bin, key, strlen(key),
				    &item_r, sizeof(item_r));
		if (-1 == rtn) {
			fprintf(stderr,
				"conf_read failed: %s, %s, %d -- %zd,%zd %s\n",
				__FILE__, __func__, __LINE__,
				rtn, sizeof(item_r), strerror(errno));
			exit(1);
		}

		if (strcmp(item_r.str, item_w.str) ||
		    item_r.i != item_w.i ||
		    item_r.ll != item_w.ll) {
			fprintf(stderr, "check failed: %s, %s, %d\n",
				__FILE__, __func__, __LINE__);
			fprintf(stderr,
				"value(r-w): %s-%s, %d-%d, %lld-%lld\n",
				item_r.str, item_w.str,
				item_r.i, item_w.i,
				item_r.ll, item_w.ll);
			exit(1);
		}
		conf_erase_bin(file_bin, key, strlen(key),
			       sizeof(struct conf_bin));
	}
}
#endif

int main()
{
	test_common(10);
	test_common(100);

	test_type(10);
	test_type(100);

	test_bin(100);
	test_bin(100);

	conf_delete(file);
	conf_delete_bin(file_bin);
	return 0;
}

/* eof */
