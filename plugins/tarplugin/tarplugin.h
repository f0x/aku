 /*

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
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

        bool isWorkingProperly();

        void loadArchive();
        void init(const KUrl &fileName);

        void extractArchive(const KUrl &destination, const QStringList &files);

        QStringList additionalHeaderStrings();

    private:
        KTar *m_archive;
        QVector<QStringList> m_entries;
        QString m_currentPath;
        double size;
        int m_currentExtracting;
        int m_filesCount;

        void getEntries(const KArchiveEntry *rootEntry);
};

#endif 
