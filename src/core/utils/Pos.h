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
#ifndef TIKZ_POS_H
#define TIKZ_POS_H

#include "Value.h"
#include <QPointF>

namespace tikz
{

class TIKZCORE_EXPORT Pos
{
    public:
        /**
         * Default constructor, setting the position to 0, 0 in unit pt.
         */
        constexpr Pos()
            : m_x()
            , m_y()
        {
        }

        /**
         * Constructor setting the position to @p x and @p y in @p unit..
         */
        constexpr Pos(const Value & x, const Value & y)
            : m_x(x)
            , m_y(y)
        {
        }

        /**
         * Constructor setting the position to @p x and @p y in @p unit..
         */
        constexpr Pos(qreal x, qreal y, Unit unit = Unit::Point)
            : m_x(x, unit)
            , m_y(y, unit)
        {
        }

        /**
         * Construct a Pos object from the QPointF object @p point.
         * The size is interpreted according to @p unit.
         */
        constexpr Pos(const QPointF & point, Unit unit = Unit::Point)
            : m_x(point.x(), unit)
            , m_y(point.y(), unit)
        {
        }

        /**
         * Check whether this value is a finite value, i.e. other than NaN or infinity.
         */
        inline constexpr bool isValid() const noexcept
        {
            return m_x.isValid() && m_y.isValid();
        }

        /**
         * Return the x coordinate of this Pos.
         */
        inline constexpr tikz::Value x() const noexcept
        {
            return m_x;
        }

        /**
         * Return a reference to the x coordinate of this Pos.
         */
        inline tikz::Value & rx() noexcept
        {
            return m_x;
        }

        /**
         * Return the y coordinate of this Pos.
         */
        inline constexpr tikz::Value y() const noexcept
        {
            return m_y;
        }

        /**
         * Return a reference to the y coordinate of this Pos.
         */
        inline tikz::Value & ry() noexcept
        {
            return m_y;
        }

        /**
         * Return a Pos where both x and y coordinates are in @p unit.
         */
        inline constexpr Pos convertTo(Unit unit) const noexcept
        {
            return Pos(m_x.convertTo(unit), m_y.convertTo(unit));
        }

        /**
         * Convert this Pos to a string in the form "(x, y)".
         */
        QString toString() const;

        /**
         * Convert @p str of the form "(x, y)" to a Pos.
         */
        static Pos fromString(const QString & str);

    //
    // operators
    //
    public:
        /**
         * Implicit cast to QPointF.
         * @warning the returned value \e always is of type Unit::Point!
         */
        inline constexpr operator QPointF () const noexcept
        {
            return QPointF(m_x.toPoint(),
                           m_y.toPoint());
        }

        /**
         * Adds @p pos to this Pos and returns a reference to this Pos.
         */
        inline Pos & operator+=(const Pos & pos) noexcept
        {
            Q_ASSERT(isValid());
            Q_ASSERT(pos.isValid());
            m_x += pos.m_x;
            m_y += pos.m_y;
            return *this;
        }

        /**
         * Subtracts @p pos to this Pos and returns a reference to this Pos.
         */
        inline Pos & operator-=(const Pos & pos) noexcept
        {
            Q_ASSERT(isValid());
            Q_ASSERT(pos.isValid());
            m_x -= pos.m_x;
            m_y -= pos.m_y;
            return *this;
        }

        /**
         * Multiplies this Pos with @p factor and returns a reference to this Pos.
         */
        inline Pos & operator*=(qreal factor) noexcept
        {
            Q_ASSERT(isValid());
            m_x *= factor;
            m_y *= factor;
            return *this;
        }

        /**
         * Divides this Pos by @p divisor and returns a reference to this Pos.
         */
        inline Pos & operator/=(qreal divisor) noexcept
        {
            Q_ASSERT(isValid());
            m_x /= divisor;
            m_y /= divisor;
            return *this;
        }

    public:
        /**
         * QDebug support.
         */
        inline friend QDebug operator<<(QDebug s, const tikz::Pos & pos)
        {
            s.nospace() << pos.toString();
            return s.space();
        }

    private:
        // the x and y coordinate of this Pos
        Value m_x;
        Value m_y;
};

//
// operators
//

/**
 * Check for equality with (p1 == p2).
 */
inline constexpr bool operator==(const Pos & p1, const Pos & p2) noexcept
{
    return (p1.x() == p2.x()) && (p1.y() == p2.y());
}

/**
 * Check for inequality with (val1 != val2).
 */
inline constexpr bool operator!=(const Pos & p1, const Pos & p2) noexcept
{
    return ! (p1 == p2);
}

/**
 * Returns a Pos that is the sum of @p p1 and @p p2.
 */
inline Pos operator+(const Pos & p1, const Pos & p2) noexcept
{
    Q_ASSERT(p1.isValid());
    Q_ASSERT(p2.isValid());
    return Pos(p1.x() + p2.x(), p1.y() + p2.y());
}

/**
 * Returns a Pos that is the difference of @p p1 and @p p2.
 */
inline Pos operator-(const Pos & p1, const Pos & p2) noexcept
{
    Q_ASSERT(p1.isValid());
    Q_ASSERT(p2.isValid());
    return Pos(p1.x() - p2.x(), p1.y() - p2.y());
}

/**
 * Returns a copy of @p pos, multiplied by @p factor.
 */
inline constexpr Pos operator*(const Pos & pos, qreal factor) noexcept
{
    return Pos(pos.x() * factor, pos.y() * factor);
}

/**
 * Returns a copy of @p pos, multiplied by @p factor.
 */
inline constexpr Pos operator*(qreal factor, const Pos & pos) noexcept
{
    return Pos(pos.x() * factor, pos.y() * factor);
}

/**
 * Returns a copy of @p pos, divided by @p factor.
 */
inline Pos operator/(const Pos & pos, qreal divisor)
{
    Q_ASSERT(divisor != 0);
    return Pos(pos.x() / divisor, pos.y() / divisor);
}

/**
 * Returns a copy of @p pos, divided by @p factor.
 */
inline Pos operator/(qreal divisor, const Pos & pos)
{
    Q_ASSERT(divisor != 0);
    return Pos(pos.x() / divisor, pos.y() / divisor);
}

}

namespace QTest
{
    // forward declaration of template in qtestcase.h
    template<typename T> char* toString(const T&);
    
    template<>
    TIKZCORE_EXPORT char *toString(const tikz::Pos & pos);
}

/**
 * Declare as movable, since the members of Pos are primitive types.
 */
Q_DECLARE_TYPEINFO(tikz::Pos, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(tikz::Pos)

#endif // TIKZ_POS_H

// kate: indent-width 4; replace-tabs on;
