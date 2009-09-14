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
#include <KIconLoader>
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

//     if (index.column() > 0) {
//         QStyledItemDelegate::paint(painter, opt, index);
//         return;
//     }


    QStyle *style = QApplication::style();
    style->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter, opt.widget);

    switch (index.column()) {
    case 0 :
        paintMainCol(painter, opt, index);
        break;
    default:
        paintPropertyCol(painter, opt, index);
        break;
    }

}

void PluginInfoDelegate::paintMainCol(QPainter *painter, const QStyleOptionViewItemV4 &opt,
                         const QModelIndex &index) const
{
    QPixmap pixmap(opt.rect.size());
    pixmap.fill(Qt::transparent);
    QPainter p(&pixmap);
    p.translate(-opt.rect.topLeft());

    QRect clipRect;

    // the icon
    p.save();
    clipRect = QRect(opt.rect.topLeft(), QSize(qMin(ICON_SIZE, opt.rect.width()), ICON_SIZE));
    QRect iconRect(opt.rect.topLeft(), QSize(ICON_SIZE, ICON_SIZE));

    KIcon icon(index.data(PluginIconRole).toString());
    p.setClipRect(clipRect);
    QIcon::Mode iconMode = QIcon::Normal;

    if (opt.state & QStyle::State_MouseOver) {
        iconMode = QIcon::Active;
    }

    icon.paint(&p, iconRect, Qt::AlignCenter, iconMode);
    p.restore();

    QColor foregroundColor = (opt.state.testFlag(QStyle::State_Selected))?
        opt.palette.color(QPalette::HighlightedText):opt.palette.color(QPalette::Text);

    p.setPen(foregroundColor);

    // the main title
    p.save();
    clipRect = QRect(iconRect.right(), iconRect.top(), opt.rect.width() - iconRect.width(), opt.rect.height() / 2);
    QFont titleFont = opt.font;
    titleFont.setBold(true);
    titleFont.setPointSize(titleFont.pointSize() + 2);
    p.setFont(titleFont);
    p.drawText(clipRect, Qt::AlignLeft | Qt::AlignBottom, index.data(PluginDescriptionRole).toString());
    p.restore();

    // the plugin comment
    p.save();
    clipRect = QRect(iconRect.right(), clipRect.bottom(), opt.rect.width() - iconRect.width(), opt.rect.height() / 2);
    titleFont = opt.font;
    titleFont.setPointSize(titleFont.pointSize() - 1);
    p.setFont(titleFont);
    p.drawText(clipRect, Qt::AlignLeft | Qt::AlignVCenter, index.data().toString());
    p.restore();

    // a gradient to fade out
    QLinearGradient gradient(opt.rect.right(), 0, opt.rect.right() -35, 0); // FIXME: 35 shoudln't be so magic
    gradient.setColorAt(0, Qt::transparent);
    gradient.setColorAt(1, Qt::white);
    p.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    p.fillRect(opt.rect, gradient);
    p.end();

    painter->drawPixmap(opt.rect.topLeft(), pixmap);
}

void PluginInfoDelegate::paintPropertyCol(QPainter *painter, const QStyleOptionViewItemV4 &opt,
                         const QModelIndex &index) const
{
    // true = dialog-ok-apply
    // false = edit-delete

    QString iconName = "edit-delete";
    if (index.data(PluginPropertyRole).toBool()) {
        iconName = "dialog-ok-apply";
    }

    KIconLoader::States state = KIconLoader::DefaultState;
    if (opt.state & QStyle::State_MouseOver) {
        state = KIconLoader::ActiveState;
    }

    QPixmap icon = KIconLoader::global()->loadIcon(iconName, KIconLoader::Small, 0, state);

    QRect iconRect;
    iconRect.setSize(icon.size());
    iconRect.moveCenter(opt.rect.center());

    painter->drawPixmap(iconRect.topLeft(), icon);
}

QSize PluginInfoDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QFontMetrics fm(QApplication::font());
    // TODO: do a real height calculation based on font sizes used
    //       (+2 the title, -1 the description)
    QSize size = QSize( 0,
                         ICON_SIZE + 2*option.rect.y());

    return size;
}
