 /*

   Copyright (C) 2008 Francesco Grieco <fgrieco@gmail.com>
   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/ 

#ifndef SZIPPLUGIN_H
#define SZIPPLUGIN_H

#include <akuplugin.h>
#include <aku_macros.h>
#include <QVariantList>

#include <KUrl>

class SzipPlugin : public AkuPlugin
{
    Q_OBJECT
    public:
        SzipPlugin(QObject *parent, const QVariantList &args);
        ~SzipPlugin();

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

     private:
        KUrl m_fileName;

};

#endif
