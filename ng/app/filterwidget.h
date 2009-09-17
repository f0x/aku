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

class KFilterProxySearchLine;
class KAction;
class QToolButton;

class FilterWidget : public QWidget
{
public:
    FilterWidget(QWidget *parent);
    ~FilterWidget();

    /**
     * @return an useful action to be used as
     * menu entry to show/hide this widget.
     */
    KAction *action();
private:
    KFilterProxySearchLine *m_filterLine;
    KAction *m_action;
    QToolButton *m_hideButton;
};

#endif // FILTERWIDGET_H
