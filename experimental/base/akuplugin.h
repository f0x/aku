 /* This file is part of the KDE project

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/

#ifndef AKUPLUGIN_H
#define AKUPLUGIN_H

#include <QObject>
#include <kdemacros.h>

/** \class AkuPlugin akuplugin.h
  * \brief Aku's Base Plugin Class for archive management
  *
  * This is the base class for every archive plugin in Aku.
  */

class KDE_EXPORT AkuPlugin : public QObject
{
    Q_OBJECT

    public:
        AkuPlugin(QObject *parent);
        virtual ~AkuPlugin();

        /**
         * Reimplement this to give a comment about the type of archive
         * handled. E.g. "Rar Archive", "Zip Archive"..
         */
        QString comment();

        /** 
         * Reimplement this to tell the app which suffix the plugin handles.
         * E.g. "zip", "rar", "tar", "tar.gz"
         */
        QString archiveSuffix();

    private:
        class AkuPluginPrivate;
        AkuPluginPrivate *d;

// TODO: add convenient methods to share in the main application in
//       order to correctly retrieve and show infos about the archive.
};

#endif 
