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

#include "rarplugin.h"

#include <QProcess>
#include <QDateTime>
#include <QDir>

#include <KLocale>
#include <KDebug>
#include <KStandardDirs>

AKU_PLUGIN_EXPORT(RarPlugin)

// the list of files in the rar archive starts after this line
const QString headerLine = "-------------------------------------------------------------------------------";

// name of the executable
QString exeName;

bool rarCreate = false;
bool rarRename = false;
bool rarDelete = false;
bool rarAddComment = false;
bool rarEncrypt = false;
bool rarLock = false;
bool rarAdd = false;

RarPlugin::RarPlugin(QObject *parent, const QVariantList &args) : AkuPlugin(parent)
{
    if (!KStandardDirs::findExe("rar").isEmpty()) {
        rarCreate = true;
        rarRename = true;
        rarDelete = true;
        rarAddComment = true;
        rarEncrypt = true;
        rarLock = true;
        rarAdd = true;
    }
}

RarPlugin::~RarPlugin()
{}

QStringList RarPlugin::mimeTypeNames()
{
    return QStringList() << "application/x-rar";
}

bool RarPlugin::canExtract()
{
    return true;
}

bool RarPlugin::canAdd()
{
    return rarAdd;
}

bool RarPlugin::canCreate()
{
    return rarCreate;
}

bool RarPlugin::canRename()
{
    return rarRename;
}

bool RarPlugin::canDelete()
{
    return rarDelete;
}

bool RarPlugin::canAddComment()
{
    return rarAddComment;
}

bool RarPlugin::canEncrypt()
{
    return rarEncrypt;
}

bool RarPlugin::canLock()
{
    return rarLock;
}

void RarPlugin::init(const KUrl &fileName)
{
    m_fileName = fileName;
}

void RarPlugin::loadArchive()
{

    QProcess process;
   
    QStringList options;
    options << "v" << m_fileName.pathOrUrl();
    process.start(exeName, options);
    process.waitForFinished();

    QString output;
    output = process.readAllStandardOutput();
   
    int indexOfHeaderLine;
    indexOfHeaderLine = output.indexOf(headerLine);
    
    // cut the the text until the end of headerLine
    output.remove(0, indexOfHeaderLine + 79);  

    // search for the second headerLine. The list of the file in the archive ends here
    indexOfHeaderLine = output.indexOf(headerLine);

    output.remove(indexOfHeaderLine, output.length());

    //kDebug() << output;
    //output.remove(0, 1);
    //output.remove(output.length() - 1, 1); //other parsing corrections

    QStringList splitList;
    splitList = output.split("\n"); // split at the newline

    splitList.removeFirst();
    splitList.removeLast();

    QVector<QStringList> archive;

    QStringList file;

#ifdef Q_WS_WIN
    // one more parsing correction on Win32
    // since appears to be a newline char issue
    for (int i = 0; i < splitList.count(); i++) {
        splitList[i] = splitList[i].remove(splitList[i].size() - 1, 1);
    }
#endif

    for (int i = 0; i < splitList.size(); i++) {
        if ( i % 2 == 0 ) {
            file << splitList[i].mid(1); // filepath
        } else {
            QStringList attributes = (splitList.at(i)).split(" ", QString::SkipEmptyParts);
            for (int j = 0; j < attributes.size(); j++) {
                //if ((j == 5) && (attributes[j].startsWith("d"))) {
                //    file[0].append(QDir().separator());;
                //}                

                if (j == 4) {
                    QDateTime modified(QDate::fromString(file[4], QString("dd-MM-yy")), 
                                       QTime::fromString(attributes[4], QString("hh:mm")));
                    modified = modified.addYears(100);
                    file[4] = KGlobal::locale()->formatDateTime(modified);
                    continue;
                }

                if (j == 0 || j == 1) {
                    file << KGlobal::locale()->formatByteSize(attributes[j].toDouble());
                    continue;
                }

                if (j > 3) {
                    file << attributes[j];
                    continue;
                }

                file << attributes[j];
            }

           archive << (QStringList() << file);
           file.clear();
       }
   }
 
//    kDebug() << archive;

    onArchiveLoaded(archive);
}

bool RarPlugin::isWorkingProperly()
{
    // WARNING: this checks for unrar or rar with no distinction
    // TODO: decide whether we should support also the shareware rar

    if (KStandardDirs::findExe("rar").isEmpty()) {
        if (!KStandardDirs::findExe("unrar").isEmpty()) {
            exeName = "unrar";
            return true;
        }
        return false;
    }
    else {
        exeName = "rar";
        return true;
    }
}

QStringList RarPlugin::additionalHeaderStrings()
{
    return QStringList() << i18n("Ratio") << i18n("Modified")
                         << i18n("Attributes") << i18n("CRC")
                         << i18n("Method") << i18n("Version");
}

void RarPlugin::extractArchive(const KUrl &destination, const QStringList &files)
{
//    Usage:     unrar <command> -<switch 1> -<switch N> <archive> <files...>
//               <@listfiles...> <path_to_extract\> 
    QProcess process;
    QStringList options;
    options = files;
    options.insert(options.size(), destination.pathOrUrl());
    options.insert(0, "x");
    options.insert(1, m_fileName.pathOrUrl());
    process.start(exeName, options);
    kDebug() << options;
    process.waitForFinished();
}

void RarPlugin::lock(const KUrl &fileName)
{  
    kDebug() << "locking archive";
    QProcess process;
    QStringList options;
    options.insert(0, "k");
    options.insert(1, m_fileName.pathOrUrl());
    process.start(exeName, options);
    kDebug() << options;
    process.waitForFinished();
}