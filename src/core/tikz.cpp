/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013-2015 Dominik Haumann <dhaumann@kde.org>
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

#include "tikz.h"

namespace tikz {

QString toString(EntityType type)
{
    switch (type) {
        case EntityType::Invalid   : return QStringLiteral("Invalid");
        case EntityType::Style     : return QStringLiteral("Style");
        case EntityType::NodeStyle : return QStringLiteral("NodeStyle");
        case EntityType::EdgeStyle : return QStringLiteral("EdgeStyle");
        case EntityType::Node      : return QStringLiteral("Node");
        case EntityType::Path      : return QStringLiteral("Path");
        default: break;
    }
    return QStringLiteral("Invalid");
}

EntityType toEntityType(const QString & str)
{
    if (str == QStringLiteral("Invalid")) {
        return EntityType::Invalid;
    } else if (str == QStringLiteral("Style")) {
        return EntityType::Style;
    } else if (str == QStringLiteral("NodeStyle")) {
        return EntityType::NodeStyle;
    } else if (str == QStringLiteral("EdgeStyle")) {
        return EntityType::EdgeStyle;
    } else if (str == QStringLiteral("Node")) {
        return EntityType::Node;
    } else if (str == QStringLiteral("Path")) {
        return EntityType::Path;
    }

    return EntityType::Invalid;
}

} // namespace tikz

// kate: indent-width 4; replace-tabs on;
