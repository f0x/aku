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

class KUrl;

class ZipPlugin : public AkuPlugin
{
    Q_OBJECT
    public:
        ZipPlugin(QObject *parent, const QVariantList &args);
        ~ZipPlugin();

        QString mimeTypeName();

        bool canExtract();
        bool canCreate();
        bool canRename();
        bool canDelete();

        bool isWorkingProperly();

        void loadArchive(const KUrl &fileName);

};

#endif
