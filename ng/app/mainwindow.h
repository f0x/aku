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
class ErrorWidget;
class FilterWidget;
class KAction;
class KActionMenu;
class KPluginInfo;
class KRecentFilesAction;
class KVBox;
class MetaWidget;
class OverwriteWidget;
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

    AkuStatusBar *m_statusBar;
    AkuTreeModel *m_model;
    AkuTreeView *m_treeView;
    CommentWidget *m_commentWidget;
    ErrorWidget *m_errorWidget;
    FilterWidget *m_filterWidget;
    MetaWidget *m_metaWidget;
    OverwriteWidget *m_overwriteWidget;
    PassWidget *m_passwordWidget;
    SortFilterModel *m_sortFilterModel;

    KActionMenu *m_actionExtract;
    KActionMenu *m_actionAdd;
    KAction *m_actionLock;
    KRecentFilesAction *m_recentFilesAction;
    KUrl m_currentUrl;

  public slots:
    void load(const KUrl);

  private slots:
    void addDialog();
    void addFiles(const QStringList &, const QString &);
    void addPlugins(AkuPlugin*, const KPluginInfo &);
    void configureAku();
    void dataMetaWidget(QModelIndex);
    void extract(const KUrl &, AkuPlugin::ExtractionOptions);
    void extractDialog();
    void getPassword(const QString &);
    void handleError(AkuPlugin::ErrorType, const QString &);
    void loadSettings();
    void lockArchive();
    void openDialog();
    void pluginStateChanged();
    void preview();
    void recentDirData();
    void remove();
    void rename();
    void selectionChanged(const QModelIndex &current, const QModelIndex &previous);
    void showArchiveContent(const AkuData &akudata);
    void showPluginsInfo();
    void tabChanged(QAction *);

};

#endif
