/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013-2014 Dominik Haumann <dhaumann@kde.org>
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

#ifndef TIKZ_UI_EXPORT_H
#define TIKZ_UI_EXPORT_H

#include <QtCore/QtGlobal>

#if defined(MAKE_TIKZGUI)
#  define TIKZKITUI_EXPORT Q_DECL_EXPORT
#else
#  define TIKZKITUI_EXPORT Q_DECL_IMPORT
#endif

#endif // TIKZ_UI_EXPORT_H

// kate: indent-width 4; replace-tabs on;
