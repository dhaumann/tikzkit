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

#include "ConfigObject.h"

namespace tikz {
namespace core {

ConfigObject::ConfigObject(QObject * parent)
    : QObject(parent)
{
}

ConfigObject::~ConfigObject()
{
}

void ConfigObject::beginConfig()
{
    Q_ASSERT(m_refCounter >= 0);
    ++m_refCounter;
}

void ConfigObject::endConfig()
{
    Q_ASSERT(m_refCounter > 0);

    --m_refCounter;
    if (m_refCounter == 0) {
        Q_EMIT changed();
    }
}

bool ConfigObject::configActive() const
{
    return m_refCounter > 0;
}

void ConfigObject::emitChangedIfNeeded()
{
    if (! configActive()) {
        Q_EMIT changed();
    }
}

ConfigTransaction::ConfigTransaction(ConfigObject * configObject)
    : m_configObject(configObject)
{
    Q_ASSERT(configObject);

    m_configObject->beginConfig();
}

ConfigTransaction::~ConfigTransaction()
{
    endConfig();
}

void ConfigTransaction::endConfig()
{
    if (m_configObject) {
        m_configObject->endConfig();
        m_configObject = nullptr;
    }
}

}
}

// kate: indent-width 4; replace-tabs on;
