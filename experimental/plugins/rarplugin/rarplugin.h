 /*

   Copyright (C) 2008 Francesco Grieco <fgrieco@gmail.com>
   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/ 

#ifndef RARPLUGIN_H
#define RARPLUGIN_H

#include <akuplugin.h>
#include <aku_macros.h>
#include <QVariantList>

#include <KUrl>

class RarPlugin : public AkuPlugin
{
    Q_OBJECT
    public:
        RarPlugin(QObject *parent, const QVariantList &args);
        ~RarPlugin();

        QStringList mimeTypeNames();

        bool canExtract();
        bool canCreate();
        bool canRename();
        bool canDelete();
        bool canAdd();

        bool isWorkingProperly();

        void loadArchive();
        void init(const KUrl &fileName);

        QStringList additionalHeaderStrings();

        void extractArchive(const KUrl &destination, const QStringList &files);

    private:
        KUrl m_fileName;

};

#endif
