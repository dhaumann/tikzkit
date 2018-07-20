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
namespace core {

class Entity;
class Document;

/**
 * The Uid class provides document-wide unique identifiers.
 *
 * The Uid is defined by an integer number that refers to an Entity of
 * the associated Document.
 *
 * @see Entity, Document
 */
class TIKZCORE_EXPORT Uid
{
    public:
        /**
         * Default constructor. Creates an invalid Uid.
         */
        explicit constexpr Uid() noexcept = default;

        /**
         * Constructor with value and type.
         */
        explicit constexpr Uid(qint64 id, Document * doc) noexcept
            : m_document(doc)
            , m_id(id)
        {
        }

        /**
         * Constructor with value and type.
         */
        explicit Uid(const QString & idStr, tikz::core::Document * doc) noexcept
            : m_document(doc)
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
         * Returns the Document this Uid belongs to.
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
         * Returns the Entity this Uid refers to.
         */
        Entity * entity() const;

        /**
         * Templated getter for accessing the entity this Uid refers to as a
         * specific type.
         */
        template <typename T>
        T * entity() const
        {
            return qobject_cast<T*>(entity());
        }

        /**
         * Convert this Uid to a string of the form "id (type)"
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
        inline friend QDebug operator<<(QDebug s, const tikz::core::Uid& uid)
        {
            s.nospace() << uid.toString();
            return s.space();
        }

        operator QVariant() const
        {
            return QVariant::fromValue(*this);
        }

    private:
        /**
         * The associated Document of this Uid.
         */
        Document * m_document = nullptr;

        /**
         * The value.
         */
        qint64 m_id = -1;
};

/**
 * Equality operator.
 * Return @e true, if the Uid @p lhs refers to the same Entity as Uid @p rhs.
 */
inline constexpr bool operator==(const Uid & lhs, const Uid & rhs) noexcept
{
    return lhs.isValid()
        && rhs.isValid()
        && (lhs.id() == rhs.id())
        && (lhs.document() == rhs.document());
}

/**
 * Inequality operator.
 * Return @e true, if the Uid @p lhs refers to a different Entity than Uid @p rhs.
 */
inline constexpr bool operator!=(const Uid & lhs, const Uid & rhs) noexcept
{
    return ! (lhs == rhs);
}

} //namespace core
} //namespace tikz

namespace QTest
{
    // forward declaration of template in qtestcase.h
    template<typename T> char* toString(const T&);
    
    template<>
    TIKZCORE_EXPORT char *toString(const tikz::core::Uid & uid);
}

/**
 * Declare as movable, since the members of Uid are primitive types.
 */
Q_DECLARE_TYPEINFO(tikz::core::Uid, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(tikz::core::Uid)

/**
 * QHash function.
 */
inline uint qHash(const tikz::core::Uid & uid)
{
    return qHash(uid.id());
}

#endif // TIKZ_CORE_UID_H

// kate: indent-width 4; replace-tabs on;
