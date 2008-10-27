 /* This file is part of the KDE project

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/ 

#include "pluginloader.h"
#include "akuplugin.h"

#include <KServiceTypeTrader>
#include <KDebug>

PluginLoader::PluginLoader(QObject *parent)
    : QObject(parent)
{}

PluginLoader::~PluginLoader()
{}

void PluginLoader::loadAllPlugins()
{
    kDebug()<<"";

    KService::List offers = KServiceTypeTrader::self()->query("Aku/Plugin");

    KService::List::const_iterator iter;

    for (iter = offers.begin(); iter < offers.end(); ++iter) {
        KService::Ptr service = *iter;
        KPluginFactory *factory = KPluginLoader(service->library()).factory();

        if (!factory) {
            kError()<<"Unable to load:"<<service->library();
            continue;
        }

        AkuPlugin *plugin = factory->create<AkuPlugin>(this);

        if (plugin) {
            kDebug()<<"Loading:"<<service->name();
            emit pluginLoaded(plugin);
        } else {
            kDebug()<<"plugin not valid";
        }
    }
}
