 /*

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/

#ifndef AKUTREEMODEL_H
#define AKUTREEMODEL_H

#include <QAbstractItemModel>
#include <QVector>

class QStringList;

class AkuTreeModel : public QAbstractItemModel
{
    Q_OBJECT
    public:
        AkuTreeModel(QVector<QStringList> filesList = QVector<QStringList>(), QObject *parent = 0);
        ~AkuTreeModel();

        int columnCount(const QModelIndex & parent = QModelIndex()) const;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
        int rowCount(const QModelIndex &parent = QModelIndex()) const;
        QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        QModelIndex parent(const QModelIndex &index) const;
        QVariant headerData(int section, Qt::Orientation, int role = Qt::DisplayRole) const;
        void setSourceData(const QVector<QStringList> &source);
        QVector<QStringList> sourceData();

    private:
        class Private;
        Private *d;
};
#endif
