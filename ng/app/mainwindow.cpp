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

#include "mainwindow.h"
#include "pluginloader.h"
#include "akuplugin.h"
#include "akutreeview.h"
#include "akutreemodel.h"
#include "infodialog.h"
#include "pluginsmodel.h"

#include <QListView>
#include <QSplitter>

#include <KVBox>
#include <KStandardAction>
#include <KApplication>
#include <KActionCollection>
#include <KConfigDialog>
#include <KLocale>
#include <KRecentFilesAction>
#include <KFileDialog>
#include <KConfigSkeleton>
#include <KPluginInfo>
#include <KPluginSelector>
#include <KServiceTypeTrader>
#include <KDebug>

MainWindow::MainWindow (QWidget* parent): KXmlGuiWindow (parent)
{
    KVBox *baseWidget = new KVBox(this);
    setCentralWidget(baseWidget);
    QSplitter *splitter = new QSplitter(baseWidget);

    m_model = new AkuTreeModel(QVector<QStringList>(), this);
    m_treeView = new AkuTreeView(splitter);
    m_treeView->setModel(m_model);

    setupActions();
    setupConnections();
    setupGUI(QSize(650,460));

    // connection for pluginloader
    PluginLoader *pluginLoader = new PluginLoader(this);
    connect(pluginLoader, SIGNAL(pluginsLoaded(AkuPlugin*, const KPluginInfo &)),
          this, SLOT(addPlugins(AkuPlugin*, const KPluginInfo &)));
    pluginLoader->loadPlugins();
    //
}

MainWindow::~MainWindow()
{
    m_recentFilesAction->saveEntries(KGlobal::config()->group("Recent Files"));
}

void MainWindow::setupActions()
{
    KStandardAction::open(this, SLOT(openDialog()), actionCollection());
    KStandardAction::quit(this, SLOT(close()), actionCollection());

    // Open Recent Files
    m_recentFilesAction = KStandardAction::openRecent(this, SLOT(load(const KUrl&)), actionCollection());
    m_recentFilesAction->setToolBarMode(KRecentFilesAction::MenuMode);
    m_recentFilesAction->setToolButtonPopupMode(QToolButton::DelayedPopup);
    m_recentFilesAction->loadEntries(KGlobal::config()->group("Recent Files"));
    //

    KAction* pluginsInfoAction = new KAction(this);
    pluginsInfoAction->setText(i18n("Show Plugins Information"));
    //pluginsInfoAction->setIcon(KIcon("document-new"));
    //pluginsInfoAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("pluginsinfo", pluginsInfoAction);
    connect(pluginsInfoAction, SIGNAL(triggered(bool)), this, SLOT(showPluginsInfo()));

}

void MainWindow::setupConnections()
{
}

void MainWindow::openDialog()
{
    KUrl url;
    KFileDialog fileDialog(QDir::homePath(), QString(), this);
    fileDialog.setOperationMode(KFileDialog::Opening);
    fileDialog.setCaption(i18n("Open"));
    fileDialog.setMimeFilter(m_mimeTypeNames);

    if (fileDialog.exec()) {
        url = fileDialog.selectedUrl();
    }

    if (!url.isEmpty()) {
        m_currentUrl = url;
        load(url);
    }
}

void MainWindow::addPlugins(AkuPlugin *plugin, const KPluginInfo &info)
{
    foreach (const QString &mimeName, plugin->mimeTypeNames()) {
        KMimeType::Ptr mime = KMimeType::mimeType(mimeName);
        m_mimeTypeNames << mimeName;
        kDebug() << mimeName;
    }
}

void MainWindow::load(const KUrl &url)
{
    m_recentFilesAction->addUrl(url);
}

void MainWindow::showPluginsInfo()
{
    KConfigSkeleton *configSkeleton = new KConfigSkeleton(KGlobal::config(), this);
    InfoDialog *infoDialog = new InfoDialog(this, "Plugins Information", configSkeleton);
    infoDialog->setAttribute(Qt::WA_DeleteOnClose);
    
    KPluginSelector *selector = new KPluginSelector(infoDialog);
    KService::List offers = KServiceTypeTrader::self()->query("Aku/Plugin");
    kDebug() << offers;
    QList<KPluginInfo> plugins = KPluginInfo::fromServices(offers);
    selector->addPlugins(plugins);

    infoDialog->addPage(selector, i18n("Aku plugins"), "preferences-plugin");
    infoDialog->exec();
}


