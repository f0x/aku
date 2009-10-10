/***************************************************************************
 *   Copyright 2009 by Francesco Grieco <fgrieco@gmail.com>                *
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

#include "akustatusbar.h"

#include <QLabel>
#include <QToolButton>
#include <QHBoxLayout>

#include <KIcon>
#include <KIconLoader>
#include <KLocale>
#include <KDebug>

AkuStatusBar::AkuStatusBar(QWidget *parent) : KStatusBar(parent)
{
    m_busyButton = new QToolButton;
    m_busyButton->setIcon(KIcon("user-away"));
    m_busyButton->setAutoRaise(true);
    m_busyButton->setToolTip(i18n("Abort the operation"));
    m_busyButton->setVisible(false);
    addWidget(m_busyButton);

    m_statusLabel = new QLabel;
    addWidget(m_statusLabel);

    m_statusWidget = new QWidget;
    addPermanentWidget(m_statusWidget);

    QHBoxLayout *layout = new QHBoxLayout(m_statusWidget);
    m_commentButton = new QToolButton;
    m_commentButton->setIcon(KIcon("edit-paste"));
    m_commentButton->setToolTip(i18n("The archive has a comment. Click to read or to modify it"));
    m_commentButton->setAutoRaise(true);
    m_commentButton->setVisible(false);

    m_statusOkLabel = new QLabel;
    m_statusOkLabel->setPixmap(KIconLoader::global()->loadMimeTypeIcon("user-online", KIconLoader::Desktop,
                                                                     KIconLoader::SizeSmall));
    m_statusOkLabel->setToolTip(i18n("This archive has no global restrictions"));
    m_statusOkLabel->setVisible(false);

    m_lockLabel = new QLabel;
    m_lockLabel->setPixmap(KIconLoader::global()->loadMimeTypeIcon("object-locked", KIconLoader::Desktop,
                                                                 KIconLoader::SizeSmall));
    m_lockLabel->setToolTip(i18n("Locked archive. Any attempt to modify the archive will be ignored"));
    m_lockLabel->setVisible(false);

    m_headerLabel = new QLabel;
    m_headerLabel->setPixmap(KIconLoader::global()->loadMimeTypeIcon("security-low", KIconLoader::Desktop,
                                                                   KIconLoader::SizeSmall));
    m_headerLabel->setToolTip(i18n("This archive has a header password protection.") + "<br>" +
                            i18n("File data, file names, sizes, attributes, comments are encrypted.") +
                            "<br>" + i18n("Without a password it is impossible to view even the list of files in archive"));
    m_headerLabel->setVisible(false);

    m_passwordLabel = new QLabel;
    m_passwordLabel->setPixmap(KIconLoader::global()->loadMimeTypeIcon("dialog-password", KIconLoader::Desktop,
                                                                     KIconLoader::SizeSmall));
    m_passwordLabel->setToolTip("This archive has one or more password protected file(s)");
    m_passwordLabel->setVisible(false);

    m_errorButton = new QToolButton;
    m_errorButton->setIcon(KIcon("dialog-warning"));
    m_errorButton->setAutoRaise(true);
    m_errorButton->setToolTip(i18n("Error occured"));
    m_errorButton->setVisible(false);

    layout->addWidget(m_errorButton);
    layout->addWidget(m_commentButton);
    layout->addWidget(m_lockLabel);
    layout->addWidget(m_headerLabel);
    layout->addWidget(m_passwordLabel);
    layout->addWidget(m_statusOkLabel);

    setupConnections();
}

AkuStatusBar::~AkuStatusBar()
{
}

void AkuStatusBar::setupConnections()
{

}

void AkuStatusBar::stateChanged(AkuPlugin *plugin)
{
    m_plugin = plugin;

    switch (plugin->currentOperation()) {
        case AkuPlugin::Extracting :
            m_busyButton->setVisible(true);
            m_statusLabel->setText(i18n("Extraction in progress..."));
            break;
        case AkuPlugin::Loading :
            m_busyButton->setVisible(true);
            m_statusLabel->setText(i18n("Loading archive..."));
            break;
        case AkuPlugin::Locking :
            m_busyButton->setVisible(true);
            m_statusLabel->setText(i18n("Locking archive..."));
            break;
        case AkuPlugin::Adding :
            m_busyButton->setVisible(true);
            m_statusLabel->setText(i18n("Adding to the archive..."));
            break;
        case AkuPlugin::Deleting :
            m_busyButton->setVisible(true);
            m_statusLabel->setText(i18n("Removing file(s)..."));
            break;
        default: ;
            m_statusLabel->clear();
            m_busyButton->setVisible(false);
    }
}

void AkuStatusBar::operationCompleted()
{
    //kDebug() << "Operation Completed";
    m_statusLabel->clear();
}

void AkuStatusBar::abortOperation()
{
    //m_plugin->abortJob();
}

void AkuStatusBar::archiveInformation(const AkuData &akudata)
{
    m_headerLabel->setVisible(akudata.headerprotected);
    m_lockLabel->setVisible(akudata.locked);
    m_commentButton->setVisible(!akudata.comment.isEmpty());
    m_passwordLabel->setVisible(!akudata.passwordprotectedPaths.isEmpty());
    kDebug() << QString::number(akudata.passwordprotectedPaths.size());
    kDebug() << akudata.passwordprotectedPaths;

    if (!akudata.headerprotected && akudata.passwordprotectedPaths.isEmpty() && !akudata.locked) {
        m_statusOkLabel->setVisible(true);
    } else {
        m_statusOkLabel->setVisible(false);
    }
}
