 /* This file is part of the KDE project

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/ 

#include "rarplugin.h"
#include <KLocale>

AKU_PLUGIN_EXPORT(RarPlugin)

RarPlugin::RarPlugin(QObject *parent, const QVariantList &args) : AkuPlugin(parent)
{}

RarPlugin::~RarPlugin()
{}

QString RarPlugin::comment()
{
    return i18n("Rar Archive");
}

QString RarPlugin::archiveSuffix()
{
    return "rar";
}

bool RarPlugin::canExtract()
{
    return true;
}

bool RarPlugin::canCreate()
{
    return true;
}

bool RarPlugin::canRename()
{
    return true;
}

bool RarPlugin::canDelete()
{
    return true;
}
