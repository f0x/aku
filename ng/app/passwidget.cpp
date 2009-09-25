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

PassWidget::PassWidget(QWidget *parent) : QWidget(parent), d(new PassWidgetPrivate)
{
    d->box = new KVBox(this);

    QWidget *widget = new QWidget(d->box);
    widget->setAutoFillBackground(true);
    QPalette palette = widget->palette();
    palette.setColor(QPalette::Window, Qt::white);
    widget->setPalette(palette);

    QVBoxLayout *layout = new QVBoxLayout(widget);

    //d->toolTip = new QLabel(widget);
    //d->toolTip->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    //QPalette pt = d->toolTip->palette();
    //pt.setColor(QPalette::WindowText, Qt::black);
    //d->toolTip->setPalette(pt);
 
    QLabel *icon = new QLabel(widget);
    icon->setPixmap(KIconLoader::global()->loadIcon("dialog-password", KIconLoader::Small));
    icon->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QFont font;
    font.setBold(true);
    QLabel *label = new QLabel(widget);
    label->setFont(font);
    label->setText(i18n("The archive is header password protected"));

    d->lineEdit = new KLineEdit(widget);
    d->lineEdit->setClickMessage(i18n("Enter the password..."));
    d->lineEdit->setPasswordMode(true);
    d->lineEdit->setClearButtonShown(true);

    d->checkBox = new QCheckBox(i18n("Hidden"), widget);
    d->checkBox->setChecked(true);
    connect(d->checkBox, SIGNAL(clicked(bool)), d->lineEdit, SLOT(setPasswordMode(bool)));

    d->okButton = new KPushButton(KIcon("dialog-ok-apply"), i18n("Enter"), widget);
    d->closeButton = new KPushButton(KIcon("dialog-close"), i18n("Abort"), widget);

    QLabel *warning = new QLabel(widget);
    warning->setPixmap(KIconLoader::global()->loadIcon("emblem-important", KIconLoader::Small));
    warning->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    font.setBold(false);
    font.setUnderline(true);
    QLabel *labelWarning = new QLabel(widget);
    labelWarning->setFont(font);
    labelWarning->setText(i18n("The password is not correct. Try again"));

    QHBoxLayout *hline1 = new QHBoxLayout;
    layout->addLayout(hline1);
    hline1->setSpacing(10);
    hline1->addSpacing(10);
    hline1->addWidget(icon);
    hline1->addWidget(label);
    hline1->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed));

    QHBoxLayout *hline2 = new QHBoxLayout;
    layout->addLayout(hline2);
    hline2->setSpacing(10);
    hline2->addSpacing(30);
    hline2->addWidget(d->lineEdit);
    hline2->addWidget(d->checkBox);
    hline2->addSpacerItem(new QSpacerItem(30, 10, QSizePolicy::Fixed, QSizePolicy::Fixed));
    hline2->addWidget(d->okButton);
    hline2->addWidget(d->closeButton);

    QHBoxLayout *hline3 = new QHBoxLayout;
    layout->addLayout(hline3);
    hline3->setSpacing(10);
    hline3->addSpacing(10);
    hline3->addWidget(warning);
    hline3->addWidget(labelWarning);
    hline3->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed));

    QHBoxLayout *baseLayout = new QHBoxLayout(this);
    baseLayout->addWidget(d->box);

    //d->actionTip = new KAction(this);
    //d->actionTip -> setIcon(KIcon("lastmoves"));
    //d->actionTip -> setText(i18n("Last tip"));
    //d->actionTip -> setEnabled(false);
}

PassWidget::~PassWidget()
{
}

KAction* PassWidget::actionTip()
{
    return d->actionTip;
}

void PassWidget::setTip(const QString& tip)
{
    //d->toolTip->setText(tip);
    //d->actionTip->setEnabled(true);
}

void PassWidget::show()
{
    if(!isVisible()) {
        setVisible(true);
        QTimer *timer = new QTimer();
        connect(timer, SIGNAL(timeout()), this, SLOT(gradualShow()));
        timer->start(10);
        d->closeTimer = new QTimer();
        connect(d->closeTimer, SIGNAL(timeout()), this, SLOT(startHide()));
        d->closeTimer->start(6000);
    } else {
        setVisible(true);
        d->box->resize(d->box->size().width(), height());
        d->closeTimer->stop();
        d->closeTimer->start(6000);
        connect(d->closeTimer, SIGNAL(timeout()), this, SLOT(startHide()));
        //disconnect(d->closeTimer,0,this,0);
        //setVisible(true);
    }
}

void PassWidget::gradualShow()
{
    if(d->count <=100) {
        d->box->resize(d->box->size().width(), (d->count/100.0)*height());
        // puts(QString().setNum(size().height()).toAscii());
        d->count++;
    } else {
        d->box->resize(d->box->size().width(), height());
        disconnect(sender(), 0, this, 0);
        //connect(d->close, SIGNAL(clicked()), this, SLOT(startHide()));
        delete sender();
    }
}

void PassWidget::startHide()
{
    disconnect(sender(),0,this,0);
    QTimer *t = new QTimer();
    connect(t, SIGNAL(timeout()), this, SLOT(gradualHide()));
    t->start(10);
}

void PassWidget::gradualHide()
{
    if (d->count >= 0) {
        d->box->resize(d->box->size().width(), (d->count/100.0)*height());
        d->count--;
    } else {
        disconnect(sender(), 0, this, 0);
        delete sender();
        setVisible(false);
    }
}

QSize PassWidget::sizeHint() const
{
    return d->size;
}
