/***************************************************************************
 *   Copyright 2009 by Alessandro Diaferia <alediaferia@gmail.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#include "akutreeview.h"
#include "akutreemodel.h"

#include <QVector>
#include <QHeaderView>
#include <QContextMenuEvent>
#include <QDir>

#include <KLocale>
#include <KAction>
#include <KMenu>
#include <KDebug>

AkuTreeView::AkuTreeView(QWidget *parent) : QTreeView(parent)
{
    setAlternatingRowColors(true);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    header()->setResizeMode(QHeaderView::ResizeToContents);

    QHeaderView* headerView = header();
    headerView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(headerView, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(headerMenu(const QPoint&)));
}

AkuTreeView::~AkuTreeView()
{
}

QStringList AkuTreeView::selectedPaths()
{
    QStringList paths;
    foreach (const QModelIndex &index, selectedIndexes()) {

        if (index.column() > 0) {
            continue;
        }

        QString path;
        path = index.data().toString();

        QModelIndex parent = index.parent();
        while (parent.isValid()) {
            path.prepend(parent.data().toString() + QDir::separator());
            parent = parent.parent();
        }
        paths << path;
    }

    return paths;
}

 void AkuTreeView::contextMenuEvent(QContextMenuEvent *event)
 {
    KAction *actionSelectAll = new KAction(i18n("Select all"),this);
    actionSelectAll->setShortcut(Qt::CTRL + Qt::Key_A);
    KAction *actionInvertSelection = new KAction(i18n("Invert selection"),this);
    actionInvertSelection->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_A);
    KAction *actionExpandAll = new KAction(i18n("Expand all the items"),this);
    KAction *actionCollapseAll = new KAction(i18n("Collapse all the items"),this);

    connect(actionSelectAll, SIGNAL(triggered()), this, SLOT(selectAll()));
    connect(actionInvertSelection, SIGNAL(triggered()), this, SLOT(invertSelection()));
    connect(actionExpandAll, SIGNAL(triggered()), this, SLOT(expandAll()));
    connect(actionCollapseAll, SIGNAL(triggered()), this, SLOT(collapseAll()));

    KMenu menu(this);
    menu.addTitle(i18n("Quick actions"));
    menu.addAction(actionSelectAll);
    menu.addAction(actionInvertSelection);
    menu.addSeparator();
    menu.addAction(actionExpandAll);
    menu.addAction(actionCollapseAll);
    menu.exec(event->globalPos());
}

void AkuTreeView::invertSelection()
{
}

void AkuTreeView::modelForHeader(AkuTreeModel *model)
{
    m_model = model;
}

void AkuTreeView::headerMenu(const QPoint& pos)
{
    // Dolphin code :D
    KMenu popup(this);
    popup.addTitle(i18nc("@title:menu", "Columns"));

    // add checkbox items for each column
    QHeaderView* headerView = header();
    const int columns = m_model->columnCount();
    // Disable File Name column
    for (int i = 1; i < columns; ++i) {
        const int logicalIndex = headerView->logicalIndex(i);
        const QString text = m_model->headerData(logicalIndex, Qt::Horizontal).toString();
        QAction* action = popup.addAction(text);
        action->setCheckable(true);
        action->setChecked(!headerView->isSectionHidden(logicalIndex));
        action->setData(logicalIndex);
    }
    popup.addSeparator();

    QAction* activatedAction = popup.exec(header()->mapToGlobal(pos));
    if (activatedAction != 0) {
        const bool show = activatedAction->isChecked();
        const int columnIndex = activatedAction->data().toInt();
        setColumnHidden(columnIndex, !show);
    }

}
