 /*

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
#ifndef AKUJOBS_H
#define AKUJOBS_H

#include <KJob>
#include <KUrl>

#include <ThreadWeaver/Job>
#include <ThreadWeaver/Weaver>

#include <QStringList>
#include <QVector>

class AkuPlugin;

namespace AkuJobs
{

class AkuJob : public KJob
{
    Q_OBJECT
    public:
        AkuJob(QObject *parent = 0) : KJob(parent)
        {}
        virtual ~AkuJob() {}

        void start();
        virtual void doWork() = 0;

    signals:
        void operationCompleted();
};


class LoadJob : public AkuJob
{
    Q_OBJECT
    public:
        LoadJob(AkuPlugin *, QObject *parent = 0);
        ~LoadJob();

        void doWork();

    private:
        AkuPlugin *m_plugin;
};

class ExtractJob : public AkuJob
{
    Q_OBJECT
    public:
        ExtractJob(AkuPlugin *, const KUrl &destination, QStringList files = QStringList(), QObject *parent = 0);
        ~ExtractJob();

        void doWork();

    private:
        AkuPlugin *m_plugin;
        KUrl m_destination;
        QStringList m_files;
};

class AkuThread : public ThreadWeaver::Job
{
    Q_OBJECT
    public:
        AkuThread(AkuJob*);

    private:
        void run();
        AkuJob *m_job;
};

class AkuHelper : public QObject
{
    Q_OBJECT
    public:
        AkuHelper(AkuPlugin *);
        ~AkuHelper();

    private:
        AkuPlugin *m_plugin;

    signals:
        void error(const QString &);
        void archiveLoaded(QVector<QStringList>);
        void progressUpdate(double, double);

    public slots:
        void onError(const QString &);
        void onArchiveLoaded(QVector<QStringList>);
        void onProgressUpdate(double, double);
};
};

#endif
