/*
 * conf-bin.c -- binary config operation
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
#include <endian.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include <config-os.h>

#include <ycc/common/unistd.h>
#include <ycc/common/debug.h>
#include <ycc/common/conf.h>

/*
 * config-head
 *	<MAGIC><VERSION>
 * config-item
 *
 *    format:
 *	<MAGIC><KEY-SIZE><VALUE-SIZE><0><KEY><VALUE>
 *
 * constraint:
 *	no constraint
 */

#define MAGIC_HEAD	0x51525354
#define MAGIC_ITEM	0xA6A7A8A9
#define CONF_VERSION	0x00000001

struct bin_head {
	uint32_t magic;
	uint32_t version;
};

struct bin_item {
	/* saved as little endian */
	uint32_t magic;
	uint32_t key_len;
	uint32_t value_len;
	uint32_t place_holder;
};

static ssize_t __check_head(int fd)
{
	uint32_t magic, version;
	ssize_t r;
	struct bin_head bh;

	if ((r = readn(fd, &bh, sizeof(bh))) != sizeof(bh)) {
		if (r == 0)
			return 0;	/* empty file ? */
		DBG_PE("readn bin_head %zd failed", r);
		return -1;
	}

	magic = le32toh(bh.magic);
	version = le32toh(bh.version);

	if (magic != MAGIC_HEAD || version < CONF_VERSION) {
		DBG_PR("magic(%x) or version(%x) error", magic, version);
		return -1;
	}

	return 0;
}

static inline ssize_t __write_head(int fd)
{
	struct bin_head bh = {
		.magic = htole32(MAGIC_HEAD),
		.version = htole32(CONF_VERSION),
	};

	if (writen(fd, &bh, sizeof(bh)) != sizeof(bh)) {
		DBG_PE("writen bin_head failed");
		return -1;
	}

	return 0;
}

static inline ssize_t __check_write(int fd, const void *buf, size_t count)
{
	if(fd >= 0 && writen(fd, buf, count) != count)
		return -1;

	return 0;
}

static ssize_t __check_copy(int fd, int fd_new, size_t count)
{
	char buf[256];
	size_t n, offset = 0;

	while (offset < count) {
		if ((n = count - offset) > 256)
			n = 256;

		if (readn(fd, buf, n) != n) {
			DBG_PE("read(%zu %zu %zu) from %d failed",
				   count, offset, n, fd);
			return -1;
		}

		if (fd_new >= 0 && write(fd_new, buf, n) != n) {
			DBG_PE("write buf %zu failed", n);
			return -1;
		}

		offset += n;
	}

	return 0;
}

static ssize_t __cmp_key(int fd, const void *key, size_t key_len)
{
	char buf[256];
	size_t n, offset = 0;

	while (offset < key_len) {
		if ((n = key_len - offset) > 256)
			n = 256;

		if (readn(fd, buf, n) != n) {
			DBG_PE("read key(offset = %zu, n = %zu) failed",
				   offset, n);
			return -2;
		}

		if (memcmp(buf, key + offset, n)) {
			offset += n;
			if (lseek(fd, -(ssize_t)offset, SEEK_CUR) < 0) {
				DBG_PE("lseek %zd failed",
					   -(ssize_t)offset);
				return -2;
			}
			return -1;
		}

		offset += n;
	}

	return 0;
}

static ssize_t __find_key(int fd, int fd_new, const void *key, size_t key_len,
			  size_t value_len)
{
	uint32_t magic, klen, vlen;
	ssize_t r;
	struct bin_item bi;

	while ((r = readn(fd, &bi, sizeof(bi))) == sizeof(bi)) {
		magic = le32toh(bi.magic);
		klen = le32toh(bi.key_len);
		vlen = le32toh(bi.value_len);

		if (magic != MAGIC_ITEM) {
			DBG_PR("magic(%x) dismatch, check format", magic);
			return -2;
		}

		if (klen != key_len || vlen != value_len ||
		    (r = __cmp_key(fd, key, key_len))) {
			if (r == -2)	/* lseek failed */
				return r;

			if (__check_write(fd_new, &bi, sizeof(bi)) < 0 ||
			    __check_copy(fd, fd_new, klen + vlen) < 0) {
				DBG_PE("__check_write/__check_copy failed");
				return -2;
			}
			continue;
		}

		return 0;	/* match success */
	}

	return -1;	/* item non-exists */
}

static inline ssize_t __read_value(int fd, void *value, size_t value_len)
{
	if (readn(fd, value, value_len) != value_len)
		return -1;

	return 0;
}

static ssize_t __write_item(int fd,
			    const void *key, size_t key_len,
			    const void *value, size_t value_len)
{
	struct bin_item bi = {
		.magic = htole32(MAGIC_ITEM),
		.key_len = htole32(key_len),
		.value_len = htole32(value_len),
		.place_holder = 0,
	};

	if (writen(fd, &bi, sizeof(bi)) != sizeof(bi))
		return -1;

	if (writen(fd, key, key_len) != key_len)
		return -1;

	if (writen(fd, value, value_len) != value_len)
		return -1;

	return 0;
}

#if 0
static ssize_t __copy_left(int fd, int fd_new)
{
	char buf[256];
	size_t n;

	while ((n = readn(fd, buf, 256)) > 0) {
		if (write(fd_new, buf, n) != n) {
			DBG_PE("write buf %zu failed", n);
			return -1;
		}
	}

	return 0;
}
#endif

ssize_t conf_read_bin(const char *file,
		      const void *key, size_t key_len,
		      void *value, size_t value_len)
{
	int fd;
	ssize_t r = -1;
	
	if ((fd = open(file, O_RDONLY)) < 0) {
		DBG_PE("open %s failed", file);
		return -1;
	}

	if (__check_head(fd) < 0)
		goto RTN;

	if (__find_key(fd, -1, key, key_len, value_len) < 0)
		goto RTN;

	if (readn(fd, value, value_len) != value_len)
		goto RTN;

	r = 0;

RTN:
	close(fd);
	return r;
}

#define CONF_WRITE	1
#define CONF_ERASE	2
ssize_t conf_op_bin(int type, const char *file,
		       const void *key, size_t key_len,
		       const void *value, size_t value_len)
{
	int fd = -1, fd_new;
	char file_tmp[PATH_MAX];
	ssize_t r = -1;

	if (pathname_tmp(file, file_tmp, PATH_MAX) < 0)
		return -1;

	if ((fd_new = open(file_tmp, O_WRONLY|O_CREAT, ACCESSPERMS)) < 0) {
		DBG_PE("open %s failed", file_tmp);
		return -1;
	}
	if ((r = __write_head(fd_new)) < 0)
		goto RTN;

	if ((fd = open(file, O_RDONLY)) < 0) {
		if (ENOENT == errno)
			goto NEW_ITEM;

		goto RTN;
	}

	if (__check_head(fd) < 0)
		goto RTN;

	while(!(r = __find_key(fd, fd_new, key, key_len, value_len)))
		__check_copy(fd, -1, value_len); /* discard value here */

	if (r == -2)
		goto RTN;

NEW_ITEM:
	if (CONF_WRITE == type)
		r = __write_item(fd_new, key, key_len, value, value_len);

	/* r = __copy_left(fd, fd_new); */

RTN:
	close(fd_new);
	if (fd >= 0)
		close(fd);
	if (r >= 0)
		r = rename(file_tmp, file);
	if (r < 0)
		unlink(file_tmp);

	return r;
}

ssize_t conf_write_bin(const char *file,
		       const void *key, size_t key_len,
		       const void *value, size_t value_len)
{
	return conf_op_bin(CONF_WRITE, file, key, key_len, value, value_len);
}

ssize_t conf_erase_bin(const char *file,
			const void *key, size_t key_len,
			size_t value_len)
{
	return conf_op_bin(CONF_ERASE, file, key, key_len, NULL, value_len);
}

ssize_t conf_delete_bin(const char *file)
{
	return unlink(file);
}

/* eof */
