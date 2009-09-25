/***************************************************************************
 *   Copyright 2009 by Francesco Grieco <fgrieco@gmail.com>                *
 *                     Alessandro Diaferia <alediaferia@gmail.com>         *
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

#include "extractiondialog.h"
//#include "akutooltip.h"
//#include "akutooltipmanager.h"

#include <QDir>
#include <QHeaderView>

#include <kinputdialog.h>
#include <KIO/Job>
#include <KIcon>
#include <KMessageBox>
#include <KUrlCompletion>
#include <KLocale>
#include <KFileTreeView>
#include <KDebug>

ExtractionDialog::ExtractionDialog(QWidget *parent) : KDialog(parent),
                                                      m_wparent(0), m_awidget(0)
{
    setAttribute(Qt::WA_DeleteOnClose);

    QWidget *widget = new QWidget(this);
    ui.setupUi(widget);
    setMainWidget(widget);

    m_dirView = new KFileTreeView(widget);
    m_dirView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    m_dirView->setDirOnlyMode(true);
    ui.generalVerticalLayout->insertWidget(1, m_dirView);

    //m_dirView->setColumnHidden(1, true);
    //m_dirView->setColumnHidden(2, true);
    m_dirView->setColumnHidden(3, true);
    m_dirView->setColumnHidden(4, true);
    m_dirView->setColumnHidden(5, true);
    m_dirView->setColumnHidden(6, true);

    m_dirView->header()->setResizeMode(QHeaderView::Stretch);
    m_dirView->header()->setResizeMode(0, QHeaderView::ResizeToContents);
    m_dirView->resizeColumnToContents(2);

    m_dirView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_dirView->setCurrentUrl(KUrl(QDir::homePath()));

    const int minWidth = m_dirView->columnWidth(0) +
                         m_dirView->columnWidth(1) +
                         m_dirView->columnWidth(2);

//     kDebug() << minWidth << sizeHint().width();

    m_dirView->setMinimumSize(minWidth, 0);
    
    setButtons(KDialog::Ok | KDialog::Cancel | KDialog::Default);
    setCaption(i18n("Extraction path and options"));

    //ui.buttonNewDir->setIcon(KIcon("folder-new"));
    KUrlCompletion *urlCompletion = new KUrlCompletion(KUrlCompletion::DirCompletion);
    ui.comboHistoryBox->setCompletionObject(urlCompletion);
    ui.comboHistoryBox->setAutoDeleteCompletionObject(true);
    ui.comboHistoryBox->setCompletionMode(KGlobalSettings::CompletionPopupAuto);

    connect(this, SIGNAL(okClicked()), this, SLOT(slotExtraction()));
    connect(m_dirView, SIGNAL(currentChanged(const KUrl &)), this, SLOT(updateCombo(const KUrl &)));

    //AkuTooltip *tip = new AkuTooltip(this);
    //ui.verticalLayout->insertWidget(0, tip);
    //tip->setTooltip(i18n("The destination path will be created if necessary"));
    //AkuTooltipManager::instance()->showTip(tip);

    resize(400, height());
    //connect(ui.buttonNewDir, SIGNAL(clicked()), this, SLOT(createNewDir()));
}

ExtractionDialog::~ExtractionDialog()
{
    if (m_awidget) {
        m_awidget->setParent(static_cast<QWidget*>(m_wparent));
    }
}

void ExtractionDialog::slotExtraction()
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

void ExtractionDialog::updateCombo(const KUrl localPath)
{
    ui.comboHistoryBox->setEditUrl(localPath);
}

void ExtractionDialog::createNewDir()
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
    m_dirView->setCurrentUrl(dir);
}

void ExtractionDialog::setAdvancedWidget(QWidget *widget)
{
    if (!widget) {
        return;
    }

    m_wparent = widget->parent();
    m_awidget = widget;
    ui.advancedLayout->addWidget(widget);
}

#include "extractiondialog.moc"
