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

#include "addfiledialog.h"

#include <QCheckBox>

#include <KLocale>
#include <KVBox>
#include <kfilewidget.h>

AddFileDialog::AddFileDialog(QWidget *parent) : KFileDialog(QDir::homePath(), "*.*", parent)
{
    setOperationMode(KFileDialog::Other);
    setMode(KFile::Files);
    setModal(true);

    // Here we set the custom widget up (this is used for adding files crypting them with password)
    KHBox *layout = new KHBox();
    //layout->setMargin(100);

    m_passwordCheck = new QCheckBox(i18n("Add file(s) with a password"), layout);
    static_cast<KFileWidget*>(fileWidget())->setCustomWidget(QString(), layout);
    setInitialSize(QSize(300, 250));

}

AddFileDialog::~AddFileDialog()
{
}
