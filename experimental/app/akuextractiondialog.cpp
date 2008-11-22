/*
   Copyright (C) 2008 Francesco Grieco <fgrieco@gmail.com>
   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/ 
#include "akuextractiondialog.h"
#include "akutooltip.h"
#include "akutooltipmanager.h"

#include <KLocale>
#include <KDebug>
#include <QDir>
#include <QHeaderView>
#include <QCompleter>

#include <kinputdialog.h>
#include <KIO/Job>
#include <KIcon>
#include <KMessageBox>
#include <KDirModel>

AkuExtractionDialog::AkuExtractionDialog(QWidget *parent) : KDialog(parent),
                                                            m_wparent(0), m_awidget(0)
{
    setAttribute(Qt::WA_DeleteOnClose);

    QWidget *widget = new QWidget(this);
    ui.setupUi(widget);
    setMainWidget(widget);

    dirView = new KFileTreeView(widget);
    dirView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    dirView->setDirOnlyMode(true);
    ui.generalVerticalLayout->insertWidget(1, dirView);

    //dirView->setColumnHidden(1, true);
    //dirView->setColumnHidden(2, true);
    dirView->setColumnHidden(3, true);
    dirView->setColumnHidden(4, true);
    dirView->setColumnHidden(5, true);
    dirView->setColumnHidden(6, true);

    dirView->header()->setResizeMode(QHeaderView::Stretch);
    dirView->header()->setResizeMode(0, QHeaderView::ResizeToContents);
    dirView->resizeColumnToContents(2);

    dirView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    dirView->setCurrentUrl(KUrl(QDir::homePath()));

    const int minWidth = dirView->columnWidth(0) +
                         dirView->columnWidth(1) +
                         dirView->columnWidth(2);

//     kDebug() << minWidth << sizeHint().width();

    dirView->setMinimumSize(minWidth, 0);
    
    setButtons(KDialog::Ok | KDialog::Cancel | KDialog::Default);
    setCaption(i18n("Extraction path and options"));

    //ui.buttonNewDir->setIcon(KIcon("folder-new"));
    ui.comboHistoryBox->setCompleter(new QCompleter(new KDirModel));

    connect(this, SIGNAL(okClicked()), this, SLOT(slotExtraction()));
    connect(dirView, SIGNAL(currentChanged(const KUrl &)), this, SLOT(updateCombo(const KUrl &)));

    AkuTooltip *tip = new AkuTooltip(this);
    ui.verticalLayout->insertWidget(0, tip);
    tip->setTooltip(i18n("The destination path will be created if necessary"));
    AkuTooltipManager::instance()->showTip(tip);

    resize(400, height());
    //connect(ui.buttonNewDir, SIGNAL(clicked()), this, SLOT(createNewDir()));
}

AkuExtractionDialog::~AkuExtractionDialog()
{
    if (m_awidget) {
        m_awidget->setParent(static_cast<QWidget*>(m_wparent));
    }
}

void AkuExtractionDialog::slotExtraction()
{
    // TODO: check whether the url has write permissions for
    //       the current user or not.

    const KUrl url(ui.comboHistoryBox->currentText());
    if (!url.isValid()) {
        KMessageBox::error(this, i18n("The specified url is not valid. Cannot extract."), i18n("Invalid destination url"));
        return;
    }


    if (!QDir(url.pathOrUrl()).exists()) {     
        KIO::mkdir(url);
    }
    
    kDebug() << "extracting in" << url;
    emit extractionClicked(url);
}

void AkuExtractionDialog::updateCombo(const KUrl localPath)
{
    ui.comboHistoryBox->setEditUrl(localPath);
}

void AkuExtractionDialog::createNewDir()
{
    KUrl dir(ui.comboHistoryBox->currentText());
    if (!dir.isValid()) {
        return;
    }

    QString newDir;
    newDir = KInputDialog::getText(i18n("New Folder"), i18n("Enter a name for the new folder"),
                                   "New Folder", 0 , this);

    if (newDir.isEmpty()) {
        return;
    }

    dir.addPath(newDir);
    KIO::mkdir(dir);
    dirView->setCurrentUrl(dir);
}

void AkuExtractionDialog::setAdvancedWidget(QWidget *widget)
{
    if (!widget) {
        return;
    }

    m_wparent = widget->parent();
    m_awidget = widget;
    ui.advancedLayout->addWidget(widget);
}
