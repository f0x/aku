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

#include <QPalette>
#include <QLabel>
#include <QToolButton>
#include <QTimeLine>
#include <QTimer>
#include <QHBoxLayout>

#include <KPushButton>
#include <KIconLoader>
#include <KLocale>
#include <KVBox>
#include <KDebug>

const int DURATION = 500; // ms

PassWidget::PassWidget(QWidget *parent) : QWidget(parent)
{
    m_base = new KVBox(this);
    m_base->setAutoFillBackground(true);   

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QPalette p = m_base->palette();
    p.setColor(QPalette::Window, Qt::white);
    p.setColor(QPalette::WindowText, Qt::black);
    m_base->setPalette(p);

    // TOP Layout
    QLabel *passwordIcon = new QLabel(this);
    passwordIcon->setPixmap(KIconLoader::global()->loadIcon("dialog-password", KIconLoader::Small));
    passwordIcon->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QFont font;
    font.setBold(true);
    QLabel *topLabel = new QLabel(this);
    topLabel->setFont(font);
    topLabel->setText(i18n("The archive is header password protected"));

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->setSpacing(10);
    topLayout->addSpacing(10);
    topLayout->addWidget(passwordIcon);
    topLayout->addWidget(topLabel);
    topLayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed));

    QHBoxLayout *baseLayout = new QHBoxLayout(this);
    baseLayout->addWidget(m_base);

    //

    //QLabel *icon = new QLabel(m_base);
    //icon->setPixmap(KIconLoader::global()->loadIcon("dialog-information", KIconLoader::Small));

    //m_tipLabel = new QLabel(m_base);
    //m_tipLabel->setWordWrap(true);
    //m_tipLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    //m_tipLabel->setPalette(p);

    //m_closeButton = new QToolButton(m_base);
    //m_closeButton->setIcon(KIcon("dialog-close"));
    //m_closeButton->setAutoRaise(true);
    //connect(m_closeButton, SIGNAL(clicked()), this, SLOT(hideTip()));

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

void PassWidget::showTip()
{
    show();
    m_hiding = false;

    if (m_timeLine->state() == QTimeLine::Running) {
        m_timeLine->stop();
    }

    m_timeLine->setFrameRange(-height(), 0);
    m_timeLine->start();
}

void PassWidget::hideTip()
{
    bool fromButton = false;
    //if (sender() == m_closeButton) {
    //    fromButton = true;
    //}

    if (m_mouseIn && !fromButton) {
        return;
    }

    m_hiding = true;
    m_timeLine->setFrameRange(0, -height());
    m_timeLine->start();
}

void PassWidget::slotFinish()
{
    if (m_hiding) {
        hide();
        //emit tooltipClosed(this);
    } else {
        QTimer::singleShot(5000, this, SLOT(hideTip()));
    }
}

//void PassWidget::enterEvent(QEvent *event)
//{
//    m_mouseIn = true;
//    QWidget::enterEvent(event);
//}
//
//void PassWidget::leaveEvent(QEvent *event)
//{
//    m_mouseIn = false;
//    slotFinish();
//    QWidget::leaveEvent(event);
//}
//
//void PassWidget::resizeEvent(QResizeEvent *event)
//{
//    Q_UNUSED(event)
//    m_base->resize(size());
//}

void PassWidget::setTooltip(const QString &tip)
{
    m_tipLabel->setText(tip);
    setMinimumSize(m_base->sizeHint());
}

QSize PassWidget::sizeHint() const
{
    return m_base->sizeHint();
}
