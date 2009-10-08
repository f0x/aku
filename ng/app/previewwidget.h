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

#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include <KDialog>

class AkuPlugin;
class QLabel;
class KTempDir;
class KVBox;

class PreviewWidget : public KDialog
{
    Q_OBJECT

public:
    PreviewWidget(QWidget *parent = 0);
    ~PreviewWidget();

private:
    QLabel *m_iconLabel;
    QLabel *m_filenameLabel;
    QLabel *m_mimetypeLabel;

    KVBox *m_vbox;

    KTempDir *m_tempDir;

public slots:
    void previewOf(const KUrl &, const QString &, AkuPlugin *);
};

#endif // PREVIEWWIDGET_H
