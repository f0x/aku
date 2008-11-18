 /* This file is part of the KDE project

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/ 

#ifndef PLUGINVIEW_H
#define PLUGINVIEW_H

#include <QTreeView>

class KPluginInfo;

class PluginView : public QTreeView
{
    Q_OBJECT
    public:
        PluginView(QWidget *parent = 0);
        ~PluginView();

        void addPluginInfo(const QString &suffix, const QString &comment, bool extraction,
                            bool deletion, bool creation, bool renaming, bool encrypting, 
                            bool addComment, bool locking, bool working,
                            const KPluginInfo &, QWidget *config = 0);

};

#endif
