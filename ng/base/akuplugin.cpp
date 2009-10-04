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

#include "akuplugin.h"
#include "akujobs.h"

#include <KDebug>
#include <KUrl>

Q_DECLARE_METATYPE(QVector<QStringList>)

class AkuPlugin::AkuPluginPrivate {

public:
    AkuPluginPrivate(AkuPlugin *q):
                     q(q),
                     helper(0),
                     currentOp(AkuPlugin::NoOperation)
    {}

    AkuPlugin *q;
    KUrl currentFile;
    AkuJobs::AkuHelper *helper;
    CurrentOperation currentOp;
};

AkuPlugin::AkuPlugin(QObject *parent) : QObject(parent),
                                        d(new AkuPluginPrivate(this))
{
    qRegisterMetaType<AkuData>("AkuData");
    //connect (this, SIGNAL(archiveLoaded(AkuData)), this, SIGNAL(operationCompleted()));
    //connect (this, SIGNAL(operationCompleted()), this, SLOT(completeOperations()));
}

AkuPlugin::~AkuPlugin()
{
    delete d;
}

QStringList AkuPlugin::mimeTypeNames()
{
    return QStringList();
}

bool AkuPlugin::canExtract()
{
    return false;
}

bool AkuPlugin::canCreate()
{
    return false;
}

bool AkuPlugin::canAdd()
{
    return false;
}

bool AkuPlugin::canRename()
{
    return false;
}

bool AkuPlugin::canDelete()
{
    return false;
}

bool AkuPlugin::canAddComment()
{
    return false;
}

bool AkuPlugin::canEncrypt()
{
    return false;
}

bool AkuPlugin::canLock()
{
    return false;
}

bool AkuPlugin::isInstalled()
{
    return false;
}

QStringList AkuPlugin::additionalHeaderStrings()
{
    return QStringList();
}

QWidget* AkuPlugin::extractionWidget()
{
    return 0;
}

void AkuPlugin::load(const KUrl &fileName, const QString &password)
{
    //if (d->currentFile != fileName) {
    init(fileName, password);
    //}

    setCurrentOperation(Loading);

    // TODO: see whether to delete previous one or not
    if (!d->helper) {
        d->helper = new AkuJobs::AkuHelper(this);
        connect (d->helper, SIGNAL(error(AkuPlugin::ErrorType, const QString &)), this,
             SIGNAL(error(AkuPlugin::ErrorType, const QString &)));
        connect (d->helper, SIGNAL(archiveLoaded(AkuData)), this, SIGNAL(archiveLoaded(AkuData)));
    }
    //connect (d->helper, SIGNAL(progressUpdate(double, double)), this, SIGNAL(progressUpdate(double, double)));

    KJob *job = new AkuJobs::LoadJob(this, this);
    connect(job, SIGNAL(operationCompleted()), this, SIGNAL(operationCompleted()));
    connect(job, SIGNAL(operationCompleted()), this, SLOT(updatePluginStatus()));

    job->start();
}

void AkuPlugin::extract(AkuExtractInfo extractInfo, AkuPlugin::ExtractionOptions extractingOptions)
{
    if (d->currentFile != extractInfo.fileName) {
        init(extractInfo.fileName);
    }

    setCurrentOperation(Extracting);

    if (!d->helper) {
        d->helper = new AkuJobs::AkuHelper(this);
        connect (d->helper, SIGNAL(error(AkuPlugin::ErrorType, const QString &)), this,
             SIGNAL(error(AkuPlugin::ErrorType, const QString &)));

    }
    //connect (d->helper, SIGNAL(archiveLoaded(AkuData)), this, SIGNAL(archiveLoaded(AkuData)));
    //connect (d->helper, SIGNAL(progressUpdate(double, double)), this, SIGNAL(progressUpdate(double, double)));

    KJob *job = new AkuJobs::ExtractJob(this, extractInfo, extractingOptions, this);
    connect(job, SIGNAL(operationCompleted()), this, SIGNAL(notifyExtractionComplete()));
    connect(job, SIGNAL(operationCompleted()), this, SIGNAL(operationCompleted()));

    job->start();
}

void AkuPlugin::lock(const KUrl &fileName)
{
    if (d->currentFile != fileName) {
        init(fileName);
    }

    setCurrentOperation(Locking);

    if (!d->helper) {
        d->helper = new AkuJobs::AkuHelper(this);
        connect (d->helper, SIGNAL(error(const QString &)), this, SIGNAL(error(const QString &)));
    }
    //connect (d->helper, SIGNAL(error(const QString &)), this, SIGNAL(error(const QString &)));

    KJob *job = new AkuJobs::LockJob(this, this);
    connect(job, SIGNAL(operationCompleted()), this, SIGNAL(operationCompleted()));

    job->start();
}

void AkuPlugin::init(const KUrl &fileName, const QString &password)
{
    Q_UNUSED(fileName)
    Q_UNUSED(password)
}

void AkuPlugin::extractArchive(const AkuExtractInfo &extractInfo, const AkuPlugin::ExtractionOptions &extractionOptions)
{
    Q_UNUSED(extractInfo)
    Q_UNUSED(extractionOptions)
}

void AkuPlugin::lockArchive()
{
}

AkuPlugin::CurrentOperation AkuPlugin::currentOperation()
{
    return d->currentOp;
}

void AkuPlugin::setCurrentOperation(CurrentOperation op)
{
    kDebug() << "setting operation";
    if (d->currentOp != op) {
        d->currentOp = op;
        kDebug() << "emitting state changed";
        emit stateChanged();
    }
}

void AkuPlugin::onError(ErrorType errorType, const QString &error)
{
    if (!d->helper) {
        return;
    }

    d->helper->onError(errorType, error);
}

void AkuPlugin::onArchiveLoaded(AkuData data)
{
    if (!d->helper) {
        return;
    }

    d->helper->onArchiveLoaded(data);
}

void AkuPlugin::onProgressUpdate(double current, double total)
{
    if (!d->helper) {
        return;
    }

    d->helper->onProgressUpdate(current, total);
}

void AkuPlugin::updatePluginStatus()
{
    setCurrentOperation(NoOperation);
}

void AkuPlugin::setAnswer(OverwriteAnswer, const QString &info)
{
}
