 /* This file is part of the KDE project

   Copyright (C) 2008 Francesco Grieco <fgrieco@gmail.com>
   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/

#ifndef AKUTREEVIEW_H
#define AKUTREEVIEW_H

#include <QTreeView>

class AkuTreeModel;

class AkuTreeView : public QTreeView
{
  Q_OBJECT

  public:
    AkuTreeView(QWidget *parent);
    ~AkuTreeView();

    AkuTreeModel* model();

};

#endif