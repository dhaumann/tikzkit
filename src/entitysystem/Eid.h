/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2015-2016 Dominik Haumann <dhaumann@kde.org>
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
#ifndef ENTITY_SYSTEM_EID_H
#define ENTITY_SYSTEM_EID_H

#include "es_export.h"

#include <QVariant>
#include <QDebug>

namespace es {

class Entity;
class Document;

/**
 * The Eid class provides document-wide unique identifiers.
 *
 * The Eid is defined by an integer number that refers to an Entity of
 * the associated Document.
 *
 * @see Entity, Document
 */
class ES_EXPORT Eid
{
    public:
        /**
         * Default constructor. Creates an invalid Eid.
         */
        explicit constexpr Eid() noexcept
        {
        }

        /**
         * Constructor with value and type.
         */
        explicit constexpr Eid(qint64 id, Document * doc) noexcept
            : m_id(id)
            , m_document(doc)
        {
        }

        /**
         * Constructor with value and type.
         */
        explicit Eid(const QString & idStr, es::Document * doc) noexcept
            : m_id(-1)
            , m_document(doc)
        {
            bool ok = false;
            m_id = idStr.toLongLong(&ok);
            Q_ASSERT(ok);

            if (! ok) {
                m_id = -1;
            }
        }

        /**
         * Returns @e true, if id() >= 0 and document() is other than a null pointer,
         * otherwise returns @e false.
         */
        inline constexpr bool isValid() const noexcept
        {
            return m_id >= 0 && m_document != nullptr;
        }

        /**
         * Returns the Document this Eid belongs to.
         */
        inline constexpr Document * document() const noexcept
        {
            return m_document;
        }

        /**
         * Get the internal id as qint64.
         */
        inline constexpr qint64 id() const noexcept
        {
            return m_id;
        }

        /**
         * Returns the Entity this Eid refers to.
         */
        Entity * entity() const;

        /**
         * Returns the Entity type, or an empty string, if no Entity is associated.
         */
        const char * type() const;

        /**
         * Convert this Eid to a string of the form "id (type)"
         */
        inline QString toString() const noexcept
        {
            return QString::number(m_id);
        }

    //
    // operators
    //
    public:
        /**
         * Implicit conversion to qint64.
         */
        constexpr inline operator qint64() const noexcept
        {
            return m_id;
        }

        /**
         * QDebug support.
         */
        inline friend QDebug operator<<(QDebug s, const es::Eid& eid)
        {
            s.nospace() << eid.toString();
            return s.space();
        }

        operator QVariant() const
        {
            return QVariant::fromValue(*this);
        }

    private:
        /**
         * The associated Document of this Eid.
         */
        Document * m_document = nullptr;

        /**
         * The value.
         */
        qint64 m_id = -1;
};

/**
 * Equality operator.
 * Return @e true, if the Eid @p lhs refers to the same Entity as Eid @p rhs.
 */
inline constexpr bool operator==(const Eid & lhs, const Eid & rhs) noexcept
{
    return lhs.isValid()
        && rhs.isValid()
        && (lhs.id() == rhs.id())
        && (lhs.document() == rhs.document());
}

/**
 * Inequality operator.
 * Return @e true, if the Eid @p lhs refers to a different Entity than Eid @p rhs.
 */
inline constexpr bool operator!=(const Eid & lhs, const Eid & rhs) noexcept
{
    return ! (lhs == rhs);
}

} //namespace es

namespace QTest
{
    // forward declaration of template in qtestcase.h
    template<typename T> char* toString(const T&);
    
    template<>
    ES_EXPORT char *toString(const es::Eid & eid);
}

/**
 * Declare as movable, since the members of Eid are primitive types.
 */
Q_DECLARE_TYPEINFO(es::Eid, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(es::Eid)

/**
 * QHash function.
 */
inline uint qHash(const es::Eid & eid)
{
    return qHash(eid.id());
}

#endif // ENTITY_SYSTEM_EID_H

// kate: indent-width 4; replace-tabs on;
