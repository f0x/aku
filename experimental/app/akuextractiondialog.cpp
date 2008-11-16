/*
   Copyright (C) 2008 Francesco Grieco <fgrieco@gmail.com>
   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/ 
#include "akuextractiondialog.h"

#include <KLocale>
#include <KDebug>
#include <QDir>
#include <QHeaderView>
#include <kinputdialog.h>

AkuExtractionDialog::AkuExtractionDialog(QWidget *parent) : KDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    QWidget *widget = new QWidget(this);
    ui.setupUi(widget);
    setMainWidget(widget);

    dirView = new KFileTreeView(widget);
    dirView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    dirView->setDirOnlyMode(true);
    ui.horizontalLayout->insertWidget(0, dirView);
    dirView-> setColumnHidden (1, true);
    dirView-> setColumnHidden (3, true);
    dirView-> setColumnHidden (4, true);
    dirView-> setColumnHidden (5, true);
    dirView-> setColumnHidden (6, true);
    dirView->header()->setResizeMode(0, QHeaderView::ResizeToContents );
    dirView->resizeColumnToContents(0);
    dirView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    dirView->setCurrentUrl(KUrl(QDir::homePath()));
    
    setButtons(KDialog::Ok | KDialog::Cancel | KDialog::Default);
    setCaption(i18n("Extraction path and options"));

    connect(this, SIGNAL(okClicked()), this, SLOT(slotExtraction()));
    connect(dirView, SIGNAL(currentChanged(const KUrl &)), this, SLOT(updateCombo(const KUrl &)));
    connect(ui.buttonNewDir, SIGNAL(clicked()), this, SLOT(createNewDir()));
}

AkuExtractionDialog::~AkuExtractionDialog()
{}

void AkuExtractionDialog::slotExtraction()
{
    // TODO: check whether the url has write permissions for
    //       the current user or not.
    emit extractionClicked(dirView->selectedUrl());
}

void AkuExtractionDialog::updateCombo(const KUrl localPath)
{
    ui.comboHistoryBox->setEditText(localPath.path());
}

void AkuExtractionDialog::createNewDir()
{
    QString newDir;
    newDir = KInputDialog::getText(i18n("New Folder"), i18n("Enter a name for the new folder"),
                                   "test", 0 , this);
    
}
