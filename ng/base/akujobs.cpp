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

#include "akujobs.h"

#include <KDebug>


namespace AkuJobs
{

void AkuJob::start()
{
    AkuThread *thread = new AkuThread(this);
    connect(thread, SIGNAL(done(ThreadWeaver::Job*)), this, SIGNAL(operationCompleted()));
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

ExtractJob::ExtractJob(AkuPlugin *plugin, const AkuExtractInfo &extractInfo,
                       const AkuPlugin::ExtractionOptions &extractingOptions, QObject *parent) :
                       AkuJob(parent), m_plugin(plugin), m_extractInfo(extractInfo), m_extractingOptions(extractingOptions)
{}

ExtractJob::~ExtractJob()
{}

void ExtractJob::doWork()
{
    m_plugin->extractArchive(m_extractInfo, m_extractingOptions);
    //emit operationCompleted();
}

AddJob::AddJob(AkuPlugin *plugin, const QStringList &files,
                       const QString &path, QObject *parent) :
                       AkuJob(parent), m_plugin(plugin), m_files(files), m_path(path)
{}

AddJob::~AddJob()
{}

void AddJob::doWork()
{
    m_plugin->addToArchive(m_files, m_path);
}

RemoveJob::RemoveJob(AkuPlugin *plugin, const QStringList &files, QObject *parent) :
                       AkuJob(parent), m_plugin(plugin), m_files(files)
{}

RemoveJob::~RemoveJob()
{}

void RemoveJob::doWork()
{
    m_plugin->removeFromArchive(m_files);
}

RenameJob::RenameJob(AkuPlugin *plugin, const QString &file, QObject *parent) :
                       AkuJob(parent), m_plugin(plugin), m_file(file)
{}

RenameJob::~RenameJob()
{}

void RenameJob::doWork()
{
    m_plugin->renameFile(m_file);
}


LockJob::LockJob(AkuPlugin *plugin, QObject *parent) :
                  AkuJob(parent), m_plugin(plugin)
{}

LockJob::~LockJob()
{}

void LockJob::doWork()
{
    m_plugin->lockArchive();
    //emit operationCompleted();
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

void AkuHelper::onError(AkuPlugin::ErrorType errorType, const QString &error)
{
    emit this->error(errorType, error);
}

void AkuHelper::onArchiveLoaded(AkuData data)
{
    emit archiveLoaded(data);
}

void AkuHelper::onProgressUpdate(double current, double total)
{
    emit progressUpdate(current, total);
}

};
