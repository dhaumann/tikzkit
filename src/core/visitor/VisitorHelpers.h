/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2014 Dominik Haumann <dhaumann@kde.org>
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

#ifndef TIKZ_VISITOR_HELPERS_H
#define TIKZ_VISITOR_HELPERS_H

#include "tikz.h"
#include <QString>

namespace tikz {
namespace core {
namespace internal {

/**
 * Convert the enum @p anchor to a QString. If @p withDot is @e true,
 * the anchor has a leading '.'.
 * Returns e.g. "north" or ".north" for a north anchor.
 */
extern QString anchorToString(tikz::Anchor anchor, bool withDot = false);

/**
 * Converts @p anchor to tikz::Anchor.
 */
extern tikz::Anchor anchorFromString(const QString anchor);

/**
 * Convert @p penStyle to a QString.
 * Examples are e.g. "solid", "densely dashed", ...
 */
extern QString penStyleToString(tikz::PenStyle penStyle);

/**
 * Convert @p penStyle to a QString.
 * Examples are e.g. "solid", "densely dashed", ...
 */
extern tikz::PenStyle penStyleFromString(const QString & penStyle);

/**
 * Convert @p arrow to a QString.
 */
extern QString arrowToString(tikz::Arrow arrow);

/**
 * Conver the string @p arrow to an enum tikz::Arrow.
 */
extern tikz::Arrow arrowFromString(const QString & arrow);

/**
 * Convert the tikz::TextAlignment @p alignment to a QString.
 */
extern QString textAlignmentToString(tikz::TextAlignment alignment);

/**
 * Conver the string @p alignment to an enum tikz::TextAlignment.
 */
extern tikz::TextAlignment textAlignmentFromString(const QString & alignment);

/**
 * Convert the tikz::Shape @p shape to a QString.
 */
extern QString shapeToString(tikz::Shape shape);

/**
 * Conver the string @p shape to an enum tikz::Shape.
 */
extern tikz::Shape shapeFromString(const QString & shape);
};

}
}

#endif // TIKZ_VISITOR_HELPERS_H

// kate: indent-width 4; replace-tabs on;
