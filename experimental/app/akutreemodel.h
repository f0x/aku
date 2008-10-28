 /* This file is part of the KDE project

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/

#ifndef AKUTREEMODEL_H
#define AKUTREEMODEL_H

#include <QAbstractItemModel>

class QStringList;

class AkuTreeModel : public QAbstractItemModel
{
    Q_OBJECT
    public:
        AkuTreeModel(QVector<QStringList> filesList, QObject *parent = 0);
        ~AkuTreeModel();

        int columnCount(const QModelIndex & parent = QModelIndex()) const;
        QVariant data(const QModelIndex &parent = QModelIndex(), int role = Qt::DisplayRole) const;
        int rowCount(const QModelIndex &parent = QModelIndex()) const;

    private:
        class Private;
        Private *d;
};
#endif
