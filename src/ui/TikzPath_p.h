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

#ifndef GUI_TIKZ_PATH_PRIVATE_ITEM_H
#define GUI_TIKZ_PATH_PRIVATE_ITEM_H

namespace tikz {
namespace core {
    class Path;
}
}

class AbstractTikzPath;

class TikzPathPrivate
{
    public:
        // edge and nodes
        tikz::core::Path * path;

        // backend path
        AbstractTikzPath * backendPath;
};

#endif // GUI_TIKZ_PATH_PRIVATE_ITEM_H

// kate: indent-width 4; replace-tabs on;
