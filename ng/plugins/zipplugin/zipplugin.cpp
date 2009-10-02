/***************************************************************************
 *   Copyright 2009 by Alessandro Diaferia <alediaferia@gmail.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#include "zipplugin.h"
#include "../karchiveutils/karchiveutils.h"

#include <QDir>

#include <KZip>
#include <KLocale>
#include <KArchiveDirectory>
#include <KUrl>
#include <KDebug>

AKU_PLUGIN_EXPORT(ZipPlugin)

ZipPlugin::ZipPlugin(QObject *parent, const QVariantList &args) : AkuPlugin(parent), m_archive(0)
{
}

ZipPlugin::~ZipPlugin()
{
    delete m_archive;
}

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

bool ZipPlugin::isInstalled()
{
    return true;
}

void ZipPlugin::init(const KUrl &fileName, const QString &password)
{
    m_archive = new KZip(fileName.pathOrUrl());
    Q_UNUSED(password)
}

void ZipPlugin::loadArchive()
{
    m_archive->open(QIODevice::ReadOnly);

    if (!m_archive->isOpen()) {
        //onError(i18n("An error occurred. Could not open archive <b>%1</b>", m_archive->fileName()));
        return;
    }

    m_currentPath.clear();
    m_akudata.paths.clear();

    getEntries(m_archive->directory());

    m_archive->close();


    onArchiveLoaded(m_akudata);

}

void ZipPlugin::getEntries(const KArchiveEntry *rootEntry)
{
    if (rootEntry->isFile()) {
        const KZipFileEntry *fileEntry = static_cast<const KZipFileEntry*>(rootEntry);

        m_akudata.paths << (QStringList() << m_currentPath + fileEntry->name()  // file name
                                    //<< KGlobal::locale()->formatByteSize(fileEntry->size()) // file size
                                    << QString::number(fileEntry->size())
                                    //<< KGlobal::locale()->formatByteSize(fileEntry->compressedSize()) // compressed size
                                    << QString::number(fileEntry->compressedSize())
                                    << QString::number(fileEntry->crc32(), 16) // crc
                                    << QString::number(fileEntry->encoding()) // method
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

QStringList ZipPlugin::additionalHeaderStrings()
{
    return QStringList() << i18n("CRC") << i18n("Method") << i18n("Owner")
                         << i18n("Group") << i18n("Permissions") << i18n("CreationTime");
}

void ZipPlugin::extractArchive(const KUrl &destination, const QStringList &files)
{
    if (!m_archive->isOpen() && !m_archive->open(QIODevice::ReadOnly)) {
        //onError(i18n("An error occurred. Could not open archive <b>%1</b>", m_archive->fileName()));
        return;
    }

    m_filesCount = files.count();
    KArchiveUtils::self()->extractArchive(m_archive, destination, files);
}
