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
#ifndef TIKZ_VALUE_H
#define TIKZ_VALUE_H

#include "tikz_export.h"
#include "tikz.h"

namespace tikz
{

namespace internal {
    static qreal units[5][5] = {
        // invalid, pt, mm      , cm     , in
        // invalid
        {0.0,  0.0    ,  0.0    , 0.0    , 0.0},
        // pt
        {0.0,  1.0    ,  0.35146, 0.03515, 72.0},
        // mm
        {0.0,  2.84526,  1.0    , 0.1    , 0.03937},
        // cm
        {0.0, 28.45274, 10.0    , 1.0    , 0.3937},
        // in
        {0.0, 72.26999, 25.40013, 2.54   , 1.0}
    };
}

/**
 * Units.
 */
enum Unit {
    Invalid = 0,// invalid unit
    Point,      // =  1.00pt = 0.3515mm
    Millimeter, // =  2.84pt = 0.99826mm
    Centimeter, // = 28.40pt = 9.9826mm
    Inch        // = 72.27pt = 25.402905mm
};

class TIKZCORE_EXPORT Value
{
    public:
        /**
         * Default Constructor, creates an instance of Unit::Invalid.
         */
        Value()
            : m_value(0.0)
            , m_unit(Invalid)
        {
        }

        /**
         * Constructor with value and type.
         */
        Value(qreal value, Unit unit)
            : m_value(value)
            , m_unit(unit)
        {
        }

        inline bool isValid() const
        {
            return m_unit != Invalid;
        }

        /**
         * Get the Unit.
         */
        inline Unit unit() const
        {
            return m_unit;
        }

        /**
         * Explicit conversion to value.
         */
        inline qreal value() const
        {
            return m_value;
        }

        /**
         *
         */
        Value convertTo(Unit unit) const
        {
            const qreal s = internal::units[m_unit][unit];
            Value val(m_value * s, unit);
            return val;
        }

    //
    // operators for equality
    //
    public:
        /**
         * Check for equality with (val1 == val2).
         * The value @p other is converted to a compatible unit, meaning that
         * the unit of @p other and this object are allowed to differ.
         */
        inline bool operator==(const Value & other) const
        {
            return m_value == other.convertTo(m_unit);
        }

        /**
         * Check for inequality with (val1 != val2).
         */
        inline bool operator!=(const Value & other) const
        {
            return !(operator==(other));
        }

    //
    // operators for qreal values
    //
    public:
        /**
         * Implicit cast to value.
         */
        inline operator qreal () const
        {
            return m_value;
        }

        /**
         * += operator.
         * Returns a reference to this object increased by @p value.
         * This operation is independent of the unit.
         */
        inline Value & operator+=(qreal value)
        {
            Q_ASSERT(isValid());
            m_value += value;
            return *this;
        }

        /**
         * -= operator.
         * Returns a reference to this object increased by @p value.
         * This operation is independent of the unit.
         */
        inline Value & operator-=(qreal value)
        {
            Q_ASSERT(isValid());
            m_value -= value;
            return *this;
        }

    //
    // operators for two Value
    //
    public:
        /**
         * Plus operator for two Value%s.
         * Returns a new Value object with the same unit increased by @p value.
         * If @p value's unit is different, @p value is first converted to a
         * compatible Unit before adding the value.
         */
        inline Value operator+(const Value & value)
        {
            Q_ASSERT(isValid());
            Q_ASSERT(value.isValid());
            return Value(m_value + value.convertTo(m_unit).value(), m_unit);
        }

        /**
         * Minus operator for two Value%s.
         * Returns a new Value object with the same unit decreased by @p value.
         * If @p value's unit is different, @p value is first converted to a
         * compatible Unit before adding the value.
         */
        inline Value operator-(const Value & value)
        {
            Q_ASSERT(isValid());
            Q_ASSERT(value.isValid());
            return Value(m_value - value.convertTo(m_unit).value(), m_unit);
        }

        /**
         * += operator for two Value%s.
         * Returns a reference to this object increased by @p value.
         * If @p value's unit is different, @p value is first converted to a
         * compatible Unit before adding the value.
         */
        inline Value & operator+=(const Value & value)
        {
            Q_ASSERT(isValid());
            Q_ASSERT(value.isValid());
            return operator+=(value.convertTo(m_unit).value());
        }

        /**
         * -= operator for two Value%s.
         * Returns a reference to this object decreased by @p value.
         * If @p value's unit is different, @p value is first converted to a
         * compatible Unit before adding the value.
         */
        inline Value & operator-=(const Value & value)
        {
            Q_ASSERT(isValid());
            Q_ASSERT(value.isValid());
            return operator-=(value.convertTo(m_unit).value());
        }

        /**
         * > operator for two Value%s.
         * Returns true if this object's value is greater than @p value.
         * If @p value's unit is different, @p value is first converted to a
         * compatible Unit before adding the value.
         */
        inline bool operator>(const Value & value)
        {
            Q_ASSERT(isValid());
            Q_ASSERT(value.isValid());
            return m_value > value.convertTo(m_unit).value();
        }

        /**
         * >= operator for two Value%s.
         * Returns true if this object's value is greater than or equal to @p value.
         * If @p value's unit is different, @p value is first converted to a
         * compatible Unit before adding the value.
         */
        inline bool operator>=(const Value & value)
        {
            Q_ASSERT(isValid());
            Q_ASSERT(value.isValid());
            return m_value >= value.convertTo(m_unit).value();
        }

        /**
         * < operator for two Value%s.
         * Returns true if this object's value is smaller than @p value.
         * If @p value's unit is different, @p value is first converted to a
         * compatible Unit before adding the value.
         */
        inline bool operator<(const Value & value)
        {
            Q_ASSERT(isValid());
            Q_ASSERT(value.isValid());
            return m_value < value.convertTo(m_unit).value();
        }

        /**
         * <= operator for two Value%s.
         * Returns true if this object's value is smaller than or equal to @p value.
         * If @p value's unit is different, @p value is first converted to a
         * compatible Unit before adding the value.
         */
        inline bool operator<=(const Value & value)
        {
            Q_ASSERT(isValid());
            Q_ASSERT(value.isValid());
            return m_value <= value.convertTo(m_unit).value();
        }

    private:
        /**
         * The value.
         */
        qreal m_value;

        /**
         * The unit of this value.
         */
        Unit m_unit;
};

inline qreal convertTo(qreal value, Unit from, Unit to)
{
    const qreal s = internal::units[from][to];
    return value * s;
}

//
// pt to *
//
inline qreal pt2mm(qreal value)
{
    const qreal s = internal::units[Unit::Point][Unit::Millimeter];
    return value * s;
}

inline qreal pt2cm(qreal value)
{
    const qreal s = internal::units[Unit::Point][Unit::Centimeter];
    return value * s;
}

inline qreal pt2in(qreal value)
{
    const qreal s = internal::units[Unit::Point][Unit::Inch];
    return value * s;
}

//
// cm to *
//
inline qreal cm2pt(qreal value)
{
    const qreal s = internal::units[Unit::Centimeter][Unit::Point];
    return value * s;
}

inline qreal cm2mm(qreal value)
{
    const qreal s = internal::units[Unit::Centimeter][Unit::Millimeter];
    return value * s;
}

inline qreal cm2in(qreal value)
{
    const qreal s = internal::units[Unit::Centimeter][Unit::Inch];
    return value * s;
}

//
// mm to *
//
inline qreal mm2pt(qreal value)
{
    const qreal s = internal::units[Unit::Millimeter][Unit::Point];
    return value * s;
}

inline qreal mm2cm(qreal value)
{
    const qreal s = internal::units[Unit::Millimeter][Unit::Centimeter];
    return value * s;
}

inline qreal mm2in(qreal value)
{
    const qreal s = internal::units[Unit::Millimeter][Unit::Inch];
    return value * s;
}

//
// in to *
//
inline qreal in2pt(qreal value)
{
    const qreal s = internal::units[Unit::Inch][Unit::Point];
    return value * s;
}

inline qreal in2cm(qreal value)
{
    const qreal s = internal::units[Unit::Inch][Unit::Centimeter];
    return value * s;
}

inline qreal in2mm(qreal value)
{
    const qreal s = internal::units[Unit::Inch][Unit::Millimeter];
    return value * s;
}

}

/**
 * Declare as movable, since the members of Value are primitive types.
 */
Q_DECLARE_TYPEINFO(tikz::Value, Q_MOVABLE_TYPE);

#endif // TIKZ_VALUE_H

// kate: indent-width 4; replace-tabs on;
