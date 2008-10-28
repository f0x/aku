/* This file is part of the KDE project

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/ 

#include "pluginview.h"

#include <QStandardItemModel>
#include <QStandardItem>
#include <KLocale>

PluginView::PluginView(QWidget *parent) : QTreeView(parent)
{
    QStandardItemModel *model = new QStandardItemModel();
    model->setHorizontalHeaderLabels(QStringList()<<i18n("Archive suffix")<<i18n("Archive Type")<<
                                     i18n("Extraction")<<i18n("Creation")<<i18n("Rename")<<i18n("Delete"));
    setModel(model);
    setRootIsDecorated(false);
}

PluginView::~PluginView()
{}

void PluginView::addPluginInfo(const QString &suffix, const QString &comment,
                               bool extraction, bool deletion, bool creation, bool renaming)
{
    QList<QStandardItem*> items;
    QStandardItem *itsuf = new QStandardItem(suffix);
    itsuf->setEditable(false);
    items << itsuf;
    QStandardItem *itcom = new QStandardItem(comment);
    itcom->setEditable(false);
    items << itcom;

    if (extraction) {
        QStandardItem *item = new QStandardItem();
        item->setCheckState(Qt::Checked);
        item->setEditable(false);
        items << item;
    } else {
        QStandardItem *item = new QStandardItem();
        item->setCheckState(Qt::Unchecked);
        item->setEditable(false);
        items << item;
    }

    if (creation) {
        QStandardItem *item = new QStandardItem();
        item->setCheckState(Qt::Checked);
        item->setEditable(false);
        items << item;
    } else {
        QStandardItem *item = new QStandardItem();
        item->setCheckState(Qt::Unchecked);
        item->setEditable(false);
        items << item;
    }

    if (renaming) {
        QStandardItem *item = new QStandardItem();
        item->setCheckState(Qt::Checked);
        item->setEditable(false);
        items << item;
    } else {
        QStandardItem *item = new QStandardItem();
        item->setCheckState(Qt::Unchecked);
        item->setEditable(false);
        items << item;
    }

    if (deletion) {
        QStandardItem *item = new QStandardItem();
        item->setCheckState(Qt::Checked);
        item->setEditable(false);
        items << item;
    } else {
        QStandardItem *item = new QStandardItem();
        item->setCheckState(Qt::Unchecked);
        item->setEditable(false);
        items << item;
    }
 

    static_cast<QStandardItemModel*>(model())->appendRow(items);
}
