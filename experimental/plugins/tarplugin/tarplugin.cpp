 /*

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/

#include "tarplugin.h"

#include <KLocale>
#include <KUrl>
#include <KDebug>
#include <KGlobal>
#include <KTar>

#include <QDir>
#include <QDateTime>

AKU_PLUGIN_EXPORT(TarPlugin)

TarPlugin::TarPlugin(QObject *parent, const QVariantList &args) : AkuPlugin(parent)
{}

TarPlugin::~TarPlugin()
{}

QStringList TarPlugin::mimeTypeNames()
{
    return QStringList() << "application/x-bzip-compressed-tar" << "application/x-compressed-tar" << "application/x-tar";
}

bool TarPlugin::canExtract()
{
    return true;
}

bool TarPlugin::canCreate()
{
    return false;
}

bool TarPlugin::canRename()
{
    return true;
}

bool TarPlugin::canAdd()
{
    return false;
}

bool TarPlugin::canDelete()
{
    return true;
}

bool TarPlugin::isWorkingProperly()
{
    return true;
}

void TarPlugin::loadArchive(const KUrl &filename)
{

    m_archive = new KTar(filename.pathOrUrl());

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

void TarPlugin::getEntries(const KArchiveEntry *rootEntry)
{
    if (rootEntry->isFile()) {
        const KArchiveFile *fileEntry = static_cast<const KArchiveFile*>(rootEntry);

        m_entries << (QStringList() << m_currentPath + fileEntry->name()  // file name
                                    << KGlobal::locale()->formatByteSize(fileEntry->size()) // file size
                                    << QString() // compressed size WARNING: not supported by KTar
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

QStringList TarPlugin::additionalHeaderStrings()
{
    return QStringList() << i18n("Owner") << i18n("Group") << i18n("Permissions") << i18n("Creation Time");
}

QString TarPlugin::formatPermissions(mode_t permissions)
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
