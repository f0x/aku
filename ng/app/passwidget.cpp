/***************************************************************************
 *   Copyright 2009 by Francesco Grieco <fgrieco@gmail.com>                *
 *                     Alessandro Diaferia <alediaferia@gmail.com>         *
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

#include "passwidget.h"

#include <QCheckBox>
#include <QVBoxLayout>
#include <QFileInfo>
#include <QPalette>
#include <QLabel>
#include <QTimeLine>
#include <QTimer>

#include <KPushButton>
#include <KIconLoader>
#include <KLineEdit>
#include <KLocale>
#include <KHBox>
#include <KDebug>

const int DURATION = 700; // ms

PassWidget::PassWidget(QWidget *parent) : QWidget(parent)
{
//     setAttribute(Qt::WA_DeleteOnClose);

    m_base = new KHBox(this);
    m_base->setAutoFillBackground(true);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QPalette p = m_base->palette();
    p.setColor(QPalette::Window, Qt::white);
    p.setColor(QPalette::WindowText, Qt::black);
    m_base->setPalette(p);

    QWidget *widget = new QWidget(m_base);

    m_layout = new QVBoxLayout;
    m_layout->setSpacing(5);
    widget->setLayout(m_layout);

    QLabel *passwordIcon = new QLabel;
    passwordIcon->setPixmap(KIconLoader::global()->loadIcon("dialog-password", KIconLoader::Small));
    passwordIcon->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QFont font;
    font.setBold(true);
    m_filenameLabel = new QLabel;
    m_filenameLabel->setFont(font);

    QHBoxLayout *hlayout1 = new QHBoxLayout;
    m_layout->addLayout(hlayout1);
    hlayout1->addSpacing(10);
    hlayout1->addWidget(passwordIcon);
    hlayout1->addWidget(m_filenameLabel);
    hlayout1->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed));
    hlayout1->setSpacing(10);

    QLabel *infoLabel = new QLabel;
    infoLabel->setText(i18n("The archive is header password protected"));

    QHBoxLayout *hlayout2 = new QHBoxLayout;
    m_layout->addLayout(hlayout2);
    hlayout2->addSpacing(10);
    hlayout2->addWidget(infoLabel);
    hlayout2->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed));
    hlayout2->setSpacing(10);

    m_lineEdit = new KLineEdit(widget);
    m_lineEdit->setClickMessage(i18n("Enter the password..."));
    m_lineEdit->setPasswordMode(true);
    m_lineEdit->setClearButtonShown(true);

    QCheckBox *checkBox = new QCheckBox(i18n("Hidden"), widget);
    checkBox->setChecked(true);
    connect(checkBox, SIGNAL(clicked(bool)), m_lineEdit, SLOT(setPasswordMode(bool)));

    KPushButton *okButton = new KPushButton(KIcon("dialog-ok-apply"), i18n("Enter"), widget);
    connect(okButton, SIGNAL(clicked()), this, SLOT(buttonPressed()));
    KPushButton *closeButton = new KPushButton(KIcon("dialog-close"), i18n("Abort"), widget);
    connect(closeButton, SIGNAL(clicked()), this, SLOT(hideWidget()));

    QHBoxLayout *hlayout3 = new QHBoxLayout;
    m_layout->addLayout(hlayout3);
    hlayout3->addSpacing(10);
    hlayout3->addWidget(m_lineEdit);
    hlayout3->addWidget(checkBox);
    hlayout3->addSpacerItem(new QSpacerItem(30, 10, QSizePolicy::Fixed, QSizePolicy::Fixed));
    hlayout3->addWidget(okButton);
    hlayout3->addWidget(closeButton);
    hlayout3->setSpacing(10);

    m_warningIcon = new QLabel(widget);
    m_warningIcon->setPixmap(KIconLoader::global()->loadIcon("emblem-important", KIconLoader::Small));
    m_warningIcon->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    //m_warningIcon->setVisible(true);

    font.setUnderline(true);
    m_warningLabel = new QLabel(widget);
    m_warningLabel->setFont(font);
    m_warningLabel->setText(i18n("The password is not correct. Try again"));
    //m_warningLabel->setVisible(false);

    QHBoxLayout *badPassLayout = new QHBoxLayout;
    m_layout->addLayout(badPassLayout);
    badPassLayout->setSpacing(10);
    badPassLayout->addSpacing(10);
    badPassLayout->addWidget(m_warningIcon);
    badPassLayout->addWidget(m_warningLabel);
    badPassLayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed));

    m_timeLine = new QTimeLine(DURATION, this);
    connect(m_timeLine, SIGNAL(frameChanged(int)), this, SLOT(animate(int)));
    connect(m_timeLine, SIGNAL(finished()), this, SLOT(slotFinish()));

    m_base->setGeometry(0, -height(), width(), height());

    hide();
}

PassWidget::~PassWidget()
{
}

void PassWidget::animate(int y)
{
    m_base->setGeometry(0, y, width(), height());
}

void PassWidget::askPassword()
{
    m_warningIcon->setVisible(!m_password.isEmpty());
    m_warningLabel->setVisible(!m_password.isEmpty());

    show();
    m_hiding = false;

    if (m_timeLine->state() == QTimeLine::Running) {
        m_timeLine->stop();
    }

    m_timeLine->setFrameRange(-height(), 0);
    m_timeLine->start();
}

void PassWidget::hideWidget()
{
    m_hiding = true;
    m_timeLine->setFrameRange(0, -height());
    m_timeLine->start();
}

void PassWidget::slotFinish()
{
    if (m_hiding) {
        hide();  
    }
}

void PassWidget::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
}

void PassWidget::leaveEvent(QEvent *event)
{
    slotFinish();
    QWidget::leaveEvent(event);
}

void PassWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    m_base->resize(size());
}

QSize PassWidget::sizeHint() const
{
    return m_base->sizeHint();
}

void PassWidget::setArchiveName(QString name)
{
    QFileInfo file(name);
    m_filenameLabel->setText(file.fileName());
}

void PassWidget::buttonPressed()
{
    emit password(m_lineEdit->text());
    m_password = m_lineEdit->text();
}

void PassWidget::clearPassword()
{
    m_lineEdit->clear();
    m_password.clear();
}
