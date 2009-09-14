 /*

   Copyright (C) 2008 Francesco Grieco <fgrieco@gmail.com>
   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/ 

#ifndef ACEPLUGIN_H
#define ACEPLUGIN_H

#include <akuplugin.h>
#include <aku_macros.h>
#include <QVariantList>

#include <KUrl>

class AcePlugin : public AkuPlugin
{
    Q_OBJECT
    public:
        AcePlugin(QObject *parent, const QVariantList &args);
        ~AcePlugin();

        QStringList mimeTypeNames();

        bool canExtract();
        bool canCreate();
        bool canRename();
        bool canDelete();
        bool canAdd();
        bool canEncrypt();
        bool canAddComment();
        bool canLock();

        bool isWorkingProperly();

        void loadArchive();
        void init(const KUrl &fileName);

        QStringList additionalHeaderStrings();

        void extractArchive(const KUrl &destination, const QStringList &files);

    private:
        KUrl m_fileName;

};

#endif
