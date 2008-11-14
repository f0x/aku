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

class AkuPlugin;

namespace AkuJobs
{
class LoadJob : public KJob
{
    Q_OBJECT
    public:
        LoadJob(const KUrl& fileName, AkuPlugin *, QObject *parent = 0);
        ~LoadJob();

        void start();
        void doWork();

    private:
        AkuPlugin *m_plugin;
        KUrl m_fileName;
};

class AkuThread : public ThreadWeaver::Job
{
    Q_OBJECT
    public:
        AkuThread(LoadJob*);

    private:
        void run();
        LoadJob *m_job;
};
};

#endif
