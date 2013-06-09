/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013 Dominik Haumann <dhaumann@kde.org>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as published
 * by the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef TIKZ_EXPORT_H
#define TIKZ_EXPORT_H

// visibility
#define HAVE_VISIBILITY

#ifdef HAVE_VISIBILITY
# define PROPERTY_NO_EXPORT __attribute__ ((visibility("hidden")))
# define PROPERTY_EXPORT __attribute__ ((visibility("default")))
# define PROPERTY_IMPORT __attribute__ ((visibility("default")))
#else
# define PROPERTY_NO_EXPORT
# define PROPERTY_EXPORT
# define PROPERTY_IMPORT
#endif

#ifndef TIKZCORE_EXPORT
# if defined(MAKE_TIKZCORE)
/* We are building this library */
#  define TIKZCORE_EXPORT PROPERTY_EXPORT
# else
/* We are using this library */
#  define TIKZCORE_EXPORT PROPERTY_IMPORT
# endif
#endif

#endif // TIKZ_EXPORT_H

// kate: indent-width 4; replace-tabs on;
