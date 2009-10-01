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

#include "errorwidget.h"

#include <QFileInfo>
#include <QTime>
#include <QTreeWidget>
#include <QVBoxLayout>

#include <KIcon>
#include <KLocale>
#include <KDebug>

ErrorWidget::ErrorWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;

    m_treeWidget = new QTreeWidget(this);
    m_treeWidget->setColumnCount(4);
    m_treeWidget->setRootIsDecorated(false);
    m_treeWidget->setHeaderHidden(true);
    m_treeWidget->setWordWrap(true);

    layout->addWidget(m_treeWidget);
    setLayout(layout);
}

ErrorWidget::~ErrorWidget()
{
}

void ErrorWidget::sendData(const QString &error, QString filename, AkuPlugin::CurrentOperation operation)
{
    QString tooltip;
    QString kdeIcon;
    switch (operation) {
        case AkuPlugin::Extracting :
            kDebug() << "EXTRACTING";
            tooltip.append(i18n("Extracting error"));
            kdeIcon = "archive-extract";
            break;
        case AkuPlugin::Loading :
            kDebug() << "LOADING";
            tooltip.append(i18n("Loading error"));
            kdeIcon = "document-open.png";
            break;
        case AkuPlugin::Locking :
            kDebug() << "LOCKING";
            tooltip.append(i18n("Locking error"));
            kdeIcon = "document-encrypt";
            break;
        default:
            kDebug() << "NO OPERATION";
            tooltip.append(i18n("Generic error"));
            kdeIcon = "system-help";
        ;
    }

    QTreeWidgetItem *iconItem = new QTreeWidgetItem;
    iconItem->setIcon(0, KIcon(kdeIcon));
    iconItem->setToolTip(0, tooltip);
    QFileInfo file(filename);
    iconItem->setText(1, file.fileName());
    QTime currTime = QTime::currentTime();
    iconItem->setText(2, currTime.toString());
    iconItem->setTextAlignment(2, Qt::AlignHCenter);
    QFont font;
    font.setBold(true);
    iconItem->setFont(3, font);
    iconItem->setText(3, error.trimmed());
    iconItem->setToolTip(3, error.trimmed());
    m_treeWidget->insertTopLevelItem(0, iconItem);
    m_treeWidget->resizeColumnToContents(0);
    m_treeWidget->resizeColumnToContents(2);
}
