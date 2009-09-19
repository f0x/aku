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
#include "sortfiltermodel.h"

#include <QHBoxLayout>
#include <QToolButton>
#include <QCheckBox>

#include <KIcon>
#include <KLocale>
#include <KAction>
#include <KComboBox>
#include <KLineEdit>

FilterWidget::FilterWidget(QWidget *parent, QSortFilterProxyModel *model) : QWidget(parent),
m_action(0),
m_hideButton(0)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_model = model;

    QHBoxLayout *layout = new QHBoxLayout;
    m_hideButton = new QToolButton(this);
    m_hideButton->setToolTip(i18n("Hide Filter Bar"));
    m_hideButton->setAutoRaise(true);
    m_hideButton->setIcon(KIcon("dialog-close"));
    layout->addWidget(m_hideButton);

    m_filterLine = new KLineEdit(this);
    m_filterLine->setClearButtonShown(true);
    m_filterLine->setClickMessage(i18n("Filter..."));
    layout->addWidget(m_filterLine);

    m_filterComboBox = new KComboBox;
    m_filterComboBox->addItem(i18n("Fixed string"), QRegExp::FixedString);
    m_filterComboBox->addItem(i18n("Wildcard"), QRegExp::Wildcard);
    m_filterComboBox->addItem(i18n("Regular expression"), QRegExp::RegExp);
    layout->addWidget(m_filterComboBox);

    m_checkBox = new QCheckBox(i18n("Case sensitive"), this);
    layout->addWidget(m_checkBox);
    layout->addSpacing(10);

    //layout->addSpacing(50);

    setLayout(layout);

    hide();

    setupConnections();
}

FilterWidget::~FilterWidget()
{
}

void FilterWidget::setupConnections()
{
    connect(m_filterLine, SIGNAL(textChanged(const QString &)), this, SLOT(textFilterChanged()));
    connect(m_hideButton, SIGNAL(clicked()), this, SLOT(hide()));
    connect(m_filterComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(textFilterChanged()));
    connect(m_checkBox, SIGNAL(toggled(bool)), this, SLOT(textFilterChanged()));
}


KAction* FilterWidget::action()
{
    if (!m_action) {
        m_action = new KAction(this);
        m_action->setText(i18n("Filter"));
        m_action->setIcon(KIcon("view-filter"));
        m_action->setShortcut(Qt::CTRL + Qt::Key_F);
        m_action->setCheckable(true);
        m_action->setChecked(isVisible());
        connect (m_action, SIGNAL(toggled(bool)), this, SLOT(setVisible(bool)));
        disconnect(m_hideButton);
        connect (m_hideButton, SIGNAL(clicked()), m_action, SLOT(toggle()));
    }

    return m_action;
}

void FilterWidget::textFilterChanged()
{
    QRegExp::PatternSyntax syntax =
            QRegExp::PatternSyntax(m_filterComboBox->itemData(m_filterComboBox->currentIndex()).toInt());
    Qt::CaseSensitivity caseSensitivity = m_checkBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;

    QRegExp regExp(m_filterLine->text(), caseSensitivity, syntax);
    m_model->setFilterRegExp(regExp);
}

void FilterWidget::clearFilterText()
{
    m_filterLine->clear();
}
