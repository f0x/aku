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

#ifndef PASSWIDGET_H
#define PASSWIDGET_H

#include <QSize>
#include <QLabel>
#include <QPaintEvent>
#include <QPalette>
#include <QTimer>
#include <QBrush>
#include <QHBoxLayout>
#include <QPainter>
#include <QPixmap>
#include <QCheckBox>

#include <KPushButton>
#include <KAction>
#include <KVBox>
#include <KLocale>
#include <KIconLoader>
#include <KIcon>
#include <KLineEdit>

class PassWidget : public QWidget
{
  Q_OBJECT

public:
    PassWidget(QWidget *parent = 0);
    ~PassWidget();

    virtual QSize sizeHint() const;
    KAction* actionTip();

private:
    class PassWidgetPrivate;
    PassWidgetPrivate *d;

public slots:
    void buttonPressed();
    void show();
    virtual void setTip(const QString&);

protected slots:
    void gradualShow();
    void gradualHide();
    void startHide();
};

class PassWidget::PassWidgetPrivate
{

public:
    PassWidgetPrivate() : size(QSize(800,22)),
                          count(0),
                          box(0),
                          lineEdit(0), checkBox(0),
                          okButton(0), closeButton(0)
                          {}

    QSize size;
    int count;
    QWidget *box;
    KAction *actionTip;
    KLineEdit *lineEdit;
    QCheckBox *checkBox;
    KPushButton *okButton;
    KPushButton *closeButton;
    QTimer *closeTimer;
};

#endif
