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

PluginView::PluginView(QWidget *parent) : QListView(parent)
{
    QStandardItemModel *model = new QStandardItemModel();
    
}
