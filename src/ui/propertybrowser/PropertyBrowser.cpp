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

#include "PropertyBrowser.h"
#include "ValuePropertyManager.h"
#include "ValueSpinBoxFactory.h"
#include "OpacityEditorFactory.h"
#include "OpacityPropertyManager.h"

#include <tikz/core/NodeStyle.h>
#include <tikz/core/EdgeStyle.h>

#include <tikz/ui/View.h>
#include <tikz/ui/NodeItem.h>
#include <tikz/ui/PathItem.h>

#include <QDebug>
#include <QVBoxLayout>
#include <QPointer>
#include <QMetaProperty>

#include <QtTreePropertyBrowser>
#include <QtDoublePropertyManager>
#include <QtDoubleSpinBoxFactory>
#include <QtBoolPropertyManager>
#include <QtColorPropertyManager>
#include <QtColorEditorFactory>

#include <QFile>
#include <QJsonDocument>

class PropertyInfo
{
public:
    PropertyInfo() = default;
    void load(const QString & jsonFileName)
    {
        QFile file(jsonFileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Could not read file" << jsonFileName;
            return;
        }
        const auto xx = file.readAll();
//         qDebug() << xx;
        m_json = QJsonDocument::fromJson(xx);
    }

    bool isValid(const QString & propertyName) const
    {
        return m_json["properties"][propertyName].isObject();
    }

    QString type(const QString & propertyName) const
    {
        return m_json["properties"][propertyName]["type"].toString();
    }

    QString title(const QString & propertyName) const
    {
        const auto value = m_json["properties"][propertyName]["title"].toString();
        return value.isEmpty() ? propertyName : value;
    }

    QVariant minimum(const QString & propertyName) const
    {
        return m_json["properties"][propertyName]["minimum"].toVariant();
    }

    QVariant maximum(const QString & propertyName) const
    {
        return m_json["properties"][propertyName]["maximum"].toVariant();
    }

    double singleStep(const QString & propertyName) const
    {
        return m_json["properties"][propertyName]["singleStep"].toDouble(1.0);
    }

private:
    QJsonDocument m_json;
};

namespace tikz {
namespace ui {

class PropertyBrowserPrivate
{
public:
    QtTreePropertyBrowser * browser = nullptr;
    ValuePropertyManager * valueManager = nullptr;
    QtBoolPropertyManager * boolManager = nullptr;
    QtColorPropertyManager * colorManager = nullptr;
    OpacityPropertyManager * opacityManager = nullptr;
    QtDoublePropertyManager * doubleManager = nullptr;
    QtEnumPropertyManager * enumManager = nullptr;

    QPointer<View> view = nullptr;
    TikzItem * item = nullptr;
    QHash<QtProperty *, QString> propertyMap;

public:
    void addProperty(QtProperty *property, const QString & name)
    {
        Q_ASSERT(! propertyMap.contains(property));
        propertyMap[property] = name;
        QtBrowserItem *item = browser->addProperty(property);
//         browser->setExpanded(item, idToExpanded[name]);
    }

    void addSubProperty(QtProperty *parent, QtProperty *property, const QString & name)
    {
        Q_ASSERT(! propertyMap.contains(property));
        propertyMap[property] = name;
        parent->addSubProperty(property);
//         browser->setExpanded(item, idToExpanded[name]);
    }

    void addProperty(QObject * object, const QString & name)
    {
        PropertyInfo info;
        info.load("../data/properties.json");

        if (!info.isValid(name)) {
            qDebug() << "Unknown property type:" << name;
            return;
        }

        QVariant prop = object->property(name.toLatin1());
        if (!prop.isValid()) {
            qDebug() << "Unknown property type:" << name;
            return;
        }

        QtProperty *property = nullptr;

        const QString type = info.type(name);
        if (type == "value") {
            property = valueManager->addProperty(info.title(name));
            valueManager->setRange(property, tikz::Value(0, tikz::Unit::Millimeter), tikz::Value(10, tikz::Unit::Millimeter));
            valueManager->setMinimum(property, tikz::Value::fromString(info.minimum(name).toString()));
            valueManager->setMaximum(property, tikz::Value::fromString(info.maximum(name).toString()));
            valueManager->setSingleStep(property, info.singleStep(name));
            valueManager->setValue(property, prop.value<tikz::Value>());
        }
        else if (type == "bool") {
            property = boolManager->addProperty(info.title(name));
            boolManager->setValue(property, prop.toBool());
        }
        else if (type == "color") {
            property = colorManager->addProperty(info.title(name));
            colorManager->setValue(property, prop.value<QColor>());
        }
        else if (type == "opacity") {
            property = opacityManager->addProperty(info.title(name));
            opacityManager->setValue(property, prop.toDouble());
        }
        else if (type == "double") {
            property = doubleManager->addProperty(info.title(name));
            doubleManager->setMinimum(property, info.minimum(name).toDouble());
            doubleManager->setMaximum(property, info.maximum(name).toDouble());
            doubleManager->setSingleStep(property, info.singleStep(name));
            doubleManager->setValue(property, prop.toDouble());
        }
        else if (type == "enum") {
            // find out property type
            auto metaProp = object->metaObject()->property(object->metaObject()->indexOfProperty(name.toLatin1()));
            if (metaProp.userType() == QMetaType::UnknownType) {
                qDebug() << "Unknown enum type";
            }

            // remove namespace, if applicable
            QString typeName = metaProp.typeName();
            if (typeName.startsWith("tikz::")) {
                typeName.remove(0, 6);
            }

            const int enumIndex = tikz::staticMetaObject.indexOfEnumerator(typeName.toLatin1());
            const QMetaEnum metaEnum = tikz::staticMetaObject.enumerator(enumIndex);
            QStringList enumNames;
            for (int i = 0; i < metaEnum.keyCount(); ++i) {
                enumNames << metaEnum.key(i);
            }
            property = enumManager->addProperty(info.title(name));
            enumManager->setEnumNames(property, enumNames);
            enumManager->setValue(property, prop.toInt());
        }
        else {
            qDebug() << "Unknown property type:" << name;
        }
        if (property)
            addProperty(property, name);
    }
};





PropertyBrowser::PropertyBrowser(QWidget *parent)
    : QWidget(parent)
    , d(new PropertyBrowserPrivate)
{
    d->browser = new QtTreePropertyBrowser(this);
    auto vboxLayout = new QVBoxLayout(this);
    vboxLayout->setContentsMargins(0, 0, 0, 0);
    vboxLayout->addWidget(d->browser);

    // setup property managers
    d->valueManager = new ValuePropertyManager(this);
    d->boolManager = new QtBoolPropertyManager(this);
    d->colorManager = new QtColorPropertyManager(this);
    d->opacityManager = new OpacityPropertyManager(this);
    d->doubleManager = new QtDoublePropertyManager(this);
    d->enumManager = new QtEnumPropertyManager(this);

    // setup propertybrowser
    d->browser->setFactoryForManager(d->valueManager, new ValueSpinBoxFactory(this));
    d->browser->setFactoryForManager(d->boolManager, new QtCheckBoxFactory(this));
    d->browser->setFactoryForManager(d->colorManager, new QtColorEditorFactory(this));
    d->browser->setFactoryForManager(d->opacityManager, new OpacityEditorFactory(this));
    d->browser->setFactoryForManager(d->doubleManager, new QtDoubleSpinBoxFactory(this));
    d->browser->setFactoryForManager(d->enumManager, new QtEnumEditorFactory(this));

    d->browser->setPropertiesWithoutValueMarked(true);
    d->browser->setRootIsDecorated(false);
    d->browser->setResizeMode(QtTreePropertyBrowser::Interactive);

    // setup connections
    connect(d->valueManager, SIGNAL(valueChanged(QtProperty*, const tikz::Value &)),
            this, SLOT(valueChanged(QtProperty*, const tikz::Value &)));
    connect(d->boolManager, SIGNAL(valueChanged(QtProperty*, bool)),
            this, SLOT(valueChanged(QtProperty*, bool)));
    connect(d->colorManager, SIGNAL(valueChanged(QtProperty*, const QColor &)),
            this, SLOT(valueChanged(QtProperty*, const QColor &)));
    connect(d->opacityManager, SIGNAL(valueChanged(QtProperty*, qreal)),
            this, SLOT(valueChanged(QtProperty*, qreal)));
    connect(d->doubleManager, SIGNAL(valueChanged(QtProperty*, double)),
            this, SLOT(doubleValueChanged(QtProperty*, double)));
    connect(d->enumManager, SIGNAL(valueChanged(QtProperty*, int)),
            this, SLOT(enumValueChanged(QtProperty*, int)));
}

PropertyBrowser::~PropertyBrowser()
{
    delete d;
}

void PropertyBrowser::setView(tikz::ui::View * view)
{
    if (d->view == view) {
        return;
    }

    if (d->view) {
        disconnect(d->view, SIGNAL(selectionChanged(tikz::ui::View*)), this, SLOT(updateCurrentItem()));
    }

    d->view = view;
    if (d->view) {
        connect(d->view, SIGNAL(selectionChanged(tikz::ui::View*)), this, SLOT(updateCurrentItem()));
    }

    updateCurrentItem();
}

QObject * styleForItem(TikzItem * item)
{
    if (item == nullptr) {
        return nullptr;
    }

    auto node = qobject_cast<tikz::ui::NodeItem *>(item);
    auto path = qobject_cast<tikz::ui::PathItem *>(item);
    Q_ASSERT(node != nullptr || path != nullptr);

    auto nodeStyle = node ? node->style() : nullptr;
    auto edgeStyle = path ? path->style() : nullptr;
    tikz::core::Style * style = nodeStyle ? static_cast<tikz::core::Style *>(nodeStyle)
                                          : static_cast<tikz::core::Style *>(edgeStyle);

    return style;
}

void PropertyBrowser::setItem(TikzItem * item)
{
    if (d->item == item) {
        return;
    }

    d->item = item;

    // clear all managers and mappings
    d->valueManager->clear();
    d->boolManager->clear();
    d->colorManager->clear();
    d->opacityManager->clear();
    d->doubleManager->clear();
    d->enumManager->clear();
    d->propertyMap.clear();

    auto style = styleForItem(d->item);
    if (style) {
        auto metaObj = style->metaObject();
        while (metaObj) {
            for (int i = metaObj->propertyOffset(); i < metaObj->propertyCount(); ++i) {
                d->addProperty(style, metaObj->property(i).name());
            }
            metaObj = metaObj->superClass();
        }
    }
}

void PropertyBrowser::valueChanged(QtProperty *property, const tikz::Value & val)
{
    // if items are inserted, the slot valueChanged() is also called.
    // In this case, the item is not yet registered in the map. Hence,
    // we just return.
    if (! d->propertyMap.contains(property)) {
        return;
    }

    auto style = styleForItem(d->item);
    QString name = d->propertyMap[property];
    if (style) {
        if (style->metaObject()->indexOfProperty(name.toLatin1()) >= 0) {
            style->setProperty(name.toLatin1(), val);
            property->setModified(true);
        }
    }
}

void PropertyBrowser::valueChanged(QtProperty *property, bool val)
{
    // if items are inserted, the slot valueChanged() is also called.
    // In this case, the item is not yet registered in the map. Hence,
    // we just return.
    if (! d->propertyMap.contains(property)) {
        return;
    }

    auto style = styleForItem(d->item);
    QString name = d->propertyMap[property];
    if (style) {
        if (style->metaObject()->indexOfProperty(name.toLatin1()) >= 0) {
            style->setProperty(name.toLatin1(), val);
            property->setModified(true);
        }
    }
}

void PropertyBrowser::valueChanged(QtProperty *property, const QColor & val)
{
    // if items are inserted, the slot valueChanged() is also called.
    // In this case, the item is not yet registered in the map. Hence,
    // we just return.
    if (! d->propertyMap.contains(property)) {
        return;
    }

    auto style = styleForItem(d->item);
    QString name = d->propertyMap[property];
    if (style) {
        if (style->metaObject()->indexOfProperty(name.toLatin1()) >= 0) {
            style->setProperty(name.toLatin1(), val);
            property->setModified(true);
        }
    }
}

void PropertyBrowser::valueChanged(QtProperty *property, int val)
{
    // if items are inserted, the slot valueChanged() is also called.
    // In this case, the item is not yet registered in the map. Hence,
    // we just return.
    if (! d->propertyMap.contains(property)) {
        return;
    }

    auto style = styleForItem(d->item);
    QString name = d->propertyMap[property];
    if (style) {
        if (style->metaObject()->indexOfProperty(name.toLatin1()) >= 0) {
            style->setProperty(name.toLatin1(), val);
            property->setModified(true);
        }
    }
}

void PropertyBrowser::valueChanged(QtProperty *property, double val)
{
    // if items are inserted, the slot valueChanged() is also called.
    // In this case, the item is not yet registered in the map. Hence,
    // we just return.
    if (! d->propertyMap.contains(property)) {
        return;
    }

    auto style = styleForItem(d->item);
    QString name = d->propertyMap[property];
    if (style) {
        if (style->metaObject()->indexOfProperty(name.toLatin1()) >= 0) {
            style->setProperty(name.toLatin1(), val);
            property->setModified(true);
        }
    }
}

void PropertyBrowser::doubleValueChanged(QtProperty *property, double val)
{
    // if items are inserted, the slot valueChanged() is also called.
    // In this case, the item is not yet registered in the map. Hence,
    // we just return.
    if (! d->propertyMap.contains(property)) {
        return;
    }

    auto style = styleForItem(d->item);
    QString name = d->propertyMap[property];
    if (style) {
        if (style->metaObject()->indexOfProperty(name.toLatin1()) >= 0) {
            style->setProperty(name.toLatin1(), val);
            property->setModified(true);
        }
    }
}

void PropertyBrowser::enumValueChanged(QtProperty *property, int val)
{
    // if items are inserted, the slot valueChanged() is also called.
    // In this case, the item is not yet registered in the map. Hence,
    // we just return.
    if (! d->propertyMap.contains(property)) {
        return;
    }

    auto style = styleForItem(d->item);
    QString name = d->propertyMap[property];
    if (style) {
        if (style->metaObject()->indexOfProperty(name.toLatin1()) >= 0) {
            style->setProperty(name.toLatin1(), val);
            property->setModified(true);
        }
    }
}

void PropertyBrowser::updateCurrentItem()
{
    auto tikzItems = d->view->selectedItems();
    if (tikzItems.size() != 1) {
        setItem(nullptr);
        return;
    }

    setItem(tikzItems.front());
}

}
}

// kate: indent-width 4; replace-tabs on;
