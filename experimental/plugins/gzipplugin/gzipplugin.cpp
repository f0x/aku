 /*

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/

#include "gzipplugin.h"

#include <KLocale>
#include <KUrl>
#include <KDebug>

#include <QFile>

AKU_PLUGIN_EXPORT(GZipPlugin)

GZipPlugin::GZipPlugin(QObject *parent, const QVariantList &args) : AkuPlugin(parent)
{}

GZipPlugin::~GZipPlugin()
{}

QString GZipPlugin::mimeTypeName()
{
    return "application/x-gzip";
}

bool GZipPlugin::canExtract()
{
    return true;
}

bool GZipPlugin::canCreate()
{
    return false;
}

bool GZipPlugin::canRename()
{
    return true;
}

bool GZipPlugin::canDelete()
{
    return true;
}

bool GZipPlugin::isWorkingProperly()
{
    return true;
}

void GZipPlugin::loadArchive(const KUrl &fileName)
{
    QFile gzFile(fileName.pathOrUrl());

    if(!gzFile.open(QIODevice::ReadOnly)) {
        emit(i18n("Could not open file"));
    }

    char buffer[10];
    gzFile.read(buffer, 10);

    if ( (uint)buffer[0] != 31 && (uint)buffer[1] != 139) {
        emit(error(i18n("Not valid gzip format")));
        return;
    }

    uchar flag = (uchar)buffer[3];
    kDebug() << flag;

    if (!(flag & 8)) {
        emit (error(i18n("File name not set")));
        kDebug() << "file name not set";
        return;
    }

    if (!(flag & 4)) {
        kDebug() << "extra field not present";
    }

    QByteArray entryName;
    char c;

    // here we retrieve original file name
    while (c!='\0') {
        gzFile.read(&c, 1);
        entryName.append(c);
    }

    entryName.append('\0');

//    0 - FAT filesystem (MS-DOS, OS/2, NT/Win32)
//    1 - Amiga
//    2 - VMS (or OpenVMS)
//    3 - Unix
//    4 - VM/CMS
//    5 - Atari TOS
//    6 - HPFS filesystem (OS/2, NT)
//    7 - Macintosh
//    8 - Z-System
//    9 - CP/M
//   10 - TOPS-20
//   11 - NTFS filesystem (NT)
//   12 - QDOS
//   13 - Acorn RISCOS
//  255 - unknown
    uchar os = (uchar)buffer[9];
    QByteArray osType;

    switch (os) {
        case 0:
            osType.append(i18n("FAT filesystem (MS-DOS, OS/2, NT/Win32)"));
            break;
        case 1:
            osType.append(i18n("Amiga"));
            break;
        case 2:
            osType.append(i18n("VMS (or OpenVMS)"));
            break;
        case 3:
            osType.append(i18n("Unix"));
            break;
        case 4:
            osType.append(i18n("VM/CMS"));
            break;
        case 5:
            osType.append(i18n("Atari TOS"));
            break;
        case 6:
            osType.append(i18n("HPFS filesystem (OS/2, NT)"));
            break;
        case 7:
            osType.append(i18n("Machintosh"));
            break;
        case 8:
            osType.append(i18n("Z-System"));
            break;
        case 9:
            osType.append(i18n("CP/M"));
            break;
        case 10:
            osType.append(i18n("TOPS-20"));
            break;
        case 11:
            osType.append(i18n("NTFS filesystem (NT)"));
            break;
        case 12:
            osType.append(i18n("QDOS"));
            break;
        case 13:
            osType.append(i18n("Acorn RISCOS"));
            break;
        case 255:
            osType.append(i18n("Unknown"));
            break;
    }

    // last 8 bytes contain crc32 + size (4 + 4)
    gzFile.seek(gzFile.size() - 8);
    char endBlock[8];
    gzFile.read(endBlock, 8);
    gzFile.close();

    // size is 32 bit
    uint isize = (uint)endBlock[4] | (uint)endBlock[5] << 8 | (uint)endBlock[6] << 16 | (uint)endBlock[7] << 24;
    uint crc32 = (uint)endBlock[0] | (uint)endBlock[1] << 8 | (uint)endBlock[2] << 16 | (uint)endBlock[3] << 24;

    QStringList entry = QStringList() << entryName 
                                      << KLocale(QString()).formatByteSize(isize) 
                                      << QString() << osType 
                                      << QString::number(crc32);

    emit(archiveLoaded(QVector<QStringList>() << entry));
}

QStringList GZipPlugin::additionalHeaderStrings()
{
    return QStringList() << i18n("OS Type") << i18n("CRC32");
}
