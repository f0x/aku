 /* This file is part of the KDE project

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/ 

#ifndef PLUGINVIEW_H
#define PLUGINVIEW_H

#include <QListView>

class PluginView : public QListView
{
    Q_OBJECT
    public:
        PluginView(QWidget *parent = 0);
        ~PluginView();

        void addPluginInfo(QString extension, QString comment);

};

#endif
