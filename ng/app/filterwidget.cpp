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
#include <KAction>

FilterWidget::FilterWidget(QWidget *parent) : QWidget(parent),
m_action(0),
m_hideButton(0)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_filterLine = new KFilterProxySearchLine(this);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addSpacing(10);
    layout->addWidget(m_filterLine);

    QCheckBox *checkBox = new QCheckBox(i18n("Case sensitive"), this);
    layout->addWidget(checkBox);

    layout->addSpacing(50);

    m_hideButton = new QToolButton(this);
    m_hideButton->setAutoRaise(true);
    m_hideButton->setIcon(KIcon("dialog-close"));
    connect(m_hideButton, SIGNAL(clicked()), this, SLOT(hide()));
    layout->addWidget(m_hideButton);

    setLayout(layout);

    hide();
}

FilterWidget::~FilterWidget()
{
}

KAction* FilterWidget::action()
{
    if (!m_action) {
        m_action = new KAction(this);
        m_action->setText(i18n("Filter"));
        m_action->setShortcut(Qt::CTRL + Qt::Key_F);
        m_action->setCheckable(true);
        m_action->setChecked(isVisible());
        connect (m_action, SIGNAL(toggled(bool)), this, SLOT(setVisible(bool)));
        disconnect(m_hideButton);
        connect (m_hideButton, SIGNAL(clicked()), m_action, SLOT(toggle()));
    }

    return m_action;
}
