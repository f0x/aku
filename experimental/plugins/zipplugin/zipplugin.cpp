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
{}

ZipPlugin::~ZipPlugin()
{}

QString ZipPlugin::mimeTypeName()
{
    return "application/zip";
}

bool ZipPlugin::canExtract()
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

    // NOTE: here the errors are handled internally
    //       in the plugin. Think about this, whether
    //       to use them internally or globally by the main app.
    if (!m_archive->isOpen()) {
        emit error(i18n("An error occurred. Could not open archive <b>%1</b>").arg(m_archive->fileName()));
        return;
    }

    m_currentPath.clear();
    m_entries.clear();

    getEntries(m_archive->directory());

    m_archive->close();

    kDebug()<<m_entries;
    QVector<QStringList> archive;

    foreach (const QString &field, m_entries) {
        archive << (QStringList() << field);
    }

    emit archiveLoaded(archive);

}

void ZipPlugin::getEntries(const KArchiveEntry *rootEntry)
{
    if (rootEntry->isFile()) {
        m_entries << m_currentPath + rootEntry->name();
        kDebug() << m_entries.last();
        return;
    }

    if (rootEntry->name() != QDir::separator()) {
        m_currentPath.append(rootEntry->name());
    }

    // if we are here then the rootEntry is a dir.
    if (!m_currentPath.isEmpty()) {
        m_currentPath.append(QDir::separator());
    }

    const KArchiveDirectory *rootDir = static_cast<const KArchiveDirectory*>(rootEntry);

    foreach (const QString &entry, rootDir->entries()) {
        kDebug() << "root" << rootDir->name();
        kDebug() << rootDir->entry(entry)->name();
        getEntries(rootDir->entry(entry));

        if (rootEntry == m_archive->directory()) {
            m_currentPath.clear();
        }

        // TODO: one step missing, but almost working.
    }

}

