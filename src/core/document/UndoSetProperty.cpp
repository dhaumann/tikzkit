/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013-2018 Dominik Haumann <dhaumann@kde.org>
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

#include "UndoSetProperty.h"
#include "Path.h"
#include "Document.h"
#include "PropertyManager.h"

namespace tikz {
namespace core {

UndoSetProperty::UndoSetProperty(Document * doc)
    : UndoItem("Change Property", doc)
{
}

UndoSetProperty::UndoSetProperty(const Uid & entityUid, const QString & propertyName, const QVariant & newValue)
    : UndoItem("Change Property", entityUid.document())
    , m_entityUid(entityUid)
    , m_propertyName(propertyName)
    , m_redoValue(newValue)
{
    // get path to save data
    auto entity = m_entityUid.entity();
    Q_ASSERT(entity);

    auto info = propertyManager().info(propertyName);
    if (info.isResettable()) {
        bool isModified = false;
        const bool ok = QMetaObject::invokeMethod(entity,
                                                  info.modifiedFunction().toLatin1().data(),
                                                  Qt::DirectConnection,
                                                  Q_RETURN_ARG(bool, isModified)
        );
        if (!ok) {
            qDebug() << "Unable to check modified state of property" + m_propertyName;
            entity->setProperty(m_propertyName.toLatin1().data(), m_undoValue);
        }

        m_undoValue = (ok && isModified) ? entity->property(propertyName.toLatin1().data()) : QVariant();
    } else {
        m_undoValue = entity->property(propertyName.toLatin1().data());
    }
}

UndoSetProperty::~UndoSetProperty()
{
}

void UndoSetProperty::undo()
{
    auto entity = m_entityUid.entity();
    Q_ASSERT(entity);

    if (m_undoValue.isValid()) {
        entity->setProperty(m_propertyName.toLatin1().data(), m_undoValue);
    } else {
        const auto info = propertyManager().info(m_propertyName);
        const bool ok = QMetaObject::invokeMethod(entity,
                                                  info.resetFunction().toLatin1().data(),
                                                  Qt::DirectConnection
        );
        if (!ok) {
            qDebug() << "Unable to reset property" + m_propertyName;
            entity->setProperty(m_propertyName.toLatin1().data(), m_undoValue);
        }
    }
}

void UndoSetProperty::redo()
{
    auto entity = m_entityUid.entity();
    Q_ASSERT(entity);

    if (m_redoValue.isValid()) {
        entity->setProperty(m_propertyName.toLatin1().data(), m_redoValue);
    } else {
        const auto info = propertyManager().info(m_propertyName);
        const bool ok = QMetaObject::invokeMethod(entity,
                                                  info.resetFunction().toLatin1().data(),
                                                  Qt::DirectConnection
        );
        if (!ok) {
            qDebug() << "Unable to reset property" + m_propertyName;
            entity->setProperty(m_propertyName.toLatin1().data(), m_redoValue);
        }
    }
}

bool UndoSetProperty::mergeWith(const UndoItem * command)
{
    // only merge when command is of correct type
    auto other = static_cast<const UndoSetProperty*>(command);
    if (m_entityUid != other->m_entityUid || m_propertyName != other->m_propertyName) {
        return false;
    }

    m_redoValue = other->m_redoValue;
    return true;
}

void UndoSetProperty::loadData(const QJsonObject & json)
{
    m_entityUid = Uid(json["uid"].toString(), document());
    //m_redoStyle.load(json["style"].toObject());
}

QJsonObject UndoSetProperty::saveData() const
{
    QJsonObject json;
    json["type"] = "entity-set-property";
    json["uid"] = m_entityUid.toString();
//     json["style"] = m_redoStyle.save();
    return json;
}

}
}

// kate: indent-width 4; replace-tabs on;
