/***************************************************************************
 *   Copyright 2009 by Francesco Grieco <fgrieco@gmail.com>                *
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

#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include <QWidget>

class QToolButton;
class QCheckBox;

class KAction;
class KComboBox;
class KLineEdit;
class QSortFilterProxyModel;

class FilterWidget : public QWidget
{
    Q_OBJECT

public:
    FilterWidget(QWidget *parent, QSortFilterProxyModel*);
    ~FilterWidget();

    /**
     * @return an useful action to be used as
     * menu entry to show/hide this widget.
     */
    KAction *action();

private:
    void setupConnections();

    QSortFilterProxyModel *m_model;

    KLineEdit *m_filterLine;
    KAction *m_action;
    KComboBox *m_filterComboBox;

    QToolButton *m_hideButton;
    QCheckBox *m_checkBox;

private slots:
    void textFilterChanged();

signals:
    void filterChanged(QRegExp);
};

#endif // FILTERWIDGET_H
