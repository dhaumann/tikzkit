/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2015 Dominik Haumann <dhaumann@kde.org>
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
#ifndef TIKZ_CORE_UID_H
#define TIKZ_CORE_UID_H

#include "tikz.h"
#include "tikz_export.h"

#include <QVariant>
#include <QDebug>

namespace tikz {

/**
 * The Uid class provides document-wide unique identifiers.
 *
 * The Uid is defined by an integer number and an EntityType.
 */
class TIKZCORE_EXPORT Uid
{
    public:
        /**
         * Default constructor. Creates an invalid Uid.
         */
        explicit constexpr Uid()
            : m_id(-1)
            , m_type(EntityType::Invalid)
        {
        }

        /**
         * Constructor with value and type.
         */
        explicit constexpr Uid(qint64 id, EntityType type)
            : m_id(id)
            , m_type(type)
        {
        }

        /**
         * Check whether this value is a finite value, i.e. other than NaN or infinity.
         */
        inline constexpr bool isValid() const noexcept
        {
            return m_id >= 0 && m_type != tikz::EntityType::Invalid;
        }

        /**
         * Get the Unit.
         */
        inline constexpr qint64 id() const noexcept
        {
            return m_id;
        }

        /**
         * Explicit conversion to value.
         */
        inline constexpr EntityType type() const noexcept
        {
            return m_type;
        }

        /**
         * Convert this Uid to a string of the form "id (type)"
         */
        QString toString() const;

        /**
         * Convert @p str defined by "id (type)" to a Uid.
         */
        static Uid fromString(const QString & str);

    //
    // operators
    //
    public:
        /**
         * Implicit conversion to qint64.
         */
        inline operator qint64() const
        {
            return m_id;
        }

        /**
         * QDebug support.
         */
        inline friend QDebug operator<<(QDebug s, const tikz::Uid& uid)
        {
            if (&uid != nullptr) {
                s.nospace() << uid.toString();
            } else {
                s.nospace() << "(null Uid)";
            }
            return s.space();
        }

        operator QVariant() const
        {
            return QVariant::fromValue(*this);
        }

    private:
        /**
         * The value.
         */
        qint64 m_id = -1;

        /**
         * The unit of this value.
         */
        EntityType m_type = tikz::EntityType::Invalid;
};

/**
 * Equality operator.
 * Return @e true, if the Uid @p lhs equals the Uid @p rhs.
 * @note The comparison is type-aware.
 */
inline constexpr bool operator==(const Uid & lhs, const Uid & rhs) noexcept
{
    return lhs.isValid()
        && rhs.isValid()
        && (lhs.id() == rhs.id())
        && (lhs.type() == rhs.type());
}

/**
 * Inequality operator.
 * Return @e true, if the Uid @p lhs does not equal the Uid @p rhs.
 * @note The comparison is unit-aware.
 */
inline constexpr bool operator!=(const Uid & lhs, const Uid & rhs) noexcept
{
    return ! (lhs == rhs);
}

} //namespace tikz

namespace QTest
{
    // forward declaration of template in qtestcase.h
    template<typename T> char* toString(const T&);
    
    template<>
    TIKZCORE_EXPORT char *toString(const tikz::Uid & uid);
}

/**
 * Declare as movable, since the members of Uid are primitive types.
 */
Q_DECLARE_TYPEINFO(tikz::Uid, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(tikz::Uid)

/**
 * QHash function.
 */
inline uint qHash(const tikz::Uid & uid)
{
    return qHash(uid.id());
}

#endif // TIKZ_CORE_UID_H

// kate: indent-width 4; replace-tabs on;
