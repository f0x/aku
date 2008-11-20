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
class AkuTooltipManager;

class AkuTooltip : public QWidget
{
    Q_OBJECT
    public:
        AkuTooltip(QWidget *parent);
        ~AkuTooltip();

        QSize sizeHint() const;

    public slots:
        void setTooltip(const QString &);

    private slots:
        void showTip();
        void hideTip();

    private:
        QWidget *m_base;
        QLabel *m_tipLabel;
        QToolButton *m_closeButton;
        QTimeLine *m_timeLine;
        bool m_hiding;
        bool m_mouseIn;
        friend class AkuTooltipManager;

    signals:
        void tooltipClosed(AkuTooltip *);

    protected slots:
        void animate(int);
        void slotFinish();

    protected:
        void resizeEvent(QResizeEvent*);
        void enterEvent(QEvent *event);
        void leaveEvent(QEvent *event);

};

#endif
