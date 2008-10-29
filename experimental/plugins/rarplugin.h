 /* This file is part of the KDE project

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

class RarPlugin : public AkuPlugin
{
    Q_OBJECT
    public:
        RarPlugin(QObject *parent, const QVariantList &args);
        ~RarPlugin();

        QString mimeTypeName();
        bool canExtract();
        bool canCreate();
        bool canRename();
        bool canDelete();
};

#endif
