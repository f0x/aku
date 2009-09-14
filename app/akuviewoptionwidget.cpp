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
#include <KConfigGroup>
#include <KIconLoader>

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

    connect(ui.comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(showConfigBox(int)));
    ui.comboBox->setCurrentIndex(3);

}

AkuViewOptionWidget::~AkuViewOptionWidget()
{}

void AkuViewOptionWidget::loadSettings()
{
    KConfigGroup viewOptionConfig = KGlobal::config()->group("Views Option");

    if (!viewOptionConfig.exists()) {
        viewOptionConfig.writeEntry("Active View", "TreeView");
        viewOptionConfig.writeEntry("IconView item size", 
                                    KIconLoader::global()->currentSize(KIconLoader::Desktop));
    }

    // here we load the active view
    int view = 0;
    if (viewOptionConfig.readEntry("Active View", "TreeView") == "IconView") {
        view = 1;
    }
    showView(view);

    // icon settings
    m_iconWidget->view()->setIconSize(viewOptionConfig.readEntry("IconView item size",
                                      KIconLoader::global()->currentSize(KIconLoader::Desktop) ));
    ui.iconSizeSlider->setValue(m_iconWidget->view()->iconSize());


}

void AkuViewOptionWidget::saveSettings()
{
    KConfigGroup viewOptionConfig = KGlobal::config()->group("Views Option");

    // active view settings
    QString currentView = "TreeView";
    if (ui.comboBox->currentIndex() == 1) {
        currentView = "IconView";
    }
    viewOptionConfig.writeEntry("Active View", currentView);

    viewOptionConfig.writeEntry("IconView item size", ui.iconSizeSlider->value());

}

void AkuViewOptionWidget::restoreDefaults()
{
    KConfigGroup viewOptionConfig = KGlobal::config()->group("Views Option");

    viewOptionConfig.deleteEntry("Active View");
    viewOptionConfig.deleteEntry("IconView item size");

    loadSettings();
}

void AkuViewOptionWidget::applySettings()
{
    saveSettings();
    loadSettings();
}

void AkuViewOptionWidget::showConfigBox(int box)
{
    ui.iconBox->setVisible((bool)box);
    ui.treeBox->setVisible(!(bool)box);
}

void AkuViewOptionWidget::showView(int view)
{
    if (!m_iconWidget || !m_treeView) {
        return;
    }

    m_iconWidget->setVisible((bool)view);
    m_treeView->setVisible(!(bool)view);

    ui.comboBox->setCurrentIndex(view);

    emit visibilityChange();
}

void AkuViewOptionWidget::setViews(AkuTreeView *tree, AkuIconWidget *icon)
{
    m_iconWidget = icon;
    m_treeView = tree;

    loadSettings();
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
