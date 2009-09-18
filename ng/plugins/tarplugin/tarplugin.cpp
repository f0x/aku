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

TarPlugin::TarPlugin(QObject *parent, const QVariantList &args) : AkuPlugin(parent),
                                                                  size(0), m_currentExtracting(0)
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

bool TarPlugin::isInstalled()
{
    return true;
}

void TarPlugin::init(const KUrl &fileName)
{
    m_archive = new KTar(fileName.pathOrUrl());

    QFile file(fileName.pathOrUrl());

    size = (double)(file.size() * 1.7); // NOTE: absolutely arbitrary calculation of generic gzip compression ratio
}

void TarPlugin::loadArchive()
{
    if (!m_archive->isOpen() && !m_archive->open(QIODevice::ReadOnly)) {
        onError(i18n("An error occurred. Could not open archive <b>%1</b>", m_archive->fileName()));
        return;
    }

    m_currentPath.clear();
    m_entries.clear();

    getEntries(m_archive->directory());

    m_archive->close();

    onArchiveLoaded(m_entries);

    m_entries.clear();

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
                                    << KArchiveUtils::self()->formatPermissions(fileEntry->permissions()) // permissions
                                    << KGlobal::locale()->formatDateTime(fileEntry->datetime())
        );
        return;
    }

    if (rootEntry->name() != "/") { // even on win32 the rootEntry is "/"
        m_currentPath.append(rootEntry->name() + QDir::separator());
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

void TarPlugin::extractArchive(const KUrl &destination, const QStringList &files)
{
    if (!m_archive->isOpen() && !m_archive->open(QIODevice::ReadOnly)) {
        onError(i18n("An error occurred. Could not open archive <b>%1</b>", m_archive->fileName()));
        return;
    }

    m_filesCount = files.count();

//     KArchiveUtils::self()->extractArchive(m_archive, destination, files);

    const KArchiveDirectory *mainDir = static_cast<const KArchiveDirectory*>(m_archive->directory());

    if (files.isEmpty()) {
        mainDir->copyTo(destination.pathOrUrl());
        return;
    }

    m_currentExtracting = 0;

    foreach (const QString &file, files) {
        const KArchiveEntry *entry = static_cast<const KArchiveEntry*>(mainDir->entry(file));

        if (!entry) {
            continue;
        }

        if (entry->isFile()) {
            static_cast<const KArchiveFile*>(entry)->copyTo(destination.pathOrUrl());
            continue;
        }
        static_cast<const KArchiveDirectory*>(entry)->copyTo(destination.pathOrUrl());

        m_currentExtracting++;
        onProgressUpdate(m_currentExtracting, m_filesCount);
    }
}
