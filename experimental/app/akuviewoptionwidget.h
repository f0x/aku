 /* This file is part of the KDE project

   Copyright (C) 2008 Francesco Grieco <fgrieco@gmail.com>
   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
#ifndef AKUVIEWOPTIONWIDGET_H
#define AKUVIEWOPTIONWIDGET_H

#include <KVBox>
#include "ui_akuviewoptionwidget.h"

class AkuTreeView;
class AkuIconView;

class AkuViewOptionWidget : public KVBox
{
    Q_OBJECT
    public:
        AkuViewOptionWidget(QWidget *parent = 0);
        ~AkuViewOptionWidget();

        void setViews(AkuTreeView*, AkuIconView*);

    private:
        Ui::AkuViewOptionWidget ui;
        AkuIconView *m_iconView;
        AkuTreeView *m_treeView;

    protected slots:
        void showView(int index);
        void setIconSize(int);

    signals:
        void visibilityChange();
};

#endif
