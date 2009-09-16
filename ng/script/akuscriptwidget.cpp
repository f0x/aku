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
#include "akuscriptwidget.h"
#include "scriptobject.h"

#include <QLabel>
#include <QSpacerItem>
#include <QSplitter>
#include <QFile>
#include <QTextStream>
#include <QtScript/QScriptEngine>
#include <QtScript/QScriptContext>

#include <KTextEdit>
#include <kparts/part.h>
#include <kparts/componentfactory.h>
#include <KDebug>
#include <KLocale>
#include <KHBox>
#include <KPushButton>
#include <KIcon>
#include <KFileDialog>

AkuScriptWidget::AkuScriptWidget(QWidget *parent) : KVBox(parent),
m_textEdit(0),
m_output(0),
m_part(0),
m_scriptUrl(0),
m_runButton(0),
m_openButton(0),
m_scriptBox(0),
m_scriptEngine(0)
{
    initScriptEngine();

    QSplitter *splitter = new QSplitter(Qt::Vertical, this);

    m_scriptBox = new KVBox(splitter);
    QLabel *scriptLabel = new QLabel(m_scriptBox);
    QFont font = scriptLabel->font();
    font.setBold(true);
    scriptLabel->setFont(font);
    scriptLabel->setText(i18n("Script:"));
    KVBox *base = new KVBox(m_scriptBox);
//    m_part =
//            KParts::ComponentFactory::createPartInstanceFromQuery<KParts::ReadWritePart>( "application/x-shellscript", QString(), base, base);
    if (!m_part) {
        kDebug() << "no valid part found";
        delete base;
        m_textEdit = new KTextEdit(m_scriptBox);
    }

    KHBox *bbox = new KHBox(m_scriptBox);
    QWidget *spacer = new QWidget(bbox);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    m_openButton = new KPushButton(KIcon("document-open"), i18n("Open..."), bbox);
    connect (m_openButton, SIGNAL(clicked()), this, SLOT(open()));
    m_runButton = new KPushButton(KIcon("system-run"), i18n("Run script"), bbox);
    connect (m_runButton, SIGNAL(clicked()), this, SLOT(doScriptEvaluation()));

    KVBox *outputBox = new KVBox(splitter);
    QLabel *outputLabel = new QLabel(outputBox);
    outputLabel->setFont(font);
    outputLabel->setText(i18n("Output:"));

    m_output = new KTextEdit(outputBox);
    m_output->setTextInteractionFlags(Qt::TextBrowserInteraction);

    splitter->addWidget(m_scriptBox);
    splitter->addWidget(outputBox);
}

AkuScriptWidget::~AkuScriptWidget()
{}

void AkuScriptWidget::open(const KUrl &url)
{
    if (!url.isValid()) {
        return;
    }

    if (m_part) {
        if (m_part->openUrl(url)) {
            return;
        }
    }

    delete m_part;
    m_part = 0;
    if (!m_textEdit) {
        m_textEdit = new KTextEdit(m_scriptBox);
    }
    QFile file(url.path());
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }
    m_scriptUrl = url;
    QTextStream stream(&file);
    m_textEdit->setText(stream.readAll());

    file.close();
}

void AkuScriptWidget::open()
{
    open(KFileDialog::getOpenUrl());
}

void AkuScriptWidget::initScriptEngine()
{
    if (m_scriptEngine) {
        return;
    }

    m_scriptEngine = new QScriptEngine(this);
    QScriptValue objValue = m_scriptEngine->newQObject(new ScriptObject);
    m_scriptEngine->globalObject().setProperty("Aku", objValue);
}

void AkuScriptWidget::doScriptEvaluation()
{
    if (!m_scriptEngine) {
        return;
    }

    if (m_textEdit) {
        QScriptValue result = m_scriptEngine->evaluate(m_textEdit->toPlainText());
        m_output->setText(result.toString());
    }
}
