 /*

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
#include "akuiconwidget.h"
#include "akuiconview.h"

#include <QLabel>
#include <QToolButton>

#include <KHBox>
#include <KIcon>

AkuIconWidget::AkuIconWidget(QWidget *parent) : KVBox(parent)
{
    KHBox *navigator = new KHBox(this);
    QToolButton *upButton = new QToolButton(KHBox);
    m_currentPathLabel = new QLabel(KHBox);

    m_view = new AkuIconView(this);
}

AkuIconWidget::~AkuIconWidget()
{}
