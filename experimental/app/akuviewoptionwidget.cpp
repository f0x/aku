 /* This file is part of the KDE project

   Copyright (C) 2008 Francesco Grieco <fgrieco@gmail.com>
   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/

#include "akuviewoptionwidget.h"

#include "akutreeview.h"
#include "akuiconview.h"
#include "akuiconwidget.h"

#include <KIcon>
#include <KLocale>
#include <KDebug>

AkuViewOptionWidget::AkuViewOptionWidget(QWidget *parent) : KVBox(parent),
                                                            m_iconWidget(0),
                                                            m_treeView(0)
{
    QWidget *widget = new QWidget(this);
    ui.setupUi(widget);

    ui.comboBox->insertItem(0, i18n("Tree View"));
    ui.comboBox->insertItem(1, i18n("Icon View"));

    ui.iconSizeSlider->setMinimum(16);
    ui.iconSizeSlider->setMaximum(256);

    ui.zoomOutButton->setIcon(KIcon("zoom-out"));
    ui.zoomInButton->setIcon(KIcon("zoom-in"));

    connect(ui.comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(showView(int)));
    connect(ui.iconSizeSlider, SIGNAL(sliderMoved(int)), this, SLOT(setIconSize(int)));

}

AkuViewOptionWidget::~AkuViewOptionWidget()
{}

void AkuViewOptionWidget::showView(int view)
{
    if (!m_iconWidget || !m_treeView) {
        return;
    }

    bool show = view;
    m_iconWidget->setVisible(show);
    ui.iconBox->setVisible(show);
    m_treeView->setVisible(!show);
    ui.treeBox->setVisible(!show);

    emit visibilityChange();
}

void AkuViewOptionWidget::setViews(AkuTreeView *tree, AkuIconWidget *icon)
{
    m_iconWidget = icon;
    m_treeView = tree;

    showView(ui.comboBox->currentIndex());
    ui.iconSizeSlider->setSliderPosition(m_iconWidget->view()->viewItemSize());
}

void AkuViewOptionWidget::setIconSize(int size)
{
    if (!m_iconWidget) {
        return;
    }

// TODO: only fix KDE icon sizes

    m_iconWidget->view()->setIconSize(size);
}

#include "akuviewoptionwidget.moc"
