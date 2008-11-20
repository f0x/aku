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
    emit operationCompleted();

}

ExtractJob::ExtractJob(AkuPlugin *plugin, const KUrl &destination, QStringList files, QObject *parent) :
                  AkuJob(parent), m_plugin(plugin), m_destination(destination), m_files(files)
{}

ExtractJob::~ExtractJob()
{}

void ExtractJob::doWork()
{
    m_plugin->extractArchive(m_destination, m_files);
    emit operationCompleted();
}

LockJob::LockJob(AkuPlugin *plugin, QObject *parent) :
                  AkuJob(parent), m_plugin(plugin)
{}

LockJob::~LockJob()
{}

void LockJob::doWork()
{
    m_plugin->lock();
    emit operationCompleted();
}

AkuThread::AkuThread(AkuJob *job) : m_job(job)
{
}

void AkuThread::run()
{
    m_job->doWork();
}


AkuHelper::AkuHelper(AkuPlugin *plugin) : QObject(plugin), m_plugin(plugin)
{}

AkuHelper::~AkuHelper()
{}

void AkuHelper::onError(const QString &error)
{
    emit this->error(error);
}

void AkuHelper::onArchiveLoaded(QVector<QStringList> data)
{
    emit archiveLoaded(data);
}

void AkuHelper::onProgressUpdate(double current, double total)
{
    emit progressUpdate(current, total);
}

};
