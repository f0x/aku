 /*

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/ 

#include "zipplugin.h"

#include <KZip>
#include <KLocale>
#include <KArchiveDirectory>
#include <KUrl>
#include <KDebug>

AKU_PLUGIN_EXPORT(ZipPlugin)

ZipPlugin::ZipPlugin(QObject *parent, const QVariantList &args) : AkuPlugin(parent)
{}

ZipPlugin::~ZipPlugin()
{}

QString ZipPlugin::mimeTypeName()
{
    return "application/zip";
}

bool ZipPlugin::canExtract()
{
    return true;
}

bool ZipPlugin::canCreate()
{
    return false;
}

bool ZipPlugin::canRename()
{
    return true;
}

bool ZipPlugin::canDelete()
{
    return true;
}

bool ZipPlugin::isWorkingProperly()
{
    return true;
}

void ZipPlugin::loadArchive(const KUrl &filename)
{
    // WARNING: to enable extraction/editing in general
    //          maybe archive should be made global.

    KZip archive = KZip(filename.pathOrUrl());

    archive.open(QIODevice::ReadOnly);

    // NOTE: here the errors are handled internally
    //       in the plugin. Think about this, whether
    //       to use them internally or globally by the main app.
    if (!archive.isOpen()) {
        emit error(i18n("An error occurred. Could not open archive <b>%1</b>").arg(archive.fileName()));
        return;
    }

//     kDebug()<<archive.directory()->entries();

    QStringList topLevelEntries;
    QStringList totalEntries;

    topLevelEntries = archive.directory()->entries();

    foreach (const QString &entry, topLevelEntries) {
        
    }
}

