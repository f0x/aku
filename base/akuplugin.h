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
        /**
         * Defines current operation. The state will automagically be set at the begin
         * of each operation. The state can be retrieved using currentOperation().
          */
        enum CurrentOperation { 
                                NoOperation,
                                Loading,
                                Extracting,
                                Adding,
                                Deleting,
                                Renaming,
                                AddingComment,
                                Encrypting,
                                Locking
                               };

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
        virtual bool canAddComment();
        virtual bool canLock();
        virtual bool canEncrypt();

        /**
         * In the reimplementation of this function goes the real parsing process.
         * At the end of this function emit archiveLoaded() and pass
         * the correct data. Note that the archive path is given in the init() method.
         */
        virtual void loadArchive() = 0;

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
         * @return the extraction option widget for the specific plugin or 0 if
         * there is no reimplementation.
         */
        virtual QWidget* extractionWidget();

        /**
         * This method should be reimplemented in order to allow archive extraction.
         * If @param files is empty then the whole archive should be extracted.
         */
        virtual void extractArchive(const KUrl &destination, const QStringList &files);

        /**
         */
        virtual void lockArchive();

        /**
         * Reimplement this method in order to perform standard initializing operations.
         * @note that operations such as loadArchive and extractArchive are performed
         * in an external thread so you might want to put in this function the initialization
         * of datas for every-time access. This method is called whenever a new archive needs
         * to be loaded.
         */
        virtual void init(const KUrl &fileName);

        /**
         * @internal used to load the archive in a threading way.
         */
        void load(const KUrl &fileName);

        /**
         * @internal used to extract the archive.
         */
        void extract(const KUrl &fileName, const KUrl &destination, const QStringList &files = QStringList());

        /**
         * @internal
         */
        void lock(const KUrl &fileName);

        /**
         * @return the operation in progress.
         */
        CurrentOperation currentOperation();

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
         * This signal is emitted after calling onError. Never emit this signal directly.
         */
        void error(const QString&);

        /**
         * this is emitted when the progress status changes.
         * @note never emit this signal directly, use onProgressUpdate instead
         * @param processed the current value.
         * @param total the total to reach.
         */
        void progressUpdate(double processed, double total);

         /**
          * @internal nobody should never care of this signal. 
          * It is emitted automatically at the end of each operation.
          */
         void operationCompleted();

         /**
          * @internal notifies the end of the extraction process.
          */
         void notifyExtractionComplete();

         /**
          * this signal is emitted each time the state changes. No need to 
          * manually emit this signal. The state can be retrieved using currentOperation().
          */
         void stateChanged();

    public slots:
        /**
         * reimplement this slot and perform here percentage calculation
         * of current progress. then call emit percent(uint processed, uint total);
         */
        void onProgressUpdate(double processed, double total);

        /**
         * Call this slot whenever an error occurs and pass the error as QString
         */
        void onError(const QString &);

        /**
         * Call this slot when the archive is correctly loaded.
         */
        void onArchiveLoaded(QVector<QStringList>);

    protected slots:
        /// @internal
        void completeOperations();

    protected:
        /// @internal
        void setCurrentOperation(CurrentOperation op);

    private:
        class AkuPluginPrivate;
        AkuPluginPrivate *d;

};

#endif 
