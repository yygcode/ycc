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

#ifndef __YCC_CONFIG_GCC_H_
#define __YCC_CONFIG_GCC_H_

#ifndef __aligned
#define __aligned(x)			__attribute__((aligned(x)))
#endif

#ifndef __always_inline
#define __always_inline		inline __attribute__((always_inline))
#endif

#endif /* __YCC_CONFIG_GCC_H_ */
