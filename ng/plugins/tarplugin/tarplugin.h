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

#ifndef TARPLUGIN_H
#define TARPLUGIN_H

#include <akuplugin.h>
#include <aku_macros.h>

#include <QVariantList>
#include <QVector>

#include "../karchiveutils/karchiveutils.h"

class KUrl;
class KTar;
class KArchiveEntry;

class TarPlugin : public AkuPlugin
{
    Q_OBJECT
    public:
        TarPlugin(QObject *parent, const QVariantList &args);
        ~TarPlugin();

        QStringList mimeTypeNames();

        bool canExtract();
        bool canCreate();
        bool canRename();
        bool canDelete();
        bool canAdd();

        bool isInstalled();

        void loadArchive();
        void init(const KUrl &fileName);

        void extractArchive(const KUrl &destination, const QStringList &files);

        QStringList additionalHeaderStrings();

    private:
        KTar *m_archive;
        AkuData m_akudata;
        QString m_currentPath;
        double size;
        int m_currentExtracting;
        int m_filesCount;

        void getEntries(const KArchiveEntry *rootEntry);
};

#endif 
