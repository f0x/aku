/***************************************************************************
 *   Copyright 2009 by Alessandro Diaferia <alediaferia@gmail.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#ifndef AKUJOBS_H
#define AKUJOBS_H

#include "akuplugin.h"

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
        ExtractJob(AkuPlugin *, const AkuExtractInfo &extractInfo, const AkuPlugin::ExtractionOptions &, QObject *parent = 0);
        ~ExtractJob();

        void doWork();

    private:
        AkuPlugin *m_plugin;
        AkuExtractInfo m_extractInfo;
        AkuPlugin::ExtractionOptions m_extractingOptions;
};

class LockJob : public AkuJob
{
    Q_OBJECT
    public:
        LockJob(AkuPlugin *, QObject *parent = 0);
        ~LockJob();

        void doWork();

    private:
        AkuPlugin *m_plugin;
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
        void archiveLoaded(AkuData);
        void progressUpdate(double, double);

    public slots:
        void onError(const QString &);
        void onArchiveLoaded(AkuData);
        void onProgressUpdate(double, double);
};
};

#endif
