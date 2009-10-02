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

#include "overwritewidget.h"

#include <QCheckBox>
#include <QFileInfo>
#include <QLabel>
#include <QVBoxLayout>

#include <KIconLoader>
#include <KLineEdit>
#include <KLocale>
#include <KMimeType>
#include <KPushButton>

OverwriteWidget::OverwriteWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;

    setAutoFillBackground(true);
    QPalette p = palette();
    p.setColor(QPalette::Window, Qt::white);
    p.setColor(QPalette::WindowText, Qt::black);
    setPalette(p);

    // Line 1
    QHBoxLayout *layoutLine1 = new QHBoxLayout;

    m_fileIcon = new QLabel;

    QFont font;
    font.setBold(true);
    m_filenameLabel = new QLabel;
    m_filenameLabel->setFont(font);

    QLabel *inLabel = new QLabel(i18n("in"));

    layoutLine1->addSpacing(10);
    layoutLine1->addWidget(m_fileIcon);
    layoutLine1->addWidget(m_filenameLabel);
    layoutLine1->addWidget(inLabel);
    layoutLine1->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum));
    //

    // Line 2
    QHBoxLayout *layoutLine2 = new QHBoxLayout;

    m_pathLabel = new QLabel;

    layoutLine2->addSpacing(10);
    layoutLine2->addWidget(m_pathLabel);
    layoutLine2->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum));
    //

    // Line 3
    QHBoxLayout *layoutLine3 = new QHBoxLayout;

    QLabel *messageLabel = new QLabel;
    messageLabel->setFont(font);
    messageLabel->setText("already exists. Overwrite?");

    layoutLine3->addSpacing(10);
    layoutLine3->addWidget(messageLabel);
    layoutLine3->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum));
    //

    // Grid
    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->setHorizontalSpacing(20);

    // yes
    KPushButton *yesButton = new KPushButton(KIcon("dialog-ok"), i18n("Yes"), this);
    QCheckBox *allCheck = new QCheckBox(i18n("All"));
    //

    // no
    KPushButton *noButton = new KPushButton(KIcon("dialog-cancel"), i18n("No"), this);
    QCheckBox *neverCheck = new QCheckBox(i18n("Never"));
    //

    // rename
    KPushButton *renameButton = new KPushButton(KIcon("edit-rename"), i18n("Rename"), this);
    KLineEdit *renameEdit = new KLineEdit(this);
    renameEdit->setClearButtonShown(true);
    renameEdit->setClickMessage(i18n("New name..."));
    //

    // quit
    KPushButton *quitButton = new KPushButton(KIcon("dialog-close"), i18n("Quit"), this);
    //

    gridLayout->addItem(new QSpacerItem(10, 10, QSizePolicy::Fixed, QSizePolicy::Minimum), 0, 0);
    gridLayout->addWidget(yesButton, 0, 1);
    gridLayout->addWidget(allCheck, 0, 2);
    gridLayout->addWidget(noButton, 1, 1);
    gridLayout->addWidget(neverCheck, 1, 2);
    gridLayout->addWidget(renameButton, 2, 1);
    gridLayout->addWidget(renameEdit, 2, 2);
    gridLayout->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum), 2, 3);
    gridLayout->addWidget(quitButton, 3, 1);

    ///
    layout->setSpacing(5);
    layout->addLayout(layoutLine1);
    layout->addLayout(layoutLine2);
    layout->addLayout(layoutLine3);
    layout->addSpacing(15);
    layout->addLayout(gridLayout);
    setLayout(layout);
}

OverwriteWidget::~OverwriteWidget()
{
}

void OverwriteWidget::setInfo(const QString &filename)
{
    KMimeType::Ptr mimetype = KMimeType::findByPath(filename);
    m_fileIcon->setPixmap(KIconLoader::global()->loadMimeTypeIcon(mimetype->iconName(),
                                                                  KIconLoader::Desktop, KIconLoader::SizeSmall));
    QFileInfo fileinfo(filename);
    m_filenameLabel->setText(fileinfo.fileName());
    m_pathLabel->setText(fileinfo.absolutePath());
    m_pathLabel->setToolTip(fileinfo.absolutePath());
}
