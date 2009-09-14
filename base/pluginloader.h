 /* This file is part of the KDE project

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/

#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include <QObject>

class AkuPlugin;
class KPluginInfo;

class PluginLoader : public QObject
{
    Q_OBJECT

    public:
        PluginLoader(QObject *parent);
        ~PluginLoader();

        void loadAllPlugins();

    signals:
        void pluginLoaded(AkuPlugin *plugin, const KPluginInfo &);
};
#endif 
