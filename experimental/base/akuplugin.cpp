 /* This file is part of the KDE project

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
#include "akuplugin.h"
#include "akujobs.h"

#include <KDebug>
#include <KUrl>

#include <QStringList>
#include <QVector>

#include <QTimer>

Q_DECLARE_METATYPE(QVector<QStringList>)

class AkuPlugin::AkuPluginPrivate {

public:
    AkuPluginPrivate(AkuPlugin *q):
                     q(q),
                     timer(new QTimer(q))
    {}

    AkuPlugin *q;
    QTimer *timer;
    KUrl currentFile;
    CurrentOperation currentOp;
};

AkuPlugin::AkuPlugin(QObject *parent) : QObject(parent),
                                        d(new AkuPluginPrivate(this))
{
    qRegisterMetaType<QVector<QStringList> >();
    connect (this, SIGNAL(archiveLoaded(QVector<QStringList>)), this, SIGNAL(operationCompleted()));
    connect (this, SIGNAL(operationCompleted()), this, SLOT(completeOperations()));
}

AkuPlugin::~AkuPlugin()
{}

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

bool AkuPlugin::isWorkingProperly()
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

void AkuPlugin::load(const KUrl &fileName)
{
    if (d->currentFile != fileName) {
        init(fileName);
    }

    setCurrentOperation(Loading);

    connect(d->timer, SIGNAL(timeout()), this, SLOT(emitPercent()));
    d->timer->start(500);

    KJob *job = new AkuJobs::LoadJob(this, this);
    job->start();
}

void AkuPlugin::extract(const KUrl &fileName, const KUrl &destination, const QStringList &files)
{
    if (d->currentFile != fileName) {
        init(fileName);
    }

    setCurrentOperation(Extracting);

    KJob *job = new AkuJobs::ExtractJob(this, destination, files, this);
    connect(job, SIGNAL(finished(KJob *)), this, SIGNAL(notifyExtractionComplete()));
    connect(job, SIGNAL(finished(KJob *)), this, SIGNAL(operationCompleted()));

    connect(d->timer, SIGNAL(timeout()), this, SLOT(emitPercent()));
    d->timer->start(500);

    job->start();
}

void AkuPlugin::emitPercent()
{}

void AkuPlugin::completeOperations()
{
    d->timer->stop();
}

void AkuPlugin::init(const KUrl &fileName)
{
    Q_UNUSED(fileName)
}

void AkuPlugin::extractArchive(const KUrl &destination, const QStringList &files)
{
    Q_UNUSED(destination)
    Q_UNUSED(files)
}

AkuPlugin::CurrentOperation AkuPlugin::currentOperation()
{
    return d->currentOp;
}

void AkuPlugin::setCurrentOperation(CurrentOperation op)
{
    d->currentOp = op;
}
