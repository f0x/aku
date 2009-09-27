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

#include <QWidget>

class QLabel;
class QToolButton;
class QTimeLine;

class PassWidget : public QWidget
{
    Q_OBJECT
    public:
        PassWidget(QWidget *parent);
        ~PassWidget();

        QSize sizeHint() const;

    public slots:
        void setTooltip(const QString &);

    private slots:
        void showTip();
        void hideTip();

    private:
        QWidget *m_base;
        QLabel *m_tipLabel;
        //QToolButton *m_closeButton;
        QTimeLine *m_timeLine;
        bool m_hiding;
        bool m_mouseIn;

    //signals:
    //    void tooltipClosed(PassWidget *);

    protected slots:
        void animate(int);
        void slotFinish();

    protected:
        void resizeEvent(QResizeEvent*);
      //  void enterEvent(QEvent *event);
      //  void leaveEvent(QEvent *event);

};

#endif
