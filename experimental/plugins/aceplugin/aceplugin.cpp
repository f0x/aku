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
#include <QDir>

#include <KLocale>
#include <KDebug>
#include <KStandardDirs>

AKU_PLUGIN_EXPORT(AcePlugin)

// the list of files in the ace archive starts after this line

// on Windows with unace32:
#ifdef Q_WS_WIN
const QString headerLine = " Date    |Time |Packed     |Size     |Ratio|File";
#else
// on Linux:
const QString headerLine = "  Date    Time     Packed      Size  Ratio  File";
#endif

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
    return false;
}

bool AcePlugin::canCreate()
{
    return false;
}

bool AcePlugin::canLock()
{
    return false;
}

bool AcePlugin::canRename()
{
    return false;
}

bool AcePlugin::canEncrypt()
{
    return false;
}

bool AcePlugin::canAddComment()
{
    return false;
}

bool AcePlugin::canDelete()
{
    return false;
}

void AcePlugin::init(const KUrl &fileName)
{
    m_fileName = fileName;
}

void AcePlugin::loadArchive()
{

    QProcess *process = new QProcess();
   
    QStringList options;
    options << "l" << m_fileName.pathOrUrl();
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
        // while (filePath.at(filePath.size() - 1).isSpace()) {
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

        // NOTE: ace format
        
        file << filePath;
        file << KGlobal::locale()->formatByteSize(attributes[3].toDouble()); // size
        file << KGlobal::locale()->formatByteSize(attributes[2].toDouble()); // packed
        file << attributes[4];  // ratio

        QDateTime modified(QDate::fromString(attributes[0], QString("dd.MM.yy")), 
                                       QTime::fromString(attributes[1], QString("hh:mm")));
        file << KGlobal::locale()->formatDateTime(modified);

        archive << (QStringList() << file);
        file.clear();
    }

    onArchiveLoaded(archive);
}

bool AcePlugin::isWorkingProperly()
{
#ifdef Q_WS_WIN
    if (!KStandardDirs::findExe("unace32").isEmpty()) {
        exeName = "unace32";
        return true;
    }
#else
    if (!KStandardDirs::findExe("unace").isEmpty()) {
        exeName = "unace";
        return true;
    } 
#endif
    return false;
}

QStringList AcePlugin::additionalHeaderStrings()
{
    return QStringList() << i18n("Ratio") << i18n("Modified");
}

void AcePlugin::extractArchive(const KUrl &destination, const QStringList &files)
{
//  Usage:
//  UNACE <command> [-<sw1> ...] <archive> [<base_dir>\] [<files>/@<filelist>]

    QProcess process;
    QStringList options;
    options = files;
    options.insert(0, "x");
    options.insert(1, m_fileName.pathOrUrl());
    options.insert(options.size(), destination.pathOrUrl() + QDir().separator());
    process.start(exeName, options);
    kDebug() << exeName;
    kDebug() << options;
    process.waitForFinished();
}

