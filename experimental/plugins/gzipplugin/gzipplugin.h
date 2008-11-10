 /*

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
#ifndef GZIPPLUGIN_H
#define GZIPPLUGIN_H

#include <akuplugin.h>
#include <aku_macros.h>
#include <QVariantList>

class KUrl;

class GZipPlugin : public AkuPlugin
{
    Q_OBJECT
    public:
        GZipPlugin(QObject *parent, const QVariantList &args);
        ~GZipPlugin();

        QStringList mimeTypeNames();

        bool canExtract();
        bool canCreate();
        bool canRename();
        bool canDelete();
        bool canAdd();

        bool isWorkingProperly();

        void loadArchive(const KUrl &fileName);

        QStringList additionalHeaderStrings();
};

#endif 
