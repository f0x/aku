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
    items << new QStandardItem(suffix);
    items << new QStandardItem(comment);

    if (extraction) {
        items << new QStandardItem(i18n("Supported"));
    } else {
        items << new QStandardItem(i18n("Not supported"));
    }

    if (creation) {
        items << new QStandardItem(i18n("Supported"));
    } else {
        items << new QStandardItem(i18n("Not supported"));
    }

    if (renaming) {
        items << new QStandardItem(i18n("Supported"));
    } else {
        items << new QStandardItem(i18n("Not supported"));
    }

    if (deletion) {
        items << new QStandardItem(i18n("Supported"));
    } else {
        items << new QStandardItem(i18n("Not supported"));
    }

    static_cast<QStandardItemModel*>(model())->appendRow(items);
}
