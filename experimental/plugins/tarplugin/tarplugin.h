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

        bool isWorkingProperly();

        void loadArchive(const KUrl &fileName);

        QStringList additionalHeaderStrings();

    private:
        KTar *m_archive;
        QVector<QStringList> m_entries;
        QString m_currentPath;

        void getEntries(const KArchiveEntry *rootEntry);
        QString formatPermissions(mode_t permissions);
};

#endif 
