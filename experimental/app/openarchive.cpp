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
                           i18n("The correct plugin to open <b>%1</b> mimetype was found but appears to not be working properly. "
                                "Please check the installation", mimetype->name()), 
                           i18n("Unable to load archive"));
        }

    } else {
        KMessageBox::detailedSorry(static_cast<QWidget*>(parent()),
                           i18n("Sorry, no available plugin to open: <b>%1</b>.", url.pathOrUrl() ),
                           i18n("Install a plugin for <b>%1</b> mimetype in order to load the archive."
                           , mimetype->name()), 
                           i18n("Unable to load archive"));
    }
}

void OpenArchive::setAvailablePlugins(const QMap<QString, AkuPlugin*> &plugins)
{
    m_availablePlugins = plugins;
}
