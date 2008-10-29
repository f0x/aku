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

// name of the executable
const QString EXE_NAME = "unrar";
// the list of files in the rar archive starts after this line
const QString headerLine = "-------------------------------------------------------------------------------";

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
    return false;
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
   
   QStringList options;
   options << "v" << fileName.pathOrUrl();
   process -> start(EXE_NAME, options);
   process -> waitForFinished();
   QString output;
   output = process -> readAllStandardOutput();
   kDebug() << output;
   
   int indexOfHeaderLine;
   indexOfHeaderLine = output.indexOf(headerLine);
   
   // cut the the text until the end of headerLine
   output.remove (0, indexOfHeaderLine + 79); 
   // search for the second headerLine. The list of the file in the archive ends here
   indexOfHeaderLine = output.indexOf(headerLine);
   output.remove (indexOfHeaderLine, output.length());
   output.remove (0, 1);
   output.remove (output.length() - 1, 1); //others parsing corrections
   QStringList splitList;
   splitList = output.split ("\n"); // split at the newline
   QStringList fileList;
   for (int i = 0; i < splitList.size(); i++) {
     if ( i % 2 == 0 ) {
       fileList << splitList[i].mid(1);
     }
   }
   kDebug() << fileList;
}