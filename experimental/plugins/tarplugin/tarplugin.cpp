 /*

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/

#include "tarplugin.h"

#include <KLocale>
#include <KUrl>
#include <KDebug>
#include <KGlobal>

#include <QFile>
#include <QDateTime>

AKU_PLUGIN_EXPORT(TarPlugin)

TarPlugin::TarPlugin(QObject *parent, const QVariantList &args) : AkuPlugin(parent)
{}

TarPlugin::~TarPlugin()
{}

QStringList TarPlugin::mimeTypeNames()
{
    return QStringList() << "application/x-bzip-compressed-tar" << "application/x-compressed-tar" << "application/x-tar";
}

bool TarPlugin::canExtract()
{
    return true;
}

bool TarPlugin::canCreate()
{
    return false;
}

bool TarPlugin::canRename()
{
    return true;
}

bool TarPlugin::canDelete()
{
    return true;
}

bool TarPlugin::isWorkingProperly()
{
    return true;
}

void TarPlugin::loadArchive(const KUrl &fileName)
{

}

QStringList TarPlugin::additionalHeaderStrings()
{
    return QStringList();
}
