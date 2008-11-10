/* This file is part of the KDE project

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/ 
#ifndef PLUGININFODELEGATE_H
#define PLUGININFODELEGATE_H

#include <QStyledItemDelegate>

class QStyleOptionViewItem;

class PluginInfoDelegate : public QStyledItemDelegate
{
    Q_OBJECT

    public:
        enum DataRoles { PluginDescriptionRole = 33, 
                         PluginIconRole,
                         PluginPropertyRole
                       };

        PluginInfoDelegate(QObject *parent = 0);
        ~PluginInfoDelegate();

        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

    private:
        void paintMainCol(QPainter *painter, const QStyleOptionViewItemV4 &option, const QModelIndex &index) const;
        void paintPropertyCol(QPainter *painter, const QStyleOptionViewItemV4 &option, const QModelIndex &index) const;
};

#endif