 /*

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
#include "akujobs.h"
#include "akuplugin.h"

#include <QTimer>
#include <QThread>

#include <KDebug>

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
//     QTimer *timer = new QTimer(QThread::currentThread());
//     connect(timer, SIGNAL(timeout()), m_plugin, SLOT(emitPercent()));
//     timer->start(500);
    m_plugin->loadArchive(m_fileName);
//     timer->stop();
}


AkuThread::AkuThread(LoadJob *job) : m_job(job)
{
}

void AkuThread::run()
{
    m_job->doWork();
}

};
