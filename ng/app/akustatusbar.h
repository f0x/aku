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

#ifndef AKUSTATUSBAR_H
#define AKUSTATUSBAR_H

#include "akuplugin.h"

#include <KStatusBar>

class QToolButton;
class QLabel;

class AkuStatusBar : public KStatusBar
{
    Q_OBJECT

public:
    AkuStatusBar(QWidget *parent = 0);
    ~AkuStatusBar();

private:
    AkuPlugin *m_plugin;

    QWidget *m_statusWidget;

    QToolButton *m_commentButton;
    QToolButton *m_busyButton;
    QToolButton *m_errorButton;

    QLabel *m_statusLabel;
    QLabel *m_statusOkLabel;
    QLabel *m_lockLabel;
    QLabel *m_headerLabel;
    QLabel *m_passwordLabel;

public slots:
    void stateChanged(AkuPlugin *);
    void operationCompleted();

private slots:
    void abortOperation();
    void setupConnections();
    void archiveInformation(const AkuData &);
};

#endif
