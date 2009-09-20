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
#include <KLocale>
#include <KIconLoader>
#include <KSeparator>
#include <KGlobalSettings>
#include <KMimeType>
#include <KDebug>

MetaWidget::MetaWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    m_fileLabel = new QLabel(this);
    m_fileLabel->setAlignment(Qt::AlignHCenter);
    m_fileLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    QFont font;
    font.setBold(true);
    m_fileLabel->setFont(font);
    m_fileLabel->setScaledContents(true);

    m_iconLabel = new QLabel(this);
    m_iconLabel->setAlignment(Qt::AlignHCenter);
    m_iconLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    KSeparator *topSeparator = new KSeparator(this);
    KSeparator *bottomSeparator = new KSeparator(this);

    QSpacerItem *spacer = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    QLabel *sizeLayout = new QLabel(i18n("Size") + ":", this);
    sizeLayout->setFont(KGlobalSettings::smallestReadableFont());
    QLabel *typeLayout = new QLabel(i18n("Type") + ":", this);
    typeLayout->setFont(KGlobalSettings::smallestReadableFont());

    m_size = new QLabel(this);
    m_size->setFont(KGlobalSettings::smallestReadableFont());
    m_type = new QLabel(this);
    m_type->setFont(KGlobalSettings::smallestReadableFont());

    m_gridLayout = new QGridLayout();
    m_gridLayout->setAlignment(Qt::AlignCenter);
    m_gridLayout->setMargin(width() / 100 * 20);
    m_gridLayout->addWidget(sizeLayout, 0, 0, Qt::AlignRight);
    m_gridLayout->addWidget(m_size, 0, 1, Qt::AlignLeft);
    m_gridLayout->addWidget(typeLayout, 1, 0, Qt::AlignRight);
    m_gridLayout->addWidget(m_type, 1, 1, Qt::AlignLeft);

    layout->addWidget(m_fileLabel);
    layout->addWidget(topSeparator);
    layout->addWidget(m_iconLabel);
    layout->addWidget(bottomSeparator);
    layout->addLayout(m_gridLayout);
    layout->addSpacerItem(spacer);
    layout->insertSpacing(0, 5);
    layout->insertSpacing(2, 10);
    layout->insertSpacing(4, 10);
    layout->insertSpacing(6, 5);
    //setLayout(layout);

}

MetaWidget::~MetaWidget()
{
}

void MetaWidget::sendData(AkuTreeNode *node)
{
    QString fileName = node->name();
    QString mimeIcon = node->mimeType();
    kDebug() << mimeIcon;
    QString size = QString::number(node->size());
    updateData(fileName, mimeIcon, size);
}

void MetaWidget::updateData(QString fileName, QString mimeIcon, QString size)
{
    m_fileLabel->setText(fileName);
    m_size->setText(size);
    m_iconLabel->setPixmap(KIconLoader::global()->loadMimeTypeIcon(KMimeType::mimeType(mimeIcon)->iconName(),
                                                                   KIconLoader::Desktop, KIconLoader::SizeEnormous));
    m_type->setText(KMimeType::mimeType(mimeIcon)->comment());

}
