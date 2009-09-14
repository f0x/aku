 /*

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
#ifndef ZIPPLUGIN_H
#define ZIPPLUGIN_H

#include <akuplugin.h>
#include <aku_macros.h>
#include <QVariantList>
#include <QStringList>
#include <QVector>

#include "ui_zipconfig.h"

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

        bool isWorkingProperly();

        void loadArchive();
        void init(const KUrl &fileName);

        void extractArchive(const KUrl &destination, const QStringList &files);

        QStringList additionalHeaderStrings();

        QWidget* extractionWidget();


    private:
        Ui::ZipConfig ui;
        QWidget *m_widget;
        KZip *m_archive;
        QVector<QStringList> m_entries;
        QString m_currentPath;
        int m_currentExtracting;
        int m_filesCount;

        void getEntries(const KArchiveEntry *rootEntry);

};

#endif
