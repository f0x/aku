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
class KUrl;

class KDE_EXPORT AkuPlugin : public QObject
{
    Q_OBJECT

    public:
        AkuPlugin(QObject *parent);
        virtual ~AkuPlugin();

        /**
         * Reimplement this to reimplement a correct mime type name.
         * Examples are: "application/x-rar" for .rar, 
         * "application/x-compressed-tar" for .tar.gz, "application/zip" for .zip.
         * Use this QString to build a KMimeType::Ptr.
         */
        virtual QString mimeTypeName();

        /**
         * The reimplementation should return true if the plugin supports
         * archive extraction.
         */
        virtual bool canExtract();

// TODO: add docs
        virtual bool canCreate();
        virtual bool canRename();
        virtual bool canDelete();

        /**
         * In the reimplementation of this function goes the real parsing process.
         * At the end of this function emit archiveLoaded() and pass
         * the correct data.
         */
        virtual void loadArchive(const KUrl &file);

        /**
         * Return true if the plugin has all needed resources ready and working
         * to properly load the archive otherwise the plugin will be marked as not working.
         */
        virtual bool isWorkingProperly();

    signals:
        /**
         * emit this signal when the data is ready to be used.
         * each QStringList is in the form of: FilePath:FileSize:PackedSize:Ratio ...
         * so each QStringList is a single file in the archive.
         */
        void archiveLoaded(const QVector<QStringList> &archive);

        /**
         * emit this signal when an error occurs. Pass the error
         * as QString so that it will be displayed by the main app.
         */
        void error(QString);

    private:
        class AkuPluginPrivate;
        AkuPluginPrivate *d;

// TODO: add convenient methods to share in the main application in
//       order to correctly retrieve and show infos about the archive.
};

#endif 
