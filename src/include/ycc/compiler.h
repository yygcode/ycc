/*
 * compiler.h: compiler options
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

#ifndef __YCC_COMPILER_H_
#define __YCC_COMPILER_H_	1

#ifdef __GNUC__
#include <ycc/compiler-gcc.h>
#endif

#ifndef __aligned
#define __aligned(x)
#endif

#ifndef __always_inline
#define __always_inline		static
#endif

#ifndef __printf
#define __printf(a, b)
#endif

#ifndef __scanf
#define __scanf(a, b)
#endif

#endif
