/*
 * debug.h - I/O routines for debugging
 *
 * Copyright (C) 2012-2013 yanyg (yygcode@gmail.com, cppgp@qq.com)
 *
 * Mon Jun 11, 2012
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

#ifndef __YCC_COMMON_DEBUG_H_
#define __YCC_COMMON_DEBUG_H_	1

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#include <ycc/config-ycc.h>

__BEGIN_DECLS

void set_dbgstamp(bool stamp);
bool get_dbgstamp();

#ifdef NDEBUG
/* NDEBUG: turn off */
#define DBG_INSERT(...)		__ASSERT_VOID_CAST(0)
#define DBG_PRINTF(fmt, ...)	__ASSERT_VOID_CAST(0)
#define DBG_PERROR(fmt, ...)	__ASSERT_VOID_CAST(0)
#define DBG_VPRINTF(fmt, ap)	__ASSERT_VOID_CAST(0)
#define DBG_VPERROR(fmt, ap)	__ASSERT_VOID_CAST(0)
#else
#define DBG_INSERT(...)		__VA_ARGS__
#define DBG_PRINTF(fmt, ...)	__dfprintf(					\
					stdout, __FILE__, __func__, __LINE__,	\
					(fmt), ## __VA_ARGS__)
#define DBG_VPRINTF(fmt, ap)	__dvfprintf(					\
					stdout, __FILE__, __func__, __LINE__,	\
					(fmt), (ap))
#define DBG_PERROR(fmt, ...)	__dfprintf(					\
					stderr, __FILE__, __func__, __LINE__,	\
					(fmt), ## __VA_ARGS__)
#define DBG_VPERROR(fmt, ap)	__dvfprintf(					\
					stderr, __FILE__, __func__, __LINE__,	\
					(fmt), (ap))

#endif

#define OUT_INSERT(...)		__VA_ARGS__

#define OUT_PRINTF(fmt, ...)	__dfprintf(					\
					stdout, __FILE__, __func__, __LINE__,	\
					(fmt), ## __VA_ARGS__)
#define OUT_VPRINTF(fmt, ap)	__dvfprintf(					\
					stdout, __FILE__, __func__, __LINE__,	\
					(fmt), (ap))
#define OUT_PERROR(fmt, ...)	__dfprintf(					\
					stderr, __FILE__, __func__, __LINE__,	\
					(fmt), ## __VA_ARGS__)
#define OUT_VPERROR(fmt, ap)	__dvfprintf(					\
					stderr, __FILE__, __func__, __LINE__,	\
					(fmt), (ap))
void __dvfprintf(FILE *stream,
	      const char *file,
	      const char *func,
	      size_t line,
	      const char *fmt,
	      va_list ap);
static inline void __dfprintf(FILE *stream,
			      const char *file,
			      const char *func,
			      size_t line,
			      const char *fmt,
			      ...)
{
	va_list ap;

	va_start(ap, fmt);
	__dvfprintf(stream, file, func, line, fmt, ap);
	va_end(ap);
}

__END_DECLS

#endif /* __YCC_COMMON_DEBUG_H_ */
