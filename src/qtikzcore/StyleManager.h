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

#ifndef TIKZ_STYLEMANAGER_H
#define TIKZ_STYLEMANAGER_H

#include <QVector>

namespace tikz {

class Style;

class StyleManager
{
    private:
        static StyleManager* s_self;

    public:
        ~StyleManager();

        static StyleManager* self();

        Style* documentStyle();

        void registerStyle(Style* style);
        void unregisterStyle(Style* style);

    //
    // private constructor, as it is a singleton
    //
    private:
        /**
         * Constructor. Called in ::self() accessor.
         */
        StyleManager();

    private:
        Style* m_documentStyle;
        QVector<Style*> m_styles;
};

}

#endif // TIKZ_STYLEMANAGER_H

// kate: indent-width 4; replace-tabs on;
