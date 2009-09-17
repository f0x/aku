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

#include "filterwidget.h"

#include <QHBoxLayout>
#include <QToolButton>
#include <QCheckBox>

#include <KIcon>
#include <KFilterProxySearchLine>
#include <KLocale>

FilterWidget::FilterWidget(QWidget *parent) : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    filterLine = new KFilterProxySearchLine(this);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addSpacing(10);
    layout->addWidget(filterLine);

    QCheckBox *checkBox = new QCheckBox(i18n("Case sensitive"), this);
    layout->addWidget(checkBox);

    layout->addSpacing(50);

    QToolButton *hideButton = new QToolButton(this);
    hideButton->setAutoRaise(true);
    hideButton->setIcon(KIcon("dialog-close"));
    connect(hideButton, SIGNAL(clicked()), this, SLOT(hide()));
    layout->addWidget(hideButton);

    setLayout(layout);

    hide();
}

FilterWidget::~FilterWidget()
{
}
