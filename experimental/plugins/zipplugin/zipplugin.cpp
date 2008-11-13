 /*

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/ 

#include "zipplugin.h"

#include <QDir>

#include <KZip>
#include <KLocale>
#include <KArchiveDirectory>
#include <KUrl>
#include <KDebug>

AKU_PLUGIN_EXPORT(ZipPlugin)

ZipPlugin::ZipPlugin(QObject *parent, const QVariantList &args) : AkuPlugin(parent)
{
    m_widget = new QWidget();
    ui.setupUi(m_widget);
}

ZipPlugin::~ZipPlugin()
{}

QStringList ZipPlugin::mimeTypeNames()
{
    return QStringList() << "application/zip";
}

bool ZipPlugin::canExtract()
{
    return true;
}

bool ZipPlugin::canAdd()
{
    return true;
}

bool ZipPlugin::canCreate()
{
    return false;
}

bool ZipPlugin::canRename()
{
    return true;
}

bool ZipPlugin::canDelete()
{
    return true;
}

bool ZipPlugin::isWorkingProperly()
{
    return true;
}

void ZipPlugin::loadArchive(const KUrl &filename)
{

    m_archive = new KZip(filename.pathOrUrl());

    m_archive->open(QIODevice::ReadOnly);

    if (!m_archive->isOpen()) {
        emit error(i18n("An error occurred. Could not open archive <b>%1</b>").arg(m_archive->fileName()));
        return;
    }

    m_currentPath.clear();
    m_entries.clear();

    getEntries(m_archive->directory());

    m_archive->close();


    emit archiveLoaded(m_entries);

}

void ZipPlugin::getEntries(const KArchiveEntry *rootEntry)
{
    if (rootEntry->isFile()) {
        const KZipFileEntry *fileEntry = static_cast<const KZipFileEntry*>(rootEntry);

        m_entries << (QStringList() << m_currentPath + fileEntry->name()  // file name
                                    << KGlobal::locale()->formatByteSize(fileEntry->size()) // file size
                                    << KGlobal::locale()->formatByteSize(fileEntry->compressedSize()) // compressed size
                                    << QString::number(fileEntry->crc32(), 16) // crc
                                    << QString::number(fileEntry->encoding()) // method
                                    << fileEntry->user() // owner
                                    << fileEntry->group() // group
                                    << formatPermissions(fileEntry->permissions()) // permissions
                                    << KGlobal::locale()->formatDateTime(fileEntry->datetime())
        );
        return;
    }

    if (rootEntry->name() != "/") { // even on win32 the rootEntry is "/"
        m_currentPath.append(rootEntry->name());
    }

    // if we are here then the rootEntry is a dir.
    if (!m_currentPath.isEmpty()) {
        m_currentPath.append(QDir::separator());
    }

    const KArchiveDirectory *rootDir = static_cast<const KArchiveDirectory*>(rootEntry);

    foreach (const QString &entry, rootDir->entries()) {
        getEntries(rootDir->entry(entry));
    }

    m_currentPath.remove(rootDir->name() + QDir::separator());

}

QStringList ZipPlugin::additionalHeaderStrings()
{
    return QStringList() << i18n("CRC") << i18n("Method") << i18n("Owner")
                         << i18n("Group") << i18n("Permissions") << i18n("CreationTime");
}

QString ZipPlugin::formatPermissions(mode_t permissions)
{
    QString pString = "---------"; // permission string

    // we should convert mode_t to human readable permissions

    // Owner permissions:
    // S_IRWXU = rwx
    // S_IRUSR = r
    // S_IWUSR = w
    // S_IXUSR = x

    // Group permissions
    // S_IRWXG = rwx
    // S_IRGRP = r
    // S_IWGRP = w
    // S_IXGRP = x

    // Others permissions
    // S_IRWXO = rwx
    // S_IROTH = r
    // S_IWOTH = w
    // S_IXOTH = x

    // owner
    uint own = permissions & S_IRWXU;
    if (own & S_IRUSR) {
        pString.replace(0, 1, "r");
    }
    if (own & S_IWUSR){
        pString.replace(1, 1, "w");
    }
    if (own & S_IXUSR) {
        pString.replace(2, 1, "x");
    }

    uint grp = permissions & S_IRWXG;
    if (grp& S_IRGRP) {
        pString.replace(3, 1, "r");
    }
    if (grp & S_IWGRP){
        pString.replace(4, 1, "w");
    }
    if (grp & S_IXGRP) {
        pString.replace(5, 1, "x");
    }

    uint oth = permissions & S_IRWXO;
    if (oth & S_IROTH) {
        pString.replace(6, 1, "r");
    }
    if (oth & S_IWOTH){
        pString.replace(7, 1, "w");
    }
    if (oth & S_IXOTH) {
        pString.replace(8, 1, "x");
    }

    
    return pString;

}

QWidget* ZipPlugin::configurationWidget()
{
    return m_widget;
}
