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

#ifndef TIKZ_DOCUMENT_H
#define TIKZ_DOCUMENT_H

#include <EntitySystem/Document.h>
#include "tikz_export.h"
#include "tikz.h"
#include "Path.h"
#include "MetaPos.h"

#include <QVector>

class QUrl;

namespace tikz {
namespace core {

class DocumentPrivate;
class Style;
class Node;
class NodeStyle;
class EdgeStyle;
class UndoItem;
class Visitor;

class TIKZCORE_EXPORT Document : public es::Document
{
    Q_OBJECT

    public:
        /**
         * Default constructor.
         */
        explicit Document(QObject * parent = 0);

        /**
         * Destructor
         */
        virtual ~Document();

    //
    // visitor pattern
    //
    public:
        /**
         * Visitor pattern.
         * Visits all elements of the document.
         */
        bool accept(Visitor & visitor);

        /**
         * Export the picture to TikZ.
         */
        QString tikzCode();

//         /**
//          * Returns the history model.
//          */
//         QAbstractItemModel * historyModel() const;

    // es::Document overrides
    protected:
        /**
         * Load the payload for derived Documents.
         */
        void loadData(const QJsonObject & json) override;

        /**
         * Save the Document to the file opened with load().
         */
        QJsonObject saveData() override;

    //
    // convenience functions
    //
    public:
        /**
         * Returns the position for @p pos in tikz::Pos coordinates.
         */
        virtual tikz::Pos scenePos(const MetaPos & pos) const;

        /**
         * Sets the preferred Unit to @p unit.
         * The preferred Unit is typically used by the graphical scene when
         * moving nodes and paths.
         * @see preferredUnit(), referredUnitChanged()
         */
        void setPreferredUnit(tikz::Unit unit);

        /**
         * Returns the currently preferred unit.
         * @see setPreferredUnit(), referredUnitChanged()
         */
        tikz::Unit preferredUnit() const;

    Q_SIGNALS:
        /**
         * This signal is emitted whenever the preferred unit changed.
         * @see setPreferredUnit(), preferredUnit()
         */
        void preferredUnitChanged(tikz::Unit unit);

    //
    // Node, Path and style management
    //
    public:
        /**
         * Get the global document style.
         * This is the global document style. All other styles fall back to
         * this style, if a property is not set.
         */
        Style * style() const;

        /**
         * Get the list of nodes of the tikz document.
         */
        QVector<Node*> nodes() const;

        /**
         * Get the list of paths of the tikz document.
         */
        QVector<Path*> paths() const;

    //
    // Node and path creation
    //
    public:
        /**
         * Delete entity @p eid associated with this document.
         */
        void deleteEntity(const es::Eid & eid) override;

    //
    // data pointer
    //
    private:
        DocumentPrivate * const d;

    //
    // friends
    //
    protected:
        // visitors
        friend class DeserializeVisitor;
};

}
}

#endif // TIKZ_DOCUMENT_H

// kate: indent-width 4; replace-tabs on;
