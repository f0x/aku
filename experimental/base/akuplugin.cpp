 /* This file is part of the KDE project

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
#include "akuplugin.h"

class AkuPlugin::AkuPluginPrivate {

public:
    AkuPluginPrivate(AkuPlugin *q):
                     q(q)
                     {}

    AkuPlugin *q;
};

AkuPlugin::AkuPlugin(QObject *parent) : QObject(parent),
                                        d(new AkuPluginPrivate(this))
{}

AkuPlugin::~AkuPlugin()
{}

QString AkuPlugin::comment()
{
    return QString();
}

QString AkuPlugin::archiveSuffix()
{
    return QString();
}

bool AkuPlugin::canExtract()
{
    return false;
}

bool AkuPlugin::canCreate()
{
    return false;
}

bool AkuPlugin::canRename()
{
    return false;
}

bool AkuPlugin::canDelete()
{
    return false;
}
