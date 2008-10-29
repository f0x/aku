 /* This file is part of the KDE project

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/ 

#include "rarplugin.h"

#include <QProcess>

#include <KLocale>
#include <KDebug>

AKU_PLUGIN_EXPORT(RarPlugin)

const QString EXE_NAME = "unrar";

RarPlugin::RarPlugin(QObject *parent, const QVariantList &args) : AkuPlugin(parent)
{}

RarPlugin::~RarPlugin()
{}

QString RarPlugin::mimeTypeName()
{
    return "application/x-rar";
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

void RarPlugin::loadArchive(const KUrl &fileName)
{
   QProcess *process = new QProcess();
   
   kDebug() << fileName;
   QStringList options;
   options << "v" + fileName.pathOrUrl();
   process -> start(EXE_NAME, options);
   process -> waitForFinished();
   QString output;
   output = process -> readAllStandardOutput();
}