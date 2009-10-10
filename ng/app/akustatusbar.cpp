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
#include "akuplugin.h"

#include <QLabel>
#include <QToolButton>
#include <QHBoxLayout>

#include <KIcon>
#include <KIconLoader>
#include <KLocale>
#include <KDebug>

AkuStatusBar::AkuStatusBar(QWidget *parent) : KStatusBar(parent)
{
    busyButton = new QToolButton;
    busyButton->setIcon(KIcon("user-away"));
    busyButton->setAutoRaise(true);
    busyButton->setToolTip(i18n("Abort the operation"));
    busyButton->setVisible(false);
    addWidget(busyButton);

    statusLabel = new QLabel;
    addWidget(statusLabel);

    statusWidget = new QWidget;
    addPermanentWidget(statusWidget);

    QHBoxLayout *layout = new QHBoxLayout(statusWidget);
    commentButton = new QToolButton;
    commentButton->setIcon(KIcon("edit-paste"));
    commentButton->setToolTip(i18n("The archive has a comment. Click to read or to modify it"));
    commentButton->setAutoRaise(true);

    statusOkLabel = new QLabel;
    statusOkLabel->setPixmap(KIconLoader::global()->loadMimeTypeIcon("user-online", KIconLoader::Desktop,
                                                                     KIconLoader::SizeSmall));
    statusOkLabel->setToolTip(i18n("This archive has no global restrictions"));

    lockLabel = new QLabel;
    lockLabel->setPixmap(KIconLoader::global()->loadMimeTypeIcon("object-locked", KIconLoader::Desktop,
                                                                 KIconLoader::SizeSmall));
    lockLabel->setToolTip(i18n("Locked archive. Any attempt to modify the archive will be ignored"));

    headerLabel = new QLabel;
    headerLabel->setPixmap(KIconLoader::global()->loadMimeTypeIcon("security-low", KIconLoader::Desktop,
                                                                   KIconLoader::SizeSmall));
    headerLabel->setToolTip(i18n("This archive has a header password protectio.n") + "<br>" +
                            i18n("File data, file names, sizes, attributes, comments are encrypted.") +
                            "<br>" + i18n("Without a password it is impossible to view even the list of files in archive"));

    passwordLabel = new QLabel;
    passwordLabel->setPixmap(KIconLoader::global()->loadMimeTypeIcon("dialog-password", KIconLoader::Desktop,
                                                                     KIconLoader::SizeSmall));
    passwordLabel->setToolTip("This archive has one or more password protected file(s)");

    errorButton = new QToolButton;
    errorButton->setIcon(KIcon("dialog-warning"));
    errorButton->setAutoRaise(true);
    errorButton->setToolTip(i18n("Error occured"));

    layout->addWidget(errorButton);
    layout->addWidget(commentButton);
    layout->addWidget(lockLabel);
    layout->addWidget(headerLabel);
    layout->addWidget(passwordLabel);
    layout->addWidget(statusOkLabel);

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
            busyButton->setVisible(true);
            statusLabel->setText(i18n("Extraction in progress..."));
            //showMessage(i18n("Extraction in progress..."));
            break;
        case AkuPlugin::Loading :
            busyButton->setVisible(true);
            statusLabel->setText(i18n("Loading archive..."));
            //showMessage(i18n("Loading archive..."));
            break;
        case AkuPlugin::Locking :
            busyButton->setVisible(true);
            statusLabel->setText(i18n("Locking archive..."));
            //showMessage(i18n("Locking archive..."));
            break;
        case AkuPlugin::Adding :
            busyButton->setVisible(true);
            statusLabel->setText(i18n("Adding to the archive..."));
            break;
        case AkuPlugin::Deleting :
            busyButton->setVisible(true);
            statusLabel->setText(i18n("Removing files..."));
            break;
        default: ;
            statusLabel->clear();
            busyButton->setVisible(false);
    }
}

void AkuStatusBar::operationCompleted()
{
    kDebug() << "Operation Completed";
    clearMessage();
}

void AkuStatusBar::abortOperation()
{
    //m_plugin->abortJob();
}
