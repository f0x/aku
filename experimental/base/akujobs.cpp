 /*

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
#include "akujobs.h"
#include "akuplugin.h"

#include <KDebug>


namespace AkuJobs
{

void AkuJob::start()
{
    AkuThread *thread = new AkuThread(this);
    ThreadWeaver::Weaver::instance()->enqueue(thread);
}

LoadJob::LoadJob(AkuPlugin *plugin, QObject *parent) :
                  AkuJob(parent), m_plugin(plugin)
{}

LoadJob::~LoadJob()
{}

void LoadJob::doWork()
{
    m_plugin->loadArchive();

}

ExtractJob::ExtractJob(AkuPlugin *plugin, const KUrl &destination, QStringList files, QObject *parent) :
                  AkuJob(parent), m_plugin(plugin), m_destination(destination), m_files(files)
{}

ExtractJob::~ExtractJob()
{}

void ExtractJob::doWork()
{
    m_plugin->extractArchive(m_destination, m_files);

}

AkuThread::AkuThread(AkuJob *job) : m_job(job)
{
}

void AkuThread::run()
{
    m_job->doWork();
}

};
