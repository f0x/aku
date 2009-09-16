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
#ifndef AKUSCRIPTWIDGET_H
#define AKUSCRIPTWIDGET_H

#include <KVBox>
#include <KUrl>

class KTextEdit;
namespace KParts {
    class ReadWritePart;
}
class KPushButton;
class QScriptEngine;

class AkuScriptWidget : public KVBox
{
    Q_OBJECT
public:
    AkuScriptWidget(QWidget *parent = 0);
    ~AkuScriptWidget();

public slots:
    void open(const KUrl &);
    void open();
    void doScriptEvaluation();

protected:
    void initScriptEngine();

private:
    KTextEdit *m_textEdit;
    KTextEdit *m_output;
    KParts::ReadWritePart *m_part;
    KUrl m_scriptUrl;
    KPushButton *m_runButton;  // system-run
    KPushButton *m_openButton; // document-open
    KVBox *m_scriptBox;
    QScriptEngine *m_scriptEngine;
};

#endif // AKUSCRIPTWIDGET_H
