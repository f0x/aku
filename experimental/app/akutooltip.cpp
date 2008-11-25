 /* This file is part of the KDE project

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/

#include "akutooltip.h"
#include "akutooltipmanager.h"

#include <QPalette>
#include <QLabel>
#include <QToolButton>
#include <QTimeLine>
#include <QTimer>

#include <KPushButton>
#include <KIconLoader>
#include <KLocale>
#include <KHBox>
#include <KDebug>

const int DURATION = 750; // ms

AkuTooltip::AkuTooltip(QWidget *parent) : QWidget(parent),
                                          m_mouseIn(false)
{
//     setAttribute(Qt::WA_DeleteOnClose);

    m_base = new KHBox(this);
    m_base->setAutoFillBackground(true);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QPalette p = m_base->palette();
    p.setColor(QPalette::Window, Qt::white);
    p.setColor(QPalette::WindowText, Qt::black);
    m_base->setPalette(p);

    QLabel *icon = new QLabel(m_base);
    icon->setPixmap(KIconLoader::global()->loadIcon("dialog-information", KIconLoader::Small));

    m_tipLabel = new QLabel(m_base);
    m_tipLabel->setWordWrap(true);
    m_tipLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_tipLabel->setPalette(p);

    m_closeButton = new QToolButton(m_base);
    m_closeButton->setIcon(KIcon("dialog-close"));
    m_closeButton->setAutoRaise(true);
    connect(m_closeButton, SIGNAL(clicked()), this, SLOT(hideTip()));

    m_timeLine = new QTimeLine(DURATION, this);
    connect(m_timeLine, SIGNAL(frameChanged(int)), this, SLOT(animate(int)));
    connect(m_timeLine, SIGNAL(finished()), this, SLOT(slotFinish()));

    m_base->setGeometry(0, -height(), width(), height());
    hide();

    AkuTooltipManager::instance()->registerTooltip(this, parent);
}

AkuTooltip::~AkuTooltip()
{
}

void AkuTooltip::animate(int y)
{
    m_base->setGeometry(0, y, width(), height());
}

void AkuTooltip::showTip()
{
    show();
    m_hiding = false;

    if (m_timeLine->state() == QTimeLine::Running) {
        m_timeLine->stop();
    }

    m_timeLine->setFrameRange(-height(), 0);
    m_timeLine->start();
}

void AkuTooltip::hideTip()
{
    bool fromButton = false;
    if (sender() == m_closeButton) {
        fromButton = true;
    }

    if (m_mouseIn && !fromButton) {
        return;
    }

    m_hiding = true;
    m_timeLine->setFrameRange(0, -height());
    m_timeLine->start();
}

void AkuTooltip::slotFinish()
{
    if (m_hiding) {
        hide();
        emit tooltipClosed(this);
    } else {
        QTimer::singleShot(5000, this, SLOT(hideTip()));
    }
}

void AkuTooltip::enterEvent(QEvent *event)
{
    m_mouseIn = true;
    QWidget::enterEvent(event);
}

void AkuTooltip::leaveEvent(QEvent *event)
{
    m_mouseIn = false;
    slotFinish();
    QWidget::leaveEvent(event);
}

void AkuTooltip::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    m_base->resize(size());
}

void AkuTooltip::setTooltip(const QString &tip)
{
    m_tipLabel->setText(tip);
    setMinimumSize(m_base->sizeHint());
}

QSize AkuTooltip::sizeHint() const
{
    return m_base->sizeHint();
}
