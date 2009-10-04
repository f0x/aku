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

#ifndef OVERWRITEWIDGET_H
#define OVERWRITEWIDGET_H

#include <QWidget>

class QLabel;

class KLineEdit;
class KPushButton;

class AkuPlugin;

class OverwriteWidget : public QWidget
{
    Q_OBJECT

public:
    OverwriteWidget(QWidget *parent);
    ~OverwriteWidget();

private:
    QLabel *m_fileIcon;
    QLabel *m_filenameLabel;
    QLabel *m_pathLabel;

    KLineEdit *m_renameEdit;
    KPushButton *m_yesButton;
    KPushButton *m_noButton;
    KPushButton *m_quitButton;
    KPushButton *m_renameButton;

    AkuPlugin *m_plugin;

public slots:
    void setInfo(const QString &, AkuPlugin *);

private slots:
    void sendAnswer();
};

#endif // OVERWRITEWIDGET_H
