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

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(5);
    widget->setLayout(layout);

    QLabel *passwordIcon = new QLabel;
    passwordIcon->setPixmap(KIconLoader::global()->loadIcon("dialog-password", KIconLoader::Small));
    passwordIcon->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QFont font;
    font.setBold(true);
    m_filenameLabel = new QLabel;
    m_filenameLabel->setFont(font);

    QHBoxLayout *hlayout1 = new QHBoxLayout;
    layout->addLayout(hlayout1);
    hlayout1->addSpacing(10);
    hlayout1->addWidget(passwordIcon);
    hlayout1->addWidget(m_filenameLabel);
    hlayout1->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed));
    hlayout1->setSpacing(10);

    QLabel *infoLabel = new QLabel;
    infoLabel->setText(i18n("The archive is header password protected"));

    QHBoxLayout *hlayout2 = new QHBoxLayout;
    layout->addLayout(hlayout2);
    hlayout2->addSpacing(10);
    hlayout2->addWidget(infoLabel);
    hlayout2->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed));
    hlayout2->setSpacing(10);

    KLineEdit *lineEdit = new KLineEdit(widget);
    lineEdit->setClickMessage(i18n("Enter the password..."));
    lineEdit->setPasswordMode(true);
    lineEdit->setClearButtonShown(true);

    QCheckBox *checkBox = new QCheckBox(i18n("Hidden"), widget);
    checkBox->setChecked(true);
    connect(checkBox, SIGNAL(clicked(bool)), lineEdit, SLOT(setPasswordMode(bool)));

    KPushButton *okButton = new KPushButton(KIcon("dialog-ok-apply"), i18n("Enter"), widget);
    connect(okButton, SIGNAL(clicked()), this, SLOT(buttonPressed()));
    KPushButton *closeButton = new KPushButton(KIcon("dialog-close"), i18n("Abort"), widget);
    connect(closeButton, SIGNAL(clicked()), this, SLOT(buttonPressed()));

    QHBoxLayout *hlayout3 = new QHBoxLayout;
    layout->addLayout(hlayout3);
    hlayout3->addSpacing(10);
    hlayout3->addWidget(lineEdit);
    hlayout3->addWidget(checkBox);
    hlayout3->addSpacerItem(new QSpacerItem(30, 10, QSizePolicy::Fixed, QSizePolicy::Fixed));
    hlayout3->addWidget(okButton);
    hlayout3->addWidget(closeButton);
    hlayout3->setSpacing(10);

    QLabel *warningIcon = new QLabel(widget);
    warningIcon->setPixmap(KIconLoader::global()->loadIcon("emblem-important", KIconLoader::Small));
    warningIcon->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    font.setBold(false);
    font.setUnderline(true);
    QLabel *warningLabel = new QLabel(widget);
    warningLabel->setFont(font);
    warningLabel->setText(i18n("The password is not correct. Try again"));

    QHBoxLayout *hlayout4 = new QHBoxLayout;
    layout->addLayout(hlayout4);
    hlayout4->setSpacing(10);
    hlayout4->addSpacing(10);
    hlayout4->addWidget(warningIcon);
    hlayout4->addWidget(warningLabel);
    hlayout4->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed));

   // QLabel *icon = new QLabel(m_base);
   // icon->setPixmap(KIconLoader::global()->loadIcon("dialog-information", KIconLoader::Small));

   // m_tipLabel = new QLabel(m_base);
   // m_tipLabel->setWordWrap(true);
   // m_tipLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
   // m_tipLabel->setPalette(p);

   // m_closeButton = new QToolButton(m_base);
   // m_closeButton->setIcon(KIcon("dialog-close"));
   // m_closeButton->setAutoRaise(true);
   // connect(m_closeButton, SIGNAL(clicked()), this, SLOT(hideTip()));

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
    //bool fromButton = false;
    //if (sender() == m_closeButton) {
    //    fromButton = true;
    //}

    m_hiding = true;
    m_timeLine->setFrameRange(0, -height());
    m_timeLine->start();
}

void PassWidget::slotFinish()
{
    if (m_hiding) {
        hide();
        //emit tooltipClosed(this);
    } //else {
      //  QTimer::singleShot(5000, this, SLOT(hideTip()));
    //}
}

void PassWidget::enterEvent(QEvent *event)
{
    //m_mouseIn = true;
    QWidget::enterEvent(event);
}

void PassWidget::leaveEvent(QEvent *event)
{
    //m_mouseIn = false;
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
    hideWidget();
}
