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
class AkuIconWidget;

class AkuViewOptionWidget : public KVBox
{
    Q_OBJECT
    public:
        AkuViewOptionWidget(QWidget *parent = 0);
        ~AkuViewOptionWidget();

        void setViews(AkuTreeView*, AkuIconWidget*);

    private:
        Ui::AkuViewOptionWidget ui;
        AkuIconWidget *m_iconWidget;
        AkuTreeView *m_treeView;

        void loadSettings();
        void saveSettings();

    protected slots:
        void showView(int index);
        void showConfigBox(int);
        void setIconSize(int);

    public slots:
        void applySettings();
        void restoreDefaults();

    signals:
        void visibilityChange();
};

#endif
