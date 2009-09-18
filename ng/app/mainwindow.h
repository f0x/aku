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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <KXmlGuiWindow>
#include <KUrl>

class AkuPlugin;
class AkuTreeModel;
class AkuTreeView;
class KPluginInfo;
class KRecentFilesAction;
class FilterWidget;
class MetaWidget;
class SortFilterModel;

class MainWindow : public KXmlGuiWindow
{
  Q_OBJECT

  public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();

  private:
    void setupActions();
    void setupConnections();
    void updateMetaWidget();

    QString m_currentPlugin;
    QStringList m_mimeTypeNames;
    QMap<QString, AkuPlugin*> m_plugins;

    AkuTreeModel *m_model;
    AkuTreeView *m_treeView;
    FilterWidget *m_filterWidget;
    MetaWidget *m_metaWidget;
    SortFilterModel *m_sortFilterModel;

    KRecentFilesAction *m_recentFilesAction;
    KUrl m_currentUrl;

  public slots:
    void load(const KUrl &);

  private slots:
    void openDialog();
    void addPlugins(AkuPlugin*, const KPluginInfo &);
    void showPluginsInfo();
    void configureAku();
    void showArchiveContent(const QVector<QStringList> &archive);
};

#endif
