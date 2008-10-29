 /* This file is part of the KDE project

   Copyright (C) 2008 Francesco Grieco <fgrieco@gmail.com>
   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/

#ifndef AKUICONVIEW_H
#define AKUICONVIEW_H

#include <QListView>

class AkuTreeModel;

class AkuIconView : public QListView
{
    Q_OBJECT
    public:
        AkuIconView(QWidget *parent);
        ~AkuIconView();

        AkuTreeModel* model();
        void setIconSize(int size);
        int viewItemSize();

    private:
         int m_gridSize;
         int m_iconSize;

    public slots:
        void enterItem(const QModelIndex &index);
        void browseUp();
};

#endif
