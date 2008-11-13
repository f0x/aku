 /*

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
#include "akujobs.h"
#include "akuplugin.h"

namespace AkuJobs
{

LoadJob::LoadJob(const KUrl &fileName, AkuPlugin *plugin, QObject *parent) :
                  KJob(parent), m_plugin(plugin),  m_fileName(fileName)
{}

LoadJob::~LoadJob()
{}

void LoadJob::start()
{
    AkuThread *thread = new AkuThread(this);
    ThreadWeaver::Weaver::instance()->enqueue(thread);
}

void LoadJob::doWork()
{
    m_plugin->loadArchive(m_fileName);
}


AkuThread::AkuThread(LoadJob *job) : m_job(job)
{
}

void AkuThread::run()
{
    m_job->doWork();
}

};
