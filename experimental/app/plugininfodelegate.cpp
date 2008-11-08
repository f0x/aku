/* This file is part of the KDE project

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/ 

#include "plugininfodelegate.h"

#include <QStandardItem>
#include <QStyleOptionViewItem>
#include <QFontMetrics>
#include <QApplication>
#include <QPainter>
#include <QLinearGradient>

#include <KIcon>
#include <KDebug>

#define ICON_SIZE 48

PluginInfoDelegate::PluginInfoDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

PluginInfoDelegate::~PluginInfoDelegate()
{}

void PluginInfoDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
    QStyleOptionViewItemV4 opt(option);

    if (index.column() > 0) {
        QStyledItemDelegate::paint(painter, opt, index);
        return;
    }


    QStyle *style = QApplication::style();
    style->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter, opt.widget);

    QRect clipRect;

    // the icon
    painter->save();
    clipRect = QRect(option.rect.topLeft(), QSize(qMin(ICON_SIZE, option.rect.width()), ICON_SIZE));
    QRect iconRect(option.rect.topLeft(), QSize(ICON_SIZE, ICON_SIZE));

    KIcon icon(index.data(PluginIconRole).toString());
    painter->setClipRect(clipRect);
    QIcon::Mode iconMode = QIcon::Normal;

    if (opt.state & QStyle::State_MouseOver) {
        iconMode = QIcon::Active;
    }

    icon.paint(painter, iconRect, Qt::AlignCenter, iconMode);
    painter->restore();

    // the main title
    painter->save();
    clipRect = QRect(iconRect.right(), iconRect.top(), option.rect.width() - iconRect.width(), option.rect.height() / 2);
    QFont titleFont = opt.font;
    titleFont.setBold(true);
    titleFont.setPointSize(titleFont.pointSize() + 2);
    painter->setFont(titleFont);
    painter->drawText(clipRect, Qt::AlignLeft | Qt::AlignBottom, index.data(PluginDescriptionRole).toString());
    painter->restore();

    // the plugin comment
    painter->save();
    clipRect = QRect(iconRect.right(), clipRect.bottom(), option.rect.width() - iconRect.width(), option.rect.height() / 2);
    titleFont = opt.font;
    titleFont.setPointSize(titleFont.pointSize() - 1);
    painter->setFont(titleFont);
    painter->drawText(clipRect, Qt::AlignLeft | Qt::AlignVCenter, index.data().toString());
    painter->restore();

//     QLinearGradient gradient(opt.rect.right() - 35, 0, opt.rect.right(), 0);
//     gradient.setColorAt(0, Qt::transparent);
//     gradient.setColorAt(1, Qt::white);
//     painter->save();
//     painter->setCompositionMode(QPainter::CompositionMode_DestinationOver);
//     painter->fillRect(opt.rect, gradient);
//     painter->restore();

}

QSize PluginInfoDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QFontMetrics fm(QApplication::font());
    QSize size = QSize( 0,
                         ICON_SIZE + 2*option.rect.y());

    return size;
}
