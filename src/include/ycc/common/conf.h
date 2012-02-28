/*
 * conf.h
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

#ifndef __YCC_COMMON_CONF_H_
#define __YCC_COMMON_CONF_H_

#include <sys/types.h>

#include <ycc/config-ycc.h>

__BEGIN_DECLS

/*
 * return:
 *	n bytes wrote to file for success
 *	-1 if an error encountered
 */
ssize_t conf_read(const char *file,
		  const char *key, const char *value_fmt, ...);

/*
 * return:
 *	n items matched successfully
 *	-1 if an error encountered
 */
ssize_t conf_write(const char *file,
		   const char *key,
		   const char *value_fmt,
		   ...);

ssize_t conf_erase(const char *file, const char *key);
ssize_t conf_delete(const char *file);

/* operate binary data */
ssize_t conf_read_bin(const char *file,
		      const void *key, size_t key_len,
		      void *value, size_t value_len);

ssize_t conf_write_bin(const char *file,
		       const void *key, size_t key_len,
		       const void *value, size_t value_len);

ssize_t conf_erase_bin(const char *file,
		       const void *key, size_t key_len, size_t value_len);

ssize_t conf_delete_bin(const char *file);

ssize_t conf_read_string_safe(const char *file, const char *key,
			      char *value, size_t size);

static inline ssize_t
conf_write_string(const char *file, const char *key, const char *value)
{
	return conf_write(file, key, "%s", value);
}

static inline ssize_t
conf_read_string(const char *file, const char *key, char *value)
{
	return conf_read(file, key, "%s", value);
}

static inline ssize_t
conf_write_char(const char *file, const char *key, char value)
{
	return conf_write(file, key, "%c", value);
}

static inline ssize_t
conf_read_char(const char *file, const char *key, char *value)
{
	return conf_read(file, key, "%c", value);
}

static inline ssize_t
conf_write_uchar(const char *file, const char *key, unsigned char value)
{
	return conf_write(file, key, "%c", value);
}

static inline ssize_t
conf_read_uchar(const char *file, const char *key, unsigned char *value)
{
	return conf_read(file, key, "%c", value);
}

static inline ssize_t
conf_write_short(const char *file, const char *key, short int value)
{
	return conf_write(file, key, "%hd", value);
}

static inline ssize_t
conf_read_short(const char *file, const char *key, short int *value)
{
	return conf_read(file, key, "%hd", value);
}

static inline ssize_t
conf_write_ushort(const char *file, const char *key, unsigned short int value)
{
	return conf_write(file, key, "%hu", value);
}

static inline ssize_t
conf_read_ushort(const char *file, const char *key, unsigned short int *value)
{
	return conf_read(file, key, "%hu", value);
}

static inline ssize_t
conf_write_int(const char *file, const char *key, int value)
{
	return conf_write(file, key, "%d", value);
}

static inline ssize_t
conf_read_int(const char *file, const char *key, int *value)
{
	return conf_read(file, key, "%d", value);
}

static inline ssize_t
conf_write_uint(const char *file, const char *key, unsigned int value)
{
	return conf_write(file, key, "%u", value);
}

static inline ssize_t
conf_read_uint(const char *file, const char *key, unsigned int *value)
{
	return conf_read(file, key, "%u", value);
}

static inline ssize_t
conf_write_long(const char *file, const char *key, long int value)
{
	return conf_write(file, key, "%ld", value);
}

static inline ssize_t
conf_read_long(const char *file, const char *key, long int *value)
{
	return conf_read(file, key, "%ld", value);
}

static inline ssize_t
conf_write_ulong(const char *file, const char *key, unsigned long int value)
{
	return conf_write(file, key, "%lu", value);
}

static inline ssize_t
conf_read_ulong(const char *file, const char *key, unsigned long int *value)
{
	return conf_read(file, key, "%lu", value);
}

static inline ssize_t
conf_write_longlong(const char *file, const char *key, long long int value)
{
	return conf_write(file, key, "%lld", value);
}

static inline ssize_t
conf_read_longlong(const char *file, const char *key, long long int *value)
{
	return conf_read(file, key, "%lld", value);
}

static inline ssize_t
conf_write_ulonglong(const char *file, const char *key,
		     unsigned long long int value)
{
	return conf_write(file, key, "%llu", value);
}

static inline ssize_t
conf_read_ulonglong(const char *file, const char *key,
		    unsigned long long int *value)
{
	return conf_read(file, key, "%llu", value);
}

static inline ssize_t
conf_write_size(const char *file, const char *key, size_t value)
{
	return conf_write(file, key, "%zu", value);
}

static inline ssize_t
conf_read_size(const char *file, const char *key, size_t *value)
{
	return conf_read(file, key, "%zu", value);
}

static inline ssize_t
conf_write_ssize(const char *file, const char *key, ssize_t value)
{
	return conf_write(file, key, "%zd", value);
}

static inline ssize_t
conf_read_ssize(const char *file, const char *key, ssize_t *value)
{
	return conf_read(file, key, "%zd", value);
}
static inline ssize_t
conf_write_int8(const char *file, const char *key, int8_t value)
{
	return conf_write(file, key, "%hhd", value);
}

static inline ssize_t
conf_read_int8(const char *file, const char *key, int8_t *value)
{
	return conf_read(file, key, "%hhd", value);
}

static inline ssize_t
conf_write_int16(const char *file, const char *key, int16_t value)
{
	return conf_write(file, key, "%hd", value);
}

static inline ssize_t
conf_read_int16(const char *file, const char *key, int16_t *value)
{
	return conf_read(file, key, "%hd", value);
}

static inline ssize_t
conf_write_int32(const char *file, const char *key, int32_t value)
{
	return conf_write(file, key, "%d", value);
}

static inline ssize_t
conf_read_int32(const char *file, const char *key, int32_t *value)
{
	return conf_read(file, key, "%d", value);
}

static inline ssize_t
conf_write_int64(const char *file, const char *key, int64_t value)
{
	return conf_write(file, key, "%lld", value);
}

static inline ssize_t
conf_read_int64(const char *file, const char *key, int64_t *value)
{
	return conf_read(file, key, "%lld", value);
}

static inline ssize_t
conf_write_uint8(const char *file, const char *key, u_int8_t value)
{
	return conf_write(file, key, "%hhu", value);
}

static inline ssize_t
conf_read_uint8(const char *file, const char *key, u_int8_t *value)
{
	return conf_read(file, key, "%hhu", value);
}

static inline ssize_t
conf_write_uint16(const char *file, const char *key, u_int16_t value)
{
	return conf_write(file, key, "%hu", value);
}

static inline ssize_t
conf_read_uint16(const char *file, const char *key, u_int16_t *value)
{
	return conf_read(file, key, "%hu", value);
}

static inline ssize_t
conf_write_uint32(const char *file, const char *key, u_int32_t value)
{
	return conf_write(file, key, "%u", value);
}

static inline ssize_t
conf_read_uint32(const char *file, const char *key, u_int32_t *value)
{
	return conf_read(file, key, "%u", value);
}

static inline ssize_t
conf_write_uint64(const char *file, const char *key, u_int64_t value)
{
	return conf_write(file, key, "%llu", value);
}

static inline ssize_t
conf_read_uint64(const char *file, const char *key, u_int64_t *value)
{
	return conf_read(file, key, "%llu", value);
}

__END_DECLS

#endif	/* __YCC_COMMON_CONF_H_ */

/* eof */
