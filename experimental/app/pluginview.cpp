/* This file is part of the KDE project

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/ 

#include "pluginview.h"
#include "plugininfodelegate.h"

#include <QStandardItemModel>
#include <QStandardItem>

#include <KLocale>
#include <KPushButton>
#include <KIcon>
#include <KDialog>
#include <KPluginInfo>

PluginView::PluginView(QWidget *parent) : QTreeView(parent)
{
    QStandardItemModel *model = new QStandardItemModel();
    model->setHorizontalHeaderLabels(QStringList()<<i18n("Archive Plugin")<<
                                     i18n("Extraction")<<i18n("Creation")<<i18n("Rename")<<
                                     i18n("Delete")<<i18n("Working Properly")<<i18n("Configuration"));
    setModel(model);
    setItemDelegate(new PluginInfoDelegate);
    setRootIsDecorated(false);
}

PluginView::~PluginView()
{}

void PluginView::addPluginInfo(const QString &suffix, const QString &comment,
                               bool extraction, bool deletion, bool creation,
                               bool renaming, bool working, const KPluginInfo &info, QWidget *config)
{
    QList<QStandardItem*> items;

    QStandardItem *itsuf = new QStandardItem(suffix);
    itsuf->setEditable(false);

    itsuf->setData(comment, PluginInfoDelegate::PluginDescriptionRole);
    itsuf->setData(info.icon(), PluginInfoDelegate::PluginIconRole);

    items << itsuf;

//     QStandardItem *itcom = new QStandardItem(comment);
//     itcom->setEditable(false);
//     items << itcom;

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

    if (working) {
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

    if (config) {
        KPushButton *configure = new KPushButton;
        configure->setIcon(KIcon("configure"));
        configure->setMaximumSize(configure->minimumSizeHint());

        QStandardItem *bItem = new QStandardItem();
        items << bItem;

        static_cast<QStandardItemModel*>(model())->appendRow(items);

        setIndexWidget(bItem->index(), configure);

        KDialog *dialog = new KDialog;
        dialog->setCaption(comment);
        dialog->setMainWidget(config);

        connect(configure, SIGNAL(clicked()), dialog, SLOT(exec()));

        return;
    }

    static_cast<QStandardItemModel*>(model())->appendRow(items);

}
