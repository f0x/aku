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
// #include <QFile>

#include <KLocale>
#include <KDebug>
#include <KStandardDirs>

AKU_PLUGIN_EXPORT(RarPlugin)

// the list of files in the rar archive starts after this line
const QString headerLine = "-------------------------------------------------------------------------------";

// name of the executable
QString exeName;


RarPlugin::RarPlugin(QObject *parent, const QVariantList &args) : AkuPlugin(parent)
{}

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
    process->start(exeName, options);
    process->waitForFinished();

    QString output;
    output = process->readAllStandardOutput();
   
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

    for (int i = 0; i < splitList.size(); i++) {
        if ( i % 2 == 0 ) {
            file << splitList[i].mid(1); // filepath
        } else {
            QStringList attributes = (splitList.at(i)).split(" ", QString::SkipEmptyParts);
            for (int j = 0; j < attributes.size(); j++) {

                if (j == 4) {
                    QDateTime modified(QDate::fromString(file[4], QString("dd-MM-yy")), 
                                       QTime::fromString(attributes[4], QString("hh:mm")));
                    file[4] = modified.toString("dd-MM-yy hh:MM");
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

    emit archiveLoaded(archive);
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
                         << i18n("Method") << i18n("Version") << i18n("Mimetype");
}