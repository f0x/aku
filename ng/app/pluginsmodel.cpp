/***************************************************************************
 *   Copyright 2009 by Alessandro Diaferia <alediaferia@gmail.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/
#include "pluginsmodel.h"

#include <KIcon>
#include <KServiceTypeTrader>
#include <KService>
#include <KSycoca>
#include <KDebug>

PluginsModel::PluginsModel(QObject *parent) : QAbstractItemModel(parent)
{
    init();
}

PluginsModel::~PluginsModel()
{}

void PluginsModel::init()
{
    KService::List plugins = KServiceTypeTrader::self()->query("Aku/Plugin");
    if (plugins.isEmpty()) {
        kError() << "no available plugin";
        return;
    }

    addAvailableModels(plugins);
}

int PluginsModel::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return 1;
    }

    return 0;
}

int PluginsModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return m_modelServices.count();
    }

    return 0;
}

QVariant PluginsModel::data(const QModelIndex &index, int role) const
{
    if (index.parent().isValid()) {
        return QVariant();
    }

    if (index.row() >= rowCount() || index.column() >= columnCount()) {
        return QVariant();
    }

    QVariant data;
    if (role == Qt::DisplayRole) {
        KService::Ptr service = this->service(static_cast<KService*>(index.internalPointer()));
        if (!service.isNull()) {
            data = service->name();
        }
    } else if (role == Qt::DecorationRole) {
        KService::Ptr service = this->service(static_cast<KService*>(index.internalPointer()));
        if (!service.isNull()) {
            data = KIcon(service->icon());
        }
    }

    return data;
}

QModelIndex PluginsModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return QModelIndex();
    }

    if (row >= rowCount() || column >= columnCount()) {
        return QModelIndex();
    }

    return createIndex(row, column, (KService*)m_modelServices[row].data());
}

QModelIndex PluginsModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return QModelIndex();
}

void PluginsModel::addAvailableModels(const KService::List &models)
{
    beginInsertRows(QModelIndex(), 0, models.count() - 1);
    m_modelServices << models;
    endInsertRows();
}

KService::Ptr PluginsModel::service(KService *service) const
{
    KService::List::const_iterator i = m_modelServices.constBegin();
    KService::List::const_iterator end = m_modelServices.constEnd();
    for (; i != end; ++i) {
        if (*i == service) {
            return *i;
        }
    }

    return KService::Ptr();
}
