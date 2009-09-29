/***************************************************************************
 *   Copyright 2009 by Francesco Grieco <fgrieco@gmail.com>                *
 *                     Alessandro Diaferia <alediaferia@gmail.com>         *
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

#ifndef PASSWIDGET_H
#define PASSWIDGET_H

#include <QWidget>

class QLabel;
class QTimeLine;
class QVBoxLayout;
class KHBox;
class KLineEdit;

class PassWidget : public QWidget
{
    Q_OBJECT

public:
    PassWidget(QWidget *parent);
    ~PassWidget();

    QSize sizeHint() const;

    void setArchiveName(QString);

public slots:
    void askPassword();
    void clearPassword();

private slots:
    void hideWidget();
    void buttonPressed();

private:
    KHBox *m_base;
    KLineEdit *m_lineEdit;

    QTimeLine *m_timeLine;
    QLabel *m_filenameLabel;
    QLabel *m_warningIcon;
    QLabel *m_warningLabel;
    QString m_password;
    QVBoxLayout *m_layout;

    bool m_hiding;

protected slots:
    void animate(int);
    void slotFinish();

protected:
    void resizeEvent(QResizeEvent*);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

signals:
    void password(const QString &);
};

#endif
