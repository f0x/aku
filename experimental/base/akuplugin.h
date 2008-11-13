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
#include <aku_macros.h>

/** \class AkuPlugin akuplugin.h
  * \brief Aku's Base Plugin Class for archive management
  *
  * This is the base class for every archive plugin in Aku.
  */
class KUrl;


class AKU_EXPORT AkuPlugin : public QObject
{
    Q_OBJECT

    public:
        AkuPlugin(QObject *parent);
        virtual ~AkuPlugin();

        /**
         * Reimplement this to return a list of correct mime type names supported.
         * Examples are: "application/x-rar" for .rar, 
         * "application/x-compressed-tar" for .tar.gz, "application/zip" for .zip.
         */
        virtual QStringList mimeTypeNames();

        /**
         * The reimplementation should return true if the plugin supports
         * archive extraction.
         */
        virtual bool canExtract();

// TODO: add docs
        virtual bool canCreate();
        virtual bool canRename();
        virtual bool canDelete();
        virtual bool canAdd();

        /**
         * In the reimplementation of this function goes the real parsing process.
         * At the end of this function emit archiveLoaded() and pass
         * the correct data.
         */
        virtual void loadArchive(const KUrl &file) = 0;

        /**
         * Return true if the plugin has all needed resources ready and working
         * to properly load the archive otherwise the plugin will be marked as not working.
         */
        virtual bool isWorkingProperly();

        /**
         * Reimplement this if your plugin gives more parameter for each file 
         * in addition to the standard File Name, Size, Packed Size..
         */
        virtual QStringList additionalHeaderStrings();

        /**
         * The widget will be correctly embedded in the main application.
         * @return the configuration widget for the specific plugin or 0 if
         * there is no reimplementation.
         */
        virtual QWidget* configurationWidget();

        /**
         * This method should be reimplemented in order to allow archive extraction.
         * If @param files is empty then the whole archive should be extracted.
         */
//         virtual void extractArchive(const KUrl &destination, const QStringList &files) = 0;

        /**
         * @internal used to load the archive in a threading way.
         */
        void load(const KUrl &fileName);

        /**
         * @internal used to extract the archive.
         */
        void extract(const KUrl &destination, const QStringList &files);

    signals:
        /**
         * emit this signal when the data is ready to be used.
         * each QStringList is in the form of: FilePath:FileSize:PackedSize:Ratio ...
         * so each QStringList is a single file in the archive.
         * Note that the first three arguments of each QStringList should be in order:
         * File Path, File Size, Packed Size.
         */
        void archiveLoaded(const QVector<QStringList> &archive);

        /**
         * emit this signal when an error occurs. Pass the error
         * as QString so that it will be displayed by the main app.
         */
        void error(const QString&);

    private:
        class AkuPluginPrivate;
        AkuPluginPrivate *d;

};

#endif 
