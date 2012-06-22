/*
 * config-gcc.h: macro of GNU GCC compiler options
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

#ifndef __YCC_COMPILER_GCC_H_
#define __YCC_COMPILER_GCC_H_

#define __aligned(x)		__attribute__((__aligned__(x)))

#ifndef __always_inline
#define __always_inline		inline __attribute__((__always_inline__))
#endif

#define __printf(a, b)		__attribute__((__format__(printf, a, b)))
#define __scanf(a, b)		__attribute__((__format__(scanf, a, b)))

#endif
