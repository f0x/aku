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
#include "akutreenode.h"
#include "akutreemodel.h"

#include <QCheckBox>
#include <QLabel>

#include <KComboBox>
#include <KLineEdit>
#include <KLocale>
#include <KVBox>
#include <kfilewidget.h>

#include <KDebug>

AddFileDialog::AddFileDialog(QWidget *parent) : KFileDialog(QDir::homePath(), "*.*", parent)
{
    setCaption(i18n("Add file(s) and/or dir(s)"));

    setOperationMode(KFileDialog::Other);
    setMode(KFile::Directory | KFile::Files);
    setModal(true);

    // Here we set the custom widget up (this is used for adding files crypting them with password)
    KVBox *layout = new KVBox();
    //layout->setMargin(100);
    KHBox *hlayout = new KHBox(layout);
    QLabel *label = new QLabel(hlayout);
    label->setText(i18n("Adding path") + ": ");
    m_lineEdit = new KLineEdit(hlayout);

    m_comboBox = new KComboBox(layout);
    m_comboBox->addItem(i18n("Add and replace file(s)"));
    m_comboBox->addItem(i18n("Add and update file(s)"));
    m_comboBox->addItem(i18n("Fresh existing file(s) only"));
    m_comboBox->addItem(i18n("Ask before overwrite"));
    m_comboBox->addItem(i18n("Skip existing file(s)"));

    m_passwordCheck = new QCheckBox(i18n("Add file(s) with a password"), layout);
    m_passwordCheck->setVisible(false);
    static_cast<KFileWidget*>(fileWidget())->setCustomWidget(QString(), layout);
    setInitialSize(QSize(300, 250));

    connect(this, SIGNAL(accepted()), this, SLOT(filesToAdd()));

}

AddFileDialog::~AddFileDialog()
{
}

void AddFileDialog::updateInfo(const QModelIndex &index, bool passwordSupport)
{
    if (!passwordSupport) {
        m_passwordCheck->setVisible(false);
    }

    if (!index.isValid()) {
        return;
    }

    AkuTreeNode *node = index.data(AkuTreeModel::NodeRole).value<AkuTreeNode*>();

    //kDebug() << node->path();
    if (node->isFolder()) {
        m_lineEdit->setText(node->path());
    } else {
        QFileInfo info(node->path());
        m_lineEdit->setText(info.path());
    }

}

void AddFileDialog::filesToAdd()
{
    emit addFiles(selectedFiles(), m_lineEdit->text());
}
