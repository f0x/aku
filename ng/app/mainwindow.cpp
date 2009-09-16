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
//#include "infodialog.h"
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
#include <KMessageBox>
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
    KStandardAction::preferences(this, SLOT(configureAku()), actionCollection());

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
    connect(plugin, SIGNAL(archiveLoaded(const QVector<QStringList> &)),
            this, SLOT(showArchiveContent(const QVector<QStringList> &)));

    foreach (const QString &mimeName, plugin->mimeTypeNames()) {
        KMimeType::Ptr mime = KMimeType::mimeType(mimeName);

        m_plugins.insert(mime->name(), plugin);

        m_mimeTypeNames << mimeName;

        kDebug() << mimeName;
    }
}

void MainWindow::load(const KUrl &url)
{
    if (!QFile(url.pathOrUrl()).open(QIODevice::ReadOnly)) {
        KMessageBox::error(this, i18n("Could not open") + " %1.<br>" + i18n("Check your file permissions",
                                      url.prettyUrl()), i18n("Load error"));
        return;
    }

    KMimeType::Ptr mimetype = KMimeType::findByUrl(url);
    kDebug() << mimetype -> name();

    if (!m_plugins.contains(mimetype->name())) {
        KMessageBox::detailedSorry(this, i18n("Sorry, no available plugin to open") +  " <b>" + url.pathOrUrl() + "</b>",
                           i18n("Install a plugin for") + " <b>" + mimetype->name() + "</b> " +
                                 i18n("mimetype in order to load the archive."),
                           i18n("Unable to load the archive"));
        return;
    }

    m_currentPlugin = mimetype->name();
    m_plugins[mimetype->name()]->load(url);
    m_recentFilesAction->addUrl(url);
}

void MainWindow::configureAku()
{
    KConfigSkeleton *configSkeleton = new KConfigSkeleton(KGlobal::config(), this);
    KConfigDialog *configDialog = new KConfigDialog(this, "Plugins Information", configSkeleton);
    configDialog->setAttribute(Qt::WA_DeleteOnClose);
    
    KPluginSelector *selector = new KPluginSelector(configDialog);
    KService::List offers = KServiceTypeTrader::self()->query("Aku/Plugin");
    kDebug() << offers;
    QList<KPluginInfo> plugins = KPluginInfo::fromServices(offers);
    selector->addPlugins(plugins);

    configDialog->addPage(selector, i18n("Aku plugins"), "preferences-plugin");
    configDialog->exec();
}

void MainWindow::showPluginsInfo()
{
}

void MainWindow::showArchiveContent(const QVector<QStringList> &archive)
{
    if (archive.isEmpty()) {
        return;
    }

    // here we set additional per-plugin headers
    AkuPlugin *sender = static_cast<AkuPlugin*>(this->sender());
    m_model->setAdditionalHeaders(sender->additionalHeaderStrings());
    //
    m_model->setSourceData(archive);
}


