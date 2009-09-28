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

#include "commentwidget.h"

#include <QLabel>
#include <QTextStream>
#include <QVBoxLayout>

#include <KFileDialog>
#include <KGuiItem>
#include <KLocale>
#include <KPushButton>
#include <KTextEdit>

CommentWidget::CommentWidget(QWidget *parent) : QWidget(parent)
{
    //setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QVBoxLayout *layout = new QVBoxLayout;

    m_textEdit = new KTextEdit(this);
    connect(m_textEdit, SIGNAL(textChanged()), this, SLOT(commentChanged()));

    QHBoxLayout *hlayout = new QHBoxLayout;

    KGuiItem loadGuiItem(i18n("Load comment from file"), "document-open");
    KPushButton *loadButton = new KPushButton(this);
    loadButton->setGuiItem(loadGuiItem);
    connect(loadButton, SIGNAL(clicked()), this, SLOT(loadFromFile()));

    KGuiItem saveGuiItem(i18n("Save"), "document-save");
    KPushButton *saveButton = new KPushButton(this);
    saveButton->setGuiItem(saveGuiItem);
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveComment()));

    QFont font;
    font.setItalic(true);
    m_label = new QLabel;
    m_label->setFont(font);

    hlayout->addSpacerItem(new QSpacerItem(5, 10, QSizePolicy::Fixed, QSizePolicy::Minimum));
    hlayout->addWidget(m_label);
    hlayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum));
    hlayout->addWidget(loadButton);
    hlayout->addWidget(saveButton);

    QHBoxLayout *hlayout2 = new QHBoxLayout;
    QLabel *labelSize = new QLabel(i18n("byte(s)"));
    m_size = new QLabel;
    hlayout2->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum));
    hlayout2->addWidget(m_size);
    hlayout2->addWidget(labelSize);
    hlayout2->addSpacing(10);

    layout->addLayout(hlayout);
    layout->addWidget(m_textEdit);
    layout->addLayout(hlayout2);
    setLayout(layout);

    hide();
}

CommentWidget::~CommentWidget()
{
}

void CommentWidget::setComment(QString comment)
{
    m_textEdit->setText(comment);
    m_label->clear();
}

void CommentWidget::loadFromFile()
{
    QString fileName = KFileDialog::getOpenFileName(KUrl(QDir().homePath()), i18n("All Files *.*"),
                                              this, i18n("Select comment file"));

    if ( !fileName.isEmpty()) {
        QFile file(fileName);
        file.open(QIODevice::ReadOnly);
        m_textEdit->setPlainText(QTextStream(&file).readAll());
    }
}

void CommentWidget::saveComment()
{
    m_label->clear();
}

void CommentWidget::commentChanged()
{
    m_label->setText(i18n("The comment has changed"));
    QByteArray text = m_textEdit->toPlainText().toLocal8Bit();
    m_size->setText(QString::number(text.size()));
}

void CommentWidget::clearComment()
{
    m_textEdit->clear();
    m_label->clear();
}
