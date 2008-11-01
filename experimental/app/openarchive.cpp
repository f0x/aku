 /* 
   Copyright (C) 2008 Francesco Grieco <fgrieco@gmail.com>
   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/ 
#include "openarchive.h"

#include <akuplugin.h>

#include <KFileItem>
#include <KDebug>
#include <KMessageBox>
#include <KLocale>

#include <QMap>

OpenArchive::OpenArchive(QWidget *parent) : QObject(parent)
{
}

OpenArchive::~OpenArchive()
{
}

void OpenArchive::load(KUrl url)
{
    if (!KFileItem(KFileItem::Unknown, KFileItem::Unknown, url).isReadable()) {
        return;
    }

    mimetype = KMimeType::findByUrl(url);
    kDebug() << mimetype -> name();

    if (m_availablePlugins.contains(mimetype->name())) {
        if (m_availablePlugins[mimetype->name()]->isWorkingProperly()) {
            m_availablePlugins[mimetype->name()]->loadArchive(url);
        } else {
            KMessageBox::sorry(static_cast<QWidget*>(parent()),
                           i18n("The correct plugin to open %1 mimetype was found but appears to not be working properly. "
                                "Please check the installation", "<b>"+mimetype->name()+"</b>"), 
                           i18n("Unable to load archive"));
        }
    } else {
        KMessageBox::sorry(static_cast<QWidget*>(parent()),
                           i18n("Sorry, no available plugin to open: %2. Install a plugin for %1 mimetype in order to load the archive."
                           , "<b>"+mimetype->name()+"</b>", "<b>"+url.pathOrUrl()+"</b>"), 
                           i18n("Unable to load archive"));
    }
}

void OpenArchive::buildTree()
{
}

void OpenArchive::setAvailablePlugins(const QMap<QString, AkuPlugin*> &plugins)
{
    m_availablePlugins = plugins;
}
