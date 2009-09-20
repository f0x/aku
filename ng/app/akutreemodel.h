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

#ifndef AKUTREEMODEL_H
#define AKUTREEMODEL_H

#include <QAbstractItemModel>
#include <QVector>

class AkuTreeNode;

class AkuTreeModel : public QAbstractItemModel
{
    Q_OBJECT
    public:
    enum AdditionalRoles { NodeRole = Qt::UserRole + 1 };
        AkuTreeModel(QVector<QStringList> filesList = QVector<QStringList>(), QObject *parent = 0);
        ~AkuTreeModel();

        int rowCount(const QModelIndex &parent = QModelIndex()) const;
        int columnCount(const QModelIndex & parent = QModelIndex()) const;

        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
        QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        QModelIndex parent(const QModelIndex &index) const;
        QVariant headerData(int section, Qt::Orientation, int role = Qt::DisplayRole) const;
        QVector<QStringList> sourceData();

        void setSourceData(const QVector<QStringList> &source);
        void setAdditionalHeaders(const QStringList &headers);

//        AkuTreeNode *nodeFromIndex(const QModelIndex &) const;

    private:
        class Private;
        Private *d;
};

#endif
