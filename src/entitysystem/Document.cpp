/* This file is part of the TikZKit project.
 *
 * Copyright (C) 2013-2016 Dominik Haumann <dhaumann@kde.org>
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

#include "Document.h"

#include "Entity.h"
#include "HistoryTransaction.h"
#include "HistoryManager.h"
#include "HistoryItemFactory.h"
#include "HistoryGroup.h"
#include "CreateEntityHistoryItem.h"
#include "DeleteEntityHistoryItem.h"
#include "ChangeEntityHistoryItem.h"

#include <QDebug>
#include <QTextStream>
#include <QFile>
#include <QUrl>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

namespace es {

// helper: remove \r and \n from visible document name (see Kate bug #170876)
inline static QString removeNewLines(const QString &str)
{
    QString tmp(str);
    return tmp.replace(QLatin1String("\r\n"), QLatin1String(" "))
           .replace(QLatin1Char('\r'), QLatin1Char(' '))
           .replace(QLatin1Char('\n'), QLatin1Char(' '));
}

class DocumentPrivate
{
    public:
        //! Document this private instance belongs to
        Document * q = nullptr;

        //! the Document's current url
        QUrl url;
        //! History manager
        HistoryManager * historyManager = nullptr;
        //! flag whether operations should add undo items or not
        bool undoActive = false;

        //! Entity list, contains Nodes and Paths, etc.
        std::vector<Entity *> entities;

        //! Node lookup map
        QHash<Eid, Entity *> entityMap;

        qint64 nextId = 0;

        //! helper to get a document-wide unique id
        qint64 uniqueId()
        {
            return nextId++;
        }

        QString docName = QString("Untitled");

//
// helper functions
//
public:
    void updateDocumentName() {
        if (! url.isEmpty() && docName == removeNewLines(url.fileName())) {
            return;
        }

        QString newName = removeNewLines(url.fileName());

        if (newName.isEmpty()) {
            newName = "Untitled";
        }

        if (newName != docName) {
            docName = newName;
            emit q->documentNameChanged(q);
        }
    }

    void close()
    {
        // free all node and path data
        qDeleteAll(entities);
        entities.clear();
        entityMap.clear();

        // reset unique id counter
        nextId = 0;

        // clear undo stack
        historyManager->clear();

        // unnamed document
        url.clear();
    }
};

Document::Document(QObject * parent)
    : ConfigObject(parent)
    , d(new DocumentPrivate())
{
    d->q = this;
    d->historyManager = new HistoryManager(this);
    d->undoActive = false;
    d->nextId = 0;

    connect(d->historyManager, SIGNAL(cleanChanged(bool)), this, SIGNAL(modifiedChanged()));
}

Document::~Document()
{
    // clear Document contents
    d->close();

    // make sure things are really gone
    Q_ASSERT(d->entityMap.isEmpty());
    Q_ASSERT(d->entities.empty());

    delete d;
}

void Document::close()
{
    // tell the world that all Entities are about to be deleted
    emit aboutToClear();

    // cleanup Document contents
    d->close();

    // keep the document name up-to-date
    d->updateDocumentName();
}

bool Document::load(const QUrl & fileurl)
{
    // first start a clean document
    close();

    // open file + read all json contents
    QFile file(fileurl.toLocalFile());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
         return false;
    }

    QJsonDocument json = QJsonDocument::fromJson(file.readAll());
    QJsonObject root = json.object();

    // read history and replay
    HistoryItemFactory factory(this);
    QJsonArray history = root["history"].toArray();
    for (auto action : history) {
        QJsonObject entry = action.toObject();
        HistoryTransaction transaction(this, entry["text"].toString());
        QJsonArray items = entry["items"].toArray();
        for (auto item : items) {
            QJsonObject joItem = item.toObject();
            const QString type = joItem["type"].toString();
            HistoryItem * undoItem = factory.createItem(type);
            if (undoItem) {
                undoItem->load(joItem);
                addUndoItem(undoItem);
            }
        }
    }

    // load payload
    QJsonObject data = root["data"].toObject();
    loadData(data);

    // now make sure the next free uniq id is valid by finding the maximum
    // used id, and then add "+1".
    auto keys = d->entityMap.keys();
    if (keys.size()) {
        d->nextId = *std::max_element(keys.begin(), keys.end()) + 1;
    }

    // keep the document name up-to-date
    d->updateDocumentName();

    // mark this state as unmodified
    d->historyManager->setClean();

    return true;
}

bool Document::reload()
{
    if (!d->url.isEmpty()) {
        return load(d->url);
    }
    return false;
}

bool Document::save()
{
    return saveAs(d->url);
}

bool Document::saveAs(const QUrl & targetUrl)
{
    const bool urlChanged = d->url.toLocalFile() != targetUrl.toLocalFile();

    if (targetUrl.isLocalFile()) {

        QJsonObject json;
        json["history"] = d->historyManager->save();

        // save payload
        json["data"] = saveData();

        // now save data
        QFile file(targetUrl.toLocalFile());
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            return false;
        }

        // write json to text stream
        QTextStream stream(&file);
        QJsonDocument jsonDoc(json);
        stream << jsonDoc.toJson();

        if (urlChanged) {
            d->url = targetUrl;
            // keep the document name up-to-date
            d->updateDocumentName();
        }

        // mark this state as unmodified
        d->historyManager->setClean();

        return true;
    }

    return false;
}

void Document::loadData(const QJsonObject & json)
{
}

QJsonObject Document::saveData()
{
    return QJsonObject();
}

QUrl Document::url() const
{
    return d->url;
}

QString Document::documentName() const
{
    return d->docName;
}

bool Document::isEmptyBuffer() const
{
    return d->url.isEmpty()
        && ! isModified()
        && d->entities.empty();
}

void Document::addUndoItem(es::HistoryItem * item)
{
    d->historyManager->addUndoItem(item);
}

void Document::addRedoItem(es::HistoryItem * item)
{
    d->historyManager->addRedoItem(item);
}

void Document::beginTransaction(const QString & name)
{
    // track changes
    beginConfig();

    // pass call to undo mananger
    d->historyManager->startTransaction(name);
}

void Document::cancelTransaction()
{
    d->historyManager->cancelTransaction();
}

void Document::finishTransaction()
{
    // first pass call to undo mananger
    d->historyManager->commitTransaction();

    // notify world about changes
    endConfig();
}

bool Document::transactionRunning() const
{
    return d->historyManager->transactionActive();
}

bool Document::setUndoActive(bool active)
{
    const bool lastState = d->undoActive;
    d->undoActive = active;
    return lastState;
}

bool Document::undoActive() const
{
    return d->undoActive;
}

bool Document::isModified() const
{
    return ! d->historyManager->isClean();
}

bool Document::undoAvailable() const
{
    return d->historyManager->undoAvailable();
}

bool Document::redoAvailable() const
{
    return d->historyManager->redoAvailable();
}

void Document::undo()
{
    const bool undoWasAvailable = undoAvailable();
    const bool redoWasAvailable = redoAvailable();

    d->historyManager->undo();

    const bool undoNowAvailable = undoAvailable();
    const bool redoNowAvailable = redoAvailable();

    if (undoWasAvailable != undoNowAvailable) {
        emit undoAvailableChanged(undoNowAvailable);
    }

    if (redoWasAvailable != redoNowAvailable) {
        emit redoAvailableChanged(redoNowAvailable);
    }
}

void Document::redo()
{
    const bool undoWasAvailable = undoAvailable();
    const bool redoWasAvailable = redoAvailable();

    d->historyManager->redo();

    const bool undoNowAvailable = undoAvailable();
    const bool redoNowAvailable = redoAvailable();

    if (undoWasAvailable != undoNowAvailable) {
        emit undoAvailableChanged(undoNowAvailable);
    }

    if (redoWasAvailable != redoNowAvailable) {
        emit redoAvailableChanged(redoNowAvailable);
    }
}

Entity * Document::createEntity(const QString & type)
{
    return createEntity(Eid(d->uniqueId(), this), type);
}

Entity * Document::createEntity(const Eid & eid, const QString & type)
{
    Q_ASSERT(eid.isValid());
    Q_ASSERT(!d->entityMap.contains(eid));

    HistoryTransaction ht(this, "Create Entity");

    // create new entity
    auto entity = (Entity*)nullptr; // TODO, FIXME: new Node(eid, this);
    d->entities.push_back(entity);

    // insert entity into hash map
    d->entityMap.insert(eid, entity);

    // undo/redo support
    if (!undoActive()) {
        d->historyManager->addUndoItem(new DeleteEntityHistoryItem(eid, this));
        d->historyManager->addRedoItem(new CreateEntityHistoryItem(eid, this));
        d->historyManager->addRedoItem(new ChangeEntityHistoryItem(eid, this));
    }

    // propagate changed signal
    connect(entity, &ConfigObject::changed, this, &ConfigObject::emitChangedIfNeeded);

    // notify
    emit entityCreated(entity);

    return entity;
}

void Document::deleteEntity(const Eid & eid)
{
    // valid input?
    Q_ASSERT(eid.isValid());
    Q_ASSERT(d->entityMap.contains(eid));

    // get entity
    auto it = d->entityMap.find(eid);
    if (it != d->entityMap.end()) {
        const auto entity = *it;

        // notify about deletion
        emit aboutToDeleteEntity(entity);

        // undo/redo support
        if (!undoActive()) {
            d->historyManager->addUndoItem(new ChangeEntityHistoryItem(eid, this));
            d->historyManager->addUndoItem(new CreateEntityHistoryItem(eid, this));
            d->historyManager->addRedoItem(new DeleteEntityHistoryItem(eid, this));
        }

        // unregister entity
        d->entityMap.erase(it);
        Q_ASSERT(std::find(d->entities.begin(), d->entities.end(), entity) != d->entities.end());
        d->entities.erase(std::find(d->entities.begin(), d->entities.end(), entity));

        // truly delete node
        delete entity;
    }
}

void Document::deleteEntity(Entity * entity)
{
    Q_ASSERT(entity);
    deleteEntity(entity->eid());
}

Entity * Document::entity(const es::Eid & eid) const
{
    if (eid.document() != this) {
        return nullptr;
    }

    const auto it = d->entityMap.find(eid);
    if (it != d->entityMap.end()) {
        return *it;
    }

    return nullptr;
}

const std::vector<Entity *> & Document::entities() const
{
    return d->entities;
}

}

// kate: indent-width 4; replace-tabs on;
