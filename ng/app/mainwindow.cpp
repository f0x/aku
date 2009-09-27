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
#include "akutreeview.h"
#include "akutreemodel.h"
//#include "infodialog.h"
#include "pluginsmodel.h"
#include "filterwidget.h"
#include "sortfiltermodel.h"
#include "metawidget.h"
#include "akutreenode.h"
#include "passwidget.h"
#include "extractiondialog.h"

#include <QDockWidget>
#include <QListView>
#include <QHBoxLayout>

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
#include <KMenuBar>
#include <KFileItem>
#include <KDebug>

MainWindow::MainWindow (QWidget* parent): KXmlGuiWindow (parent)
{
    KVBox *baseWidget = new KVBox(this);
    setCentralWidget(baseWidget);

    m_model = new AkuTreeModel(QVector<QStringList>(), this);

    m_sortFilterModel = new SortFilterModel(this);
    m_sortFilterModel->setDynamicSortFilter(true);
    m_sortFilterModel->setSourceModel(m_model);

    m_filterWidget = new FilterWidget(baseWidget, m_sortFilterModel);

    m_treeView = new AkuTreeView(baseWidget);
    m_treeView->setModel(m_sortFilterModel);

    m_infoDock = new QDockWidget(i18nc("@title:window", "Information"));
    m_infoDock->setObjectName("infoDock");
    m_infoDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_metaWidget = new MetaWidget(m_infoDock);
    m_infoDock->setWidget(m_metaWidget);

    addDockWidget(Qt::RightDockWidgetArea, m_infoDock);

    setupActions();
    setupConnections();
    setupGUI(QSize(650,460));

    // connection for pluginloader
    PluginLoader *pluginLoader = new PluginLoader(this);
    connect(pluginLoader, SIGNAL(pluginsLoaded(AkuPlugin*, const KPluginInfo &)),
          this, SLOT(addPlugins(AkuPlugin*, const KPluginInfo &)));
    pluginLoader->loadPlugins();
    //

    // Bottom Widget
    QWidget *bottomWidget = new QWidget(baseWidget);
    QHBoxLayout *bottomLayout = new QHBoxLayout;
    QToolButton *mainButton = new QToolButton;
    mainButton->setAutoRaise(true);
    QToolButton *errorButton = new QToolButton;
    errorButton->setAutoRaise(true);
    QToolButton *commentButton = new QToolButton;
    commentButton->setAutoRaise(true);
    QSpacerItem *spacer = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
    bottomLayout->addWidget(mainButton);
    bottomLayout->addWidget(errorButton);
    bottomLayout->addWidget(commentButton);
    bottomLayout->addSpacerItem(spacer);
    bottomWidget->setLayout(bottomLayout);
    QActionGroup *grp = new QActionGroup(this);
    actionMain = new QAction(i18n("Main Aku"), grp);
    actionMain->setCheckable(true);
    actionError = new QAction(i18n("Errors Console"), grp);
    actionError->setCheckable(true);
    actionComment = new QAction(i18n("Comment"), grp);
    actionComment->setCheckable(true);
    mainButton->setDefaultAction(actionMain);
    errorButton->setDefaultAction(actionError);
    commentButton->setDefaultAction(actionComment);

    actionMain->setChecked(true);
    connect(grp, SIGNAL(triggered(QAction *)), this, SLOT(tabChanged(QAction *)));
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

    QAction* infoDockAction = m_infoDock->toggleViewAction();
    infoDockAction->setText(i18n("Information Panel"));
    infoDockAction->setShortcut(Qt::Key_F11);
    infoDockAction->setIcon(KIcon("dialog-information"));
    actionCollection()->addAction("infopanel", infoDockAction);

    actionCollection()->addAction("filter", m_filterWidget->action());

}

void MainWindow::setupConnections()
{
    connect(m_treeView, SIGNAL(activated(QModelIndex)), this, SLOT(dataMetaWidget(QModelIndex)));
    connect(m_treeView->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)),
            this, SLOT(selectionChanged(QModelIndex, QModelIndex)));
}

void MainWindow::openDialog()
{
    KUrl url = KFileDialog::getOpenUrl(QDir::homePath(), m_mimeTypeNames.join(" "), this, i18n("Open Archive"));

    if (!url.isEmpty()) {
        load(url);
    }
}

void MainWindow::addPlugins(AkuPlugin *plugin, const KPluginInfo &info)
{
    connect(plugin, SIGNAL(archiveLoaded(const AkuData &)),
            this, SLOT(showArchiveContent(const AkuData &)));

    foreach (const QString &mimeName, plugin->mimeTypeNames()) {
        KMimeType::Ptr mime = KMimeType::mimeType(mimeName);

        m_plugins.insert(mime->name(), plugin);
        // if the exe files are installed (ex: rar, unace, 7z)
        if (m_plugins[mime->name()]->isInstalled()) {
            m_mimeTypeNames << mimeName;
        }

        kDebug() << mimeName;
    }
}

void MainWindow::load(const KUrl url)
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
    m_plugins[m_currentPlugin]->load(url);
    m_recentFilesAction->addUrl(url);
    m_currentUrl = url;

    dataMetaWidget(QModelIndex());

    // spostare questa chiamata a load completato (quando l'interfaccia sarà in freeze)
    m_treeView->modelForHeader(m_model);
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

void MainWindow::showArchiveContent(const AkuData &akudata)
{
    // if this condition is true, we need to ask for the header
    // password and reload the archive
    if (akudata.headerprotected) {
        //PassWidget *passwordWidget = new PassWidget(baseWidget);
        //passwordWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        //passwordWidget->setTooltip("ciao");
        //passwordWidget->show();
    }

    if (!akudata.comment.isEmpty()) {
        KMessageBox::about(this, akudata.comment, "comment");
    }

    if (akudata.paths.isEmpty()) {
        return;
    }

    m_filterWidget->clearFilterText();

    // here we set additional per-plugin headers
    AkuPlugin *sender = static_cast<AkuPlugin*>(this->sender());
    m_model->setAdditionalHeaders(sender->additionalHeaderStrings());
    //
    m_model->setSourceData(akudata.paths);
    m_treeView->setSortingEnabled(true);
    m_treeView->expandAll();
}

void MainWindow::dataMetaWidget(QModelIndex index)
{
    if (!m_infoDock->isVisible()) {
        return;
    }

    if (!index.isValid()) {
        KFileItem item(KFileItem::Unknown, KFileItem::Unknown, m_currentUrl);
        //m_metaWidget->setMimeIcon(item.iconName());
        return;
    }

    AkuTreeNode *node = index.data(AkuTreeModel::NodeRole).value<AkuTreeNode*>();
    m_metaWidget->sendData(node);
}

void MainWindow::selectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);

    dataMetaWidget(current);
}

void MainWindow::tabChanged(QAction *action)
{
    if (action == actionComment) {
    }
    else if (action == actionError) {
    }
    else if (action == actionMain) {
    }
}
