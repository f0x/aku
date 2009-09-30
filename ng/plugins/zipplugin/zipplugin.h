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

#ifndef ZIPPLUGIN_H
#define ZIPPLUGIN_H

#include <akuplugin.h>
#include <aku_macros.h>

#include <QVariantList>
#include <QStringList>
#include <QVector>


class KZip;
class KUrl;
class KArchiveEntry;

class ZipPlugin : public AkuPlugin
{
    Q_OBJECT
    public:
        ZipPlugin(QObject *parent, const QVariantList &args);
        ~ZipPlugin();

        QStringList mimeTypeNames();

        bool canExtract();
        bool canCreate();
        bool canRename();
        bool canDelete();
        bool canAdd();

        bool isInstalled();

        void loadArchive();
        void init(const KUrl &fileName, const QString &password = 0);

        void extractArchive(const KUrl &destination, const QStringList &files);

        QStringList additionalHeaderStrings();

    private:
        KZip *m_archive;
        AkuData m_akudata;
        QString m_currentPath;
        int m_currentExtracting;
        int m_filesCount;

        void getEntries(const KArchiveEntry *rootEntry);

};

#endif
