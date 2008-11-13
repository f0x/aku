 /*

   Copyright (C) 2008 Francesco Grieco <fgrieco@gmail.com>
   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/ 

// IMPORTANT NOTE: Keep using kdelibs coding style
//                 have a look at: http://techbase.kde.org/Policies/Kdelibs_Coding_Style

#include "aceplugin.h"

#include <QProcess>
#include <QDateTime>
// #include <QFile>

#include <KLocale>
#include <KDebug>
#include <KStandardDirs>

AKU_PLUGIN_EXPORT(AcePlugin)

// the list of files in the ace archive starts after this line
const QString headerLine = "  Date    Time     Packed      Size  Ratio  File";

// name of the executable
QString exeName;


AcePlugin::AcePlugin(QObject *parent, const QVariantList &args) : AkuPlugin(parent)
{}

AcePlugin::~AcePlugin()
{}

QStringList AcePlugin::mimeTypeNames()
{
    return QStringList() << "application/x-ace";
}

bool AcePlugin::canExtract()
{
    return true;
}

bool AcePlugin::canAdd()
{
    return true;
}

bool AcePlugin::canCreate()
{
    return false;
}

bool AcePlugin::canRename()
{
    return true;
}

bool AcePlugin::canDelete()
{
    return true;
}

void AcePlugin::loadArchive(const KUrl &fileName)
{

    QProcess *process = new QProcess();
   
    QStringList options;
    options << "l" << fileName.pathOrUrl();
    process->start(exeName, options);
    process->waitForFinished();

    QString output;
    output = process->readAllStandardOutput();
   
    int indexOfHeaderLine;
    indexOfHeaderLine = output.indexOf(headerLine);
    
    output.remove(0, indexOfHeaderLine);

    QStringList splitList;
    splitList = output.split("\n"); // split at the newline

    // we have to remove the first two and the last three strings from the list
    // the last string contains the general information of the archive

    splitList.removeFirst();
    splitList.removeFirst();
    splitList.removeLast();
    splitList.removeLast();
    splitList.removeLast();

    kDebug() << splitList;

    QVector<QStringList> archive;
    QString filePath;
    QStringList attributes;
    QStringList file;

    for (int i = 0; i < splitList.size(); i++) {      
        // a pathname starts at char 44. We can use the split string function with whitespace to take it,
        // but we lose filename that starts with a whitespace.
        filePath = splitList[i].mid(44);
        // we have to truncate the pathname because the path is followed by many whitespaces
        //while (filePath.at(filePath.size() - 1).isSpace()) {
        //   filePath.truncate(filePath.size() - 1);
        //}
        for (int j = filePath.size() - 1; j >= 0; j--) {
            if (filePath.at(j).isSpace()) {
               continue;
            }
            filePath.truncate(j + 1);
            break;
        }
        
        splitList[i].truncate(43);
        attributes = splitList[i].split(QRegExp("\\s+"), QString::SkipEmptyParts);
        
        file << filePath;
        file << attributes[3];  // size
        file << attributes[2];  // packed
        file << attributes[4];  // ratio

        QDateTime modified(QDate::fromString(attributes[0], QString("dd.MM.yy")), 
                                       QTime::fromString(attributes[1], QString("hh:mm")));
        file << KGlobal::locale()->formatDateTime(modified);

        archive << (QStringList() << file);
        file.clear();
    }

    emit archiveLoaded(archive);
}

bool AcePlugin::isWorkingProperly()
{
    if (!KStandardDirs::findExe("unace").isEmpty()) {
        exeName = "unace";
        return true;
    }
    return true;
}

QStringList AcePlugin::additionalHeaderStrings()
{
    return QStringList() << i18n("Ratio") << i18n("Modified");
}