 /* This file is part of the KDE project

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
#ifndef AKUTOOLTIP_H
#define AKUTOOLTIP_H

#include <QWidget>
class QLabel;
class QToolButton;
class QTimeLine;

class AkuTooltip : public QWidget
{
    Q_OBJECT
    public:
        AkuTooltip(QWidget *parent);
        ~AkuTooltip();

    public slots:
        void showTip();
        void hideTip();
        void setTooltip(const QString &);

    private:
        QWidget *m_base;
        QLabel *m_tipLabel;
        QToolButton *m_closeButton;
        QTimeLine *m_timeLine;
        bool m_hiding;

    protected slots:
        void animate(int);
        void slotFinish();

    protected:
        void resizeEvent(QResizeEvent*);

};

#endif
