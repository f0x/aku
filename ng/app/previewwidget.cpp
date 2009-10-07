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

#include "previewwidget.h"
#include "akuplugin.h"

#include <QFileInfo>
#include <QHBoxLayout>
#include <QLabel>

#include <KParts/ReadWritePart>
#include <kmimetypetrader.h>
#include <KMimeType>
#include <KTempDir>
#include <KVBox>

#include <KDebug>

PreviewWidget::PreviewWidget(QWidget *parent) : KDialog(parent)
{
    setModal(true);

    QVBoxLayout *layout = new QVBoxLayout;

    QHBoxLayout *topLayout = new QHBoxLayout;

    m_vbox = new KVBox;

    m_iconLabel = new QLabel;
    topLayout->addWidget(m_iconLabel);

    QVBoxLayout *vlayout = new QVBoxLayout;

    m_filenameLabel = new QLabel;
    QFont font;
    font.setBold(true);
    m_filenameLabel->setFont(font);

    m_mimetypeLabel = new QLabel;

    vlayout->addWidget(m_filenameLabel);
    vlayout->addWidget(m_mimetypeLabel);

    topLayout->addLayout(vlayout);
    topLayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum));

    layout->addLayout(topLayout);
    layout->addWidget(m_vbox);
    layout->setSpacing(10);
    mainWidget()->setLayout(layout);
    setButtons(KDialog::Close);
    resize(400, 300);

}

PreviewWidget::~PreviewWidget()
{
}

void PreviewWidget::previewOf(const KUrl &archive, const QString &filename, AkuPlugin *plugin)
{   
    QFileInfo fileInfo(filename);
    setCaption(fileInfo.fileName());
    m_filenameLabel->setText(fileInfo.fileName());

    AkuExtractInfo info;
    info.fileName = archive;
    info.files << filename;
    KTempDir tempDir;
    info.destination = tempDir.name();

    AkuPlugin::ExtractionOptions options;
    options |= AkuPlugin::OverwriteWithoutPrompt;

    plugin->extract(info, options);

    KUrl filePath = tempDir.name() + fileInfo.fileName();
    kDebug() << tempDir.name();
    kDebug() << fileInfo.fileName();
    kDebug() << filePath;

    KMimeType::Ptr mimetype = KMimeType::findByUrl(filePath);
    m_iconLabel->setPixmap(KIconLoader::global()->loadMimeTypeIcon(mimetype->iconName(),
                                                                  KIconLoader::Desktop, KIconLoader::SizeLarge));
    m_mimetypeLabel->setText(mimetype->comment());

    KParts::ReadOnlyPart *part;
    part = KMimeTypeTrader::self()->createPartInstanceFromQuery<KParts::ReadOnlyPart>(mimetype->name(),
                                                                                      m_vbox, this);

    part->openUrl(filePath);

}
