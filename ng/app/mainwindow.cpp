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

#include "addfiledialog.h"
#include "akustatusbar.h"
#include "akutreemodel.h"
#include "akutreenode.h"
#include "akutreeview.h"
#include "commentwidget.h"
#include "errorwidget.h"
#include "extractiondialog.h"
#include "filterwidget.h"
//#include "infodialog.h"
#include "metawidget.h"
#include "overwritewidget.h"
#include "passwidget.h"
#include "pluginloader.h"
#include "pluginsmodel.h"
#include "sortfiltermodel.h"

#include <QDockWidget>
#include <QListView>
#include <QHBoxLayout>

#include <KActionCollection>
#include <KActionMenu>
#include <KApplication>
#include <KConfigDialog>
#include <KConfigSkeleton>
#include <KFileDialog>
#include <KFileItem>
#include <KLocale>
#include <KMenuBar>
#include <KPluginInfo>
#include <KPluginSelector>
#include <KRecentFilesAction>
#include <KServiceTypeTrader>
#include <KStandardAction>
#include <KVBox>

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

    m_passwordWidget = new PassWidget(baseWidget);

    // Overwrite Widget
    m_overwriteWidget = new OverwriteWidget(baseWidget);
    //

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

    // Status Bar
    m_statusBar = new AkuStatusBar(this);
    setStatusBar(m_statusBar);
    //

    // connection for pluginloader
    PluginLoader *pluginLoader = new PluginLoader(this);
    connect(pluginLoader, SIGNAL(pluginsLoaded(AkuPlugin*, const KPluginInfo &)),
          this, SLOT(addPlugins(AkuPlugin*, const KPluginInfo &)));
    pluginLoader->loadPlugins();
    //

    // Comment Widget
    m_commentWidget = new CommentWidget(baseWidget);
    ///

    // Error Widget
    m_errorWidget = new ErrorWidget(baseWidget);
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
    m_actionMain = new QAction(i18n("Main Aku"), grp);
    m_actionMain->setCheckable(true);
    m_actionError = new QAction(i18n("Errors Console"), grp);
    m_actionError->setCheckable(true);
    m_actionComment = new QAction(i18n("Comment"), grp);
    m_actionComment->setCheckable(true);
    mainButton->setDefaultAction(m_actionMain);
    errorButton->setDefaultAction(m_actionError);
    commentButton->setDefaultAction(m_actionComment);

    m_actionMain->setChecked(true);
    connect(grp, SIGNAL(triggered(QAction *)), this, SLOT(tabChanged(QAction *)));
    //

    loadSettings();
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

    // Lock
    m_actionLock = new KAction(this);
    m_actionLock->setText(i18n("Lock Archive"));
    m_actionLock->setIcon(KIcon("document-encrypt"));
    actionCollection()->addAction("lock", m_actionLock);

    // Extraction
    m_actionExtract = new KActionMenu(this);
    m_actionExtract->setIcon(KIcon("archive-extract"));
    m_actionExtract->setText(i18n("Extract"));
    actionCollection()->addAction("extract", m_actionExtract);
    connect(m_actionExtract, SIGNAL(triggered()), this, SLOT(extractDialog()));

    // Add
    m_actionAdd = new KActionMenu(this);
    m_actionAdd->setIcon(KIcon("archive-insert"));
    m_actionAdd->setText(i18n("Add to archive"));
    m_actionAdd->setDelayed(false);
    KAction *actionAddFile = new KAction(i18n("Add file(s)"), this);
    connect(actionAddFile, SIGNAL(triggered()), this, SLOT(addFile()));
    actionAddFile->setIcon(KIcon("archive-insert"));
    KAction *actionAddDir = new KAction(i18n("Add dir"), this);
    actionAddDir->setIcon(KIcon("archive-insert-directory"));
    m_actionAdd->addAction(actionAddFile);
    m_actionAdd->addAction(actionAddDir);
    actionCollection()->addAction("add", m_actionAdd);
    actionCollection()->addAction("addDir", actionAddDir);
    actionCollection()->addAction("addFile", actionAddFile);
    //

    // Preview
    KAction *actionPreview = new KAction(this);
    actionPreview->setIcon(KIcon("document-preview-archive"));
    actionPreview->setText(i18n("Preview"));
    actionCollection() -> addAction("preview", actionPreview);
    connect(actionPreview, SIGNAL(triggered()), this, SLOT(preview()));

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
    connect(m_passwordWidget, SIGNAL(password(const QString &)), this, SLOT(getPassword(const QString &)));
    connect(m_actionLock, SIGNAL(triggered()), this, SLOT(lockArchive()));
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
    Q_UNUSED(info)

    connect(plugin, SIGNAL(archiveLoaded(const AkuData &)),
            this, SLOT(showArchiveContent(const AkuData &)));
    connect(plugin, SIGNAL(operationCompleted()), m_statusBar, SLOT(operationCompleted()));
    //connect(plugin, SIGNAL(notifyExtractionComplete()), this, SLOT(extractionCompleteSlot()));
    connect(plugin, SIGNAL(error(AkuPlugin::ErrorType, const QString &)), this,
            SLOT(handleError(AkuPlugin::ErrorType, const QString &)));
    connect(plugin, SIGNAL(stateChanged()), this, SLOT(pluginStateChanged()));

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
        kDebug() << url.pathOrUrl();
        // hack to show the filename in the error widget
        KUrl tmpUrl = m_currentUrl;
        m_currentUrl = url;
        handleError(AkuPlugin::NormalError, i18n("Could not open this file. Check the file permissions."));
        m_currentUrl = tmpUrl;
        return;
    }

    KMimeType::Ptr mimetype = KMimeType::findByFileContent(url.pathOrUrl());

    if (!m_plugins.contains(mimetype->name())) {
        // hack to show the filename in the error widget
        KUrl tmpUrl = m_currentUrl;
        m_currentUrl = url;
        handleError(AkuPlugin::NormalError, i18n("Sorry, no available plugin to open this file. The selected file type is") +
                    " "  + mimetype->name());
        m_currentUrl = tmpUrl;
        return;
    }

    m_currentPlugin = mimetype->name();
    m_plugins[m_currentPlugin]->load(url);
    m_recentFilesAction->addUrl(QFileInfo(url.pathOrUrl()).absoluteFilePath());
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
        m_passwordWidget->setPasswordType(m_currentUrl.prettyUrl(), PassWidget::Header);
        m_passwordWidget->askPassword();
    } else {
        m_passwordWidget->clearPassword();
    }

    if (!akudata.comment.isEmpty()) {
        m_commentWidget->setComment(akudata.comment);
    } else {
        m_commentWidget->clearComment();
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
    //tabChanged(m_actionMain);
    m_actionMain->trigger();
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
    if (action == m_actionComment) {
        m_treeView->hide();
        m_errorWidget->hide();
        m_commentWidget->show();

        m_filterWidget->setEnabled(false);
    }
    else if (action == m_actionError) {
        m_commentWidget->hide();
        m_treeView->hide();
        m_errorWidget->show();

        m_filterWidget->setEnabled(false);
    }
    else if (action == m_actionMain) {
        m_commentWidget->hide();
        m_errorWidget->hide();
        m_treeView->show();

        m_filterWidget->setEnabled(true);
    }
}

void MainWindow::extractDialog()
{
    ExtractionDialog *extractionDialog = new ExtractionDialog(this);
    connect(extractionDialog, SIGNAL(extractionClicked(const KUrl &, AkuPlugin::ExtractionOptions)),
            this, SLOT(extract(const KUrl &, AkuPlugin::ExtractionOptions)));
    extractionDialog->setAdvancedWidget(m_plugins[m_currentPlugin]->extractionWidget());
    extractionDialog->exec();
}

void MainWindow::loadSettings()
{
    QAction *actionHome = new QAction(QDir::homePath(), this);
    QAction *actionDesktop = new QAction(KGlobalSettings::desktopPath(), this);

    actionHome->setIcon(KIcon("user-home"));
    actionDesktop->setIcon(KIcon("user-desktop"));

    actionHome->setData(QDir::homePath());
    actionDesktop->setData(KGlobalSettings::desktopPath());

    KAction *actionLabel = new KAction(this);
    actionLabel->setText(i18n("Quick extracto to"));
    actionLabel->setIcon(KIcon("archive-extract"));
    actionLabel->setEnabled(false);

    m_actionExtract->addAction(actionLabel);
    m_actionExtract->addSeparator();
    m_actionExtract->addAction(actionHome);
    m_actionExtract->addAction(actionDesktop);

    connect(actionHome, SIGNAL(triggered()), this, SLOT(recentDirData()));
    connect(actionDesktop, SIGNAL(triggered()), this, SLOT(recentDirData()));

    QStringList pathsList;
    pathsList = KConfigGroup(KGlobal::config(), "Favourite Dirs").readEntry("destinationDirs", QStringList());
    foreach (const KUrl &path, pathsList) {
        KAction *recentDir = new KAction(this);
        recentDir->setData(QVariant(path.pathOrUrl()));
        //kDebug() << recentDir->data().toString();
        recentDir->setText(path.pathOrUrl());
        recentDir->setIcon(KIcon("inode-directory"));
        m_actionExtract->addAction(recentDir);
        connect(recentDir, SIGNAL(triggered()), this, SLOT(recentDirData()));
    }
}

void MainWindow::recentDirData()
{
    KAction *sender = static_cast<KAction*>(this->sender());
    KUrl url(sender->data().toString());
    kDebug() << url;
    kDebug() << sender->data().toString();

    AkuPlugin::ExtractionOptions eOptions;
    // WARNING: define standard extracting options for recentDirData (FIXME!)

    extract(url, eOptions);
}

void MainWindow::extract(const KUrl &destination, AkuPlugin::ExtractionOptions extractionOptions)
{
    // TODO: retrieve selected files to extract
    QStringList files = m_treeView->selectedPaths();
    kDebug() << files;

    AkuExtractInfo extractInfo;
    extractInfo.fileName = m_currentUrl;
    extractInfo.destination = destination;
    extractInfo.files = files;

    m_plugins[m_currentPlugin]->extract(extractInfo, extractionOptions);

    // update the favourite extraction dirs
    KConfigGroup options(KGlobal::config()->group("Favourite Dirs"));
    QStringList urlList = options.readEntry("destinationDirs", QStringList());
    if (urlList.contains(destination.pathOrUrl()) ||
        destination == KUrl(KGlobalSettings::desktopPath()) ||
        destination == KUrl(QDir::homePath())) {
        kDebug() << destination.pathOrUrl();
        return;
    }
    urlList.prepend(destination.path());
    options.writeEntry("destinationDirs", urlList);
    //kDebug() << urlList;
    //
}

void MainWindow::handleError(AkuPlugin::ErrorType errorType, const QString &error)
{
    AkuPlugin *sender = static_cast<AkuPlugin *>(this->sender());

    switch (errorType) {
        case AkuPlugin::NormalError :
            m_errorWidget->sendData(error, m_currentUrl.pathOrUrl(), sender->currentOperation());
            m_actionError->trigger();
            break;
        case AkuPlugin::RequestOverwrite :
            m_overwriteWidget->setInfo(error);
            kDebug() << "REQUEST OVERWRITE";
            m_overwriteWidget->show();
            break;
        default: ;
    }

}

void MainWindow::pluginStateChanged()
{
    AkuPlugin *sender = static_cast<AkuPlugin *>(this->sender());
    m_statusBar->stateChanged(sender);
}

// get the password from the PassWidget
void MainWindow::getPassword(const QString &password)
{
    m_password = password;
    kDebug() << password;
    switch (m_passwordWidget->currentPasswordType()) {
        case PassWidget::Files :
            break;
        case PassWidget::Header :
            m_plugins[m_currentPlugin]->load(m_currentUrl, m_password);
            m_passwordWidget->hide();
            break;
        default: ;
    }
}

void MainWindow::lockArchive()
{
    m_plugins[m_currentPlugin]->lock(m_currentUrl);
}

void MainWindow::addFile()
{
    AddFileDialog *addFileDialog = new AddFileDialog(this);
    addFileDialog->show();
}

void MainWindow::preview()
{
}
