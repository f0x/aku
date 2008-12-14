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
#include <KLocale>

AkuIconWidget::AkuIconWidget(QWidget *parent) : KVBox(parent)
{
    KHBox *navigator = new KHBox(this);
    QToolButton *upButton = new QToolButton(navigator);
    upButton->setIcon(KIcon("go-up"));
    upButton->setAutoRaise(true);
    upButton->setToolTip(i18n("Parent Folder"));
    QToolButton *backButton = new QToolButton(navigator);
    backButton->setIcon(KIcon("go-previous"));
    backButton->setAutoRaise(true);
    backButton->setToolTip(i18n("Back"));
    QToolButton *forwardButton = new QToolButton(navigator);
    forwardButton->setIcon(KIcon("go-next"));
    forwardButton->setAutoRaise(true);
    forwardButton->setToolTip(i18n("Forward"));
    QToolButton *rootButton = new QToolButton(navigator);
    rootButton->setIcon(KIcon("go-home"));
    rootButton->setAutoRaise(true);
    rootButton->setToolTip(i18n("Root Folder"));
    
    
    m_currentPathLabel = new QLabel(navigator);

    m_view = new AkuIconView(this);
    connect(upButton, SIGNAL(clicked()), m_view, SLOT(browseUp()));
    //connect(backButton, SIGNAL(clicked()), m_view, SLOT(browseRoot()));
    //connect(forwardButton, SIGNAL(clicked()), m_view, SLOT(browseRoot()));
    //connect(rootButton, SIGNAL(clicked()), m_view, SLOT(browseRoot()));
}

AkuIconWidget::~AkuIconWidget()
{}

AkuIconView* AkuIconWidget::view()
{
    return m_view;
}
