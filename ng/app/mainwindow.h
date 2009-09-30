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

#include "akuplugin.h"

#include <KXmlGuiWindow>
#include <KUrl>

class AkuStatusBar;
class AkuTreeModel;
class AkuTreeView;
class CommentWidget;
class FilterWidget;
class KAction;
class KActionMenu;
class KPluginInfo;
class KRecentFilesAction;
class KVBox;
class MetaWidget;
class PassWidget;
class QDockWidget;
class QModelIndex;
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

    QAction *m_actionMain;
    QAction *m_actionError;
    QAction *m_actionComment;
    QString m_currentPlugin;
    QString m_password;
    QStringList m_mimeTypeNames;
    QMap<QString, AkuPlugin*> m_plugins;
    QDockWidget *m_infoDock;

    AkuTreeModel *m_model;
    AkuTreeView *m_treeView;
    CommentWidget *m_commentWidget;
    FilterWidget *m_filterWidget;
    MetaWidget *m_metaWidget;
    PassWidget *m_passwordWidget;
    SortFilterModel *m_sortFilterModel;
    AkuStatusBar *m_statusBar;

    KActionMenu *m_actionExtract;
    KAction *m_actionLock;
    KRecentFilesAction *m_recentFilesAction;
    KUrl m_currentUrl;

    bool filterWidgetIsVisible;

  public slots:
    void load(const KUrl);

  private slots:
    void addPlugins(AkuPlugin*, const KPluginInfo &);
    void configureAku();
    void dataMetaWidget(QModelIndex);
    void extract(const KUrl &destination);
    void extractDialog();
    void handleError(const QString &);
    void loadSettings();
    void openDialog();
    void recentDirData();
    void selectionChanged(const QModelIndex &current, const QModelIndex &previous);
    void showArchiveContent(const AkuData &akudata);
    void showPluginsInfo();
    void tabChanged(QAction *);
    void pluginStateChanged();
    void getPassword(const QString &);
    void lockArchive();
};

#endif
