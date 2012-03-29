/*
 * debug.h
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

#ifndef __YCC_COMMON_DEBUG_H_
#define __YCC_COMMON_DEBUG_H_

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
#define DBGP(fmt, ...)		__ASSERT_VOID_CAST(0)
#define DBGE(fmt, ...)		__ASSERT_VOID_CAST(0)
#define DBGVP(fmt, ...)		__ASSERT_VOID_CAST(0)
#define DBGVE(fmt, ...)		__ASSERT_VOID_CAST(0)
#else
#define DBG_INSERT(...)	__VA_ARGS__
#define DBGP(fmt, ...)	__dbg_fprintf(					\
				stderr, __FILE__, __func__, __LINE__,	\
				false, (fmt), ## __VA_ARGS__)
#define DBGVP(fmt, ap)	__dbg_vprintf(					\
				stderr, __FILE__, __func__, __LINE__,	\
				false, (fmt), (ap))
#define DBGE(fmt, ...)	__dbg_fprintf(					\
				stderr, __FILE__, __func__, __LINE__,	\
				true, (fmt), ## __VA_ARGS__)
#define DBGVE(fmt, ap)	__dbg_vprintf(					\
				stderr, __FILE__, __func__, __LINE__,	\
				true, (fmt), (ap))
#endif

void __dbg_vprintf(FILE *stream,
		   const char *file,
		   const char *func,
		   size_t line,
		   bool berr,
		   const char *fmt,
		   va_list ap);

static inline
void __dbg_fprintf(FILE *stream,
		   const char *file,
		   const char *func,
		   size_t line,
		   bool berr,
		   const char *fmt,
		   ...)
{
	va_list ap;

	va_start(ap, fmt);
	__dbg_vprintf(stream, file, func, line, berr, fmt, ap);
	va_end(ap);
}

__END_DECLS

#endif /* __YCC_COMMON_DEBUG_H_ */

/* eof */
