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

#include "metawidget.h"
#include "akutreenode.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>

#include <KVBox>
#include <KIcon>
#include <KLocale>
#include <KSeparator>
#include <KGlobalSettings>
#include <KDebug>

MetaWidget::MetaWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    m_fileLabel = new QLabel(this);
    m_fileLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    m_fileLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    m_iconLabel = new QLabel(this);
    m_iconLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    m_iconLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    KSeparator *topSeparator = new KSeparator(this);
    KSeparator *bottomSeparator = new KSeparator(this);

    QSpacerItem *spacer = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding);

    QLabel *modifiedLayout = new QLabel(i18n("Modified") + ":", this);
    modifiedLayout->setFont(KGlobalSettings::smallestReadableFont());
    QLabel *sizeLayout = new QLabel(i18n("Size") + ":", this);
    sizeLayout->setFont(KGlobalSettings::smallestReadableFont());
    QLabel *typeLayout = new QLabel(i18n("Type") + ":", this);
    typeLayout->setFont(KGlobalSettings::smallestReadableFont());

    m_modified = new QLabel(this);
    m_size = new QLabel(this);
    m_type = new QLabel(this);

    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->addWidget(modifiedLayout, 0, 0, Qt::AlignRight);
    gridLayout->addWidget(m_modified, 0, 1, Qt::AlignLeft);
    gridLayout->addWidget(sizeLayout, 1, 0, Qt::AlignRight);
    gridLayout->addWidget(m_size, 0, 1, Qt::AlignLeft);
    gridLayout->addWidget(typeLayout, 2, 0, Qt::AlignRight);
    gridLayout->addWidget(m_type, 0, 1, Qt::AlignLeft);
    gridLayout->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    layout->addWidget(m_fileLabel);
    layout->addWidget(topSeparator);
    layout->addWidget(m_iconLabel);
    layout->addWidget(bottomSeparator);
    layout->addLayout(gridLayout);
    layout->addSpacerItem(spacer);
    layout->insertSpacing(2, 10);
    layout->insertSpacing(4, 10);
    layout->insertSpacing(6, 5);
    setLayout(layout);

}

MetaWidget::~MetaWidget()
{
}

void MetaWidget::updateData(AkuTreeNode *node)
{
    kDebug() << node->mimeType();
}

void MetaWidget::setMimeIcon(QString mimeIcon)
{
    m_iconLabel->setPixmap(KIcon(mimeIcon).pixmap(128, 128));
}
