#include "mainwindow.h"
#include "pluginloader.h"
#include "pluginview.h"
#include "akutreemodel.h"
#include "akutreeview.h"
#include "akuiconview.h"
#include "akuviewoptionwidget.h"
#include "akuiconwidget.h"
#include "akuextractiondialog.h"

#include <akuplugin.h>

#include <KStandardAction>
#include <KActionCollection>
#include <KApplication>
#include <KLocale>
#include <KFileDialog>
#include <KPageWidget>
#include <KPageWidgetItem>
#include <KDialog>
#include <KMimeType>
#include <KDebug>
#include <KIcon>
#include <KMessageBox>
#include <KConfigGroup>
#include <KStatusBar>
#include <KFileItem>

#include <QListView>
#include <QTreeView>
#include <QProgressBar>

MainWindow::MainWindow (QWidget* parent): KXmlGuiWindow (parent),
                                          m_pluginView(new PluginView()),
                                          m_progressBar(new QProgressBar(this))
{
  m_model = new AkuTreeModel(QVector<QStringList>(), this);
  PluginLoader *loader = new PluginLoader(this);

  connect(loader, SIGNAL(pluginLoaded(AkuPlugin*, const KPluginInfo &)),
          this, SLOT(addPlugin(AkuPlugin*, const KPluginInfo &)));

  loader->loadAllPlugins();

  splitter = new QSplitter(this);
  setCentralWidget(splitter);

  treeView = new AkuTreeView(splitter);
  iconWidget = new AkuIconWidget(splitter);
  treeView->setModel(m_model);
  iconWidget->view()->setModel(m_model);

  statusBar()->insertPermanentWidget(0, m_progressBar);
  m_progressBar->setMinimum(0);
  m_progressBar->setMaximum(100);
  m_progressBar->hide();

  setupOptionsWidget();
  setupActions();
  setupConnections();
  setupGUI(QSize(650,460)); 
  loadSettings();
}

MainWindow::~MainWindow()
{
   actionRecentFiles->saveEntries(KGlobal::config()->group("RecentFiles"));
}

void MainWindow::setupActions()
{
  KStandardAction::openNew(this, SLOT(createNewArchive()), actionCollection());

  KStandardAction::open(this, SLOT(openDialog()), actionCollection());

  KStandardAction::quit(kapp, SLOT(quit()), actionCollection());

  KStandardAction::preferences(m_optionDialog, SLOT(exec()), actionCollection());

  // Open Recent Files
  actionRecentFiles = KStandardAction::openRecent( this, SLOT(load(const KUrl&)), actionCollection());
  actionRecentFiles->loadEntries(KConfigGroup(KGlobal::config()->group("RecentFiles")));

  actionViewChoose = new KActionMenu(this);
  actionViewChoose->setIcon(KIcon("view-choose"));
  actionViewChoose->setText(i18n("View"));
  actionCollection()->addAction("view", actionViewChoose);
  actionViewChoose->setDelayed(false);
  viewTree = new QAction(i18n("Tree View"), this);
  viewTree->setIcon(KIcon("view-list-tree"));
  viewIcon = new QAction(i18n("Icon View"), this);
  viewIcon->setIcon(KIcon("view-list-icons"));
  actionViewChoose->addAction(viewTree);
  actionViewChoose->addAction(viewIcon);

  actionExtract = new KActionMenu(this);
  actionExtract->setIcon(KIcon("archive-extract.png"));
  actionExtract->setText(i18n("Extract"));
  actionCollection()->addAction("extract", actionExtract);
  connect(actionExtract, SIGNAL(triggered()), this, SLOT(extractionSlot()));

  actionAdd = new KActionMenu(this);
  actionAdd->setIcon(KIcon("archive-insert"));
  actionAdd->setText(i18n("Add to archive"));
  actionCollection()->addAction("add", actionAdd);
  actionAdd->setDelayed(false);
  addFile = new QAction(i18n("Add file(s)"), this);
  addFile->setIcon(KIcon("archive-insert"));
  addDir = new QAction(i18n("Add dir"), this);
  addDir->setIcon(KIcon("archive-insert-directory"));
  actionAdd->addAction(addFile);
  actionAdd->addAction(addDir);

  actionDelete = new KAction(this);
  actionDelete->setIcon(KIcon("archive-remove.png"));
  actionDelete->setText(i18n("Remove"));
  actionCollection()->addAction("delete", actionDelete);

  actionExtract->setEnabled(false);
  actionDelete->setEnabled(false);
  actionAdd->setEnabled(false);
}

void MainWindow::setupConnections()
{
  connect(viewTree, SIGNAL(triggered()), this, SLOT(changeView()));
  connect(viewIcon, SIGNAL(triggered()), this, SLOT(changeView()));
}

void MainWindow::loadSettings()
{  
   actionHome = new QAction(QDir().homePath(), this);
   actionDesktop = new QAction(KGlobalSettings::desktopPath(), this);
    
   actionHome->setIcon(KIcon("user-home"));
   actionDesktop->setIcon(KIcon("user-desktop"));
   
   actionHome->setData(QVariant(QDir().homePath()));
   actionDesktop->setData(QVariant(KGlobalSettings::desktopPath()));   
   actionExtract->addAction(actionHome);
   actionExtract->addAction(actionDesktop);

}

void MainWindow::openDialog()
{
  // TODO: base suffixes on available plugins
  KUrl url = KFileDialog::getOpenUrl(KUrl("kfiledialog:///AkuOpenDir"), 
                          i18n("*.rar *.7z *.zip *.bz2 *.gz *.tar *.ace|All supported types\n"
                               "*.rar|Rar archives\n*.7z|7-zip archives\n*.zip|Zip archives\n*.bz2|Tar archives (bzip)"
                               "\n*.gz|Tar archives (gzip)\n*.tar|Tar archives\n*.ace|Ace archives\n*.*|All files"), this);
  if (!url.isEmpty()) {
    m_currentUrl = url;
    load(url);
  }
}

void MainWindow::load(const KUrl &url)
{
    if (!KFileItem(KFileItem::Unknown, KFileItem::Unknown, url).isReadable()) {
        // TODO: show an error
        return;
    }

    KMimeType::Ptr mimetype = KMimeType::findByUrl(url);
    kDebug() << mimetype -> name();

    if (m_plugins.contains(mimetype->name())) {
        if (m_plugins[mimetype->name()]->isWorkingProperly()) {
            // this emit adds the url of the file loaded to recent files
            m_currentPlugin = mimetype->name();
            m_plugins[mimetype->name()]->load(url);
            addRecentFile(url);
        } else {
            KMessageBox::sorry(this,
                           i18n("The correct plugin to open <b>%1</b> mimetype was found but appears to not be working properly. "
                                "Please check the installation", mimetype->name()), 
                           i18n("Unable to load archive"));
        }

    } else {
        KMessageBox::detailedSorry(this,
                           i18n("Sorry, no available plugin to open: <b>%1</b>.", url.pathOrUrl() ),
                           i18n("Install a plugin for <b>%1</b> mimetype in order to load the archive."
                           , mimetype->name()), 
                           i18n("Unable to load archive"));
    }
}

void MainWindow::addRecentFile(KUrl recent)
{
    if (actionRecentFiles->maxItems() >= actionRecentFiles->urls().count()) {
        actionRecentFiles->removeUrl(actionRecentFiles->urls().first());
    }
    actionRecentFiles -> addUrl(recent);
}

void MainWindow::setupOptionsWidget()
{
    m_optionDialog = new KDialog(this);
    m_optionDialog->setButtons(KDialog::Ok | KDialog::Apply | KDialog::Cancel | KDialog::Default);

    KPageWidget *optionsWidget = new KPageWidget(m_optionDialog);

    KPageWidgetItem *plugins = new KPageWidgetItem( m_pluginView, i18n( "Plugins" ) );
    plugins->setHeader( i18n( "Aku Loaded Plugins" ) );
    plugins->setIcon( KIcon( "configure" ) );

    m_optionView = new AkuViewOptionWidget(m_optionDialog);
    m_optionView->setViews(treeView, iconWidget);

    connect(m_optionDialog, SIGNAL(applyClicked()), m_optionView, SLOT(applySettings()));
    connect(m_optionDialog, SIGNAL(okClicked()), m_optionView, SLOT(applySettings()));
    connect(m_optionDialog, SIGNAL(defaultClicked()), m_optionView, SLOT(restoreDefaults()));

    KPageWidgetItem *viewopt = new KPageWidgetItem(m_optionView, i18n( "Setup the View" ) );
    viewopt->setHeader( i18n( "Tree View Settings" ) );
    viewopt->setIcon( KIcon( "view-choose" ) );

    optionsWidget->addPage(plugins);
    optionsWidget->addPage(viewopt);
    m_optionDialog->setMainWidget(optionsWidget);
    m_optionDialog->setCaption(i18n("Configuration"));

    m_optionDialog->resize(600, 400);
}

void MainWindow::extractionSlot()
{
    m_extractionDialog = new AkuExtractionDialog(this);
    connect(m_extractionDialog, SIGNAL(extractionClicked(const KUrl &)), this, SLOT(doExtraction(const KUrl &)));
    m_extractionDialog->exec();
}

void MainWindow::doExtraction(const KUrl &destination)
{
    // TODO: retrieve selected files to extract
    QStringList files = treeView->selectedPaths();
    if (iconWidget->isVisible()) {
       files = iconWidget->view()->selectedPaths();
    }
    m_plugins[m_currentPlugin]->extract(m_currentUrl, destination, files);
    
}

void MainWindow::changeView()
{
    if (dynamic_cast<QAction*>(sender()) == viewTree) {
        m_optionView->showView(0);
    }

    if (dynamic_cast<QAction*>(sender()) == viewIcon) {
        m_optionView->showView(1);
    }

    m_optionView->applySettings();
}

void MainWindow::addPlugin(AkuPlugin *plugin, const KPluginInfo &info)
{
    connect(plugin, SIGNAL(archiveLoaded(const QVector<QStringList> &)),
            this, SLOT(showArchive(const QVector<QStringList> &)));
    connect(plugin, SIGNAL(error(const QString &)), this, SLOT(handleError(const QString &)));
    connect(plugin, SIGNAL(percent(double, double)), this, SLOT(handleProgress(double, double)));
    connect(plugin, SIGNAL(operationCompleted()), this, SLOT(completeOperations()));
    connect(plugin, SIGNAL(notifyExtractionComplete()), this, SLOT(extractionCompleteSlot()));

    foreach (const QString &mimeName, plugin->mimeTypeNames()) {
        KMimeType::Ptr mime = KMimeType::mimeType(mimeName);

        if (!mime) {
            kError()<<"Could not retrieve mimetype. Maybe wrong plugin implementation.";
            continue;
        }

        m_plugins.insert(mime->name(), plugin);

        m_pluginView->addPluginInfo(
                      mime->name(),
                      mime->comment(),
                      plugin->canExtract(),
                      plugin->canDelete(),
                      plugin->canCreate(),
                      plugin->canRename(),
                      plugin->isWorkingProperly(),
                      info,
                      plugin->configurationWidget()
                     );
    }
}

void MainWindow::showArchive(const QVector<QStringList> &archive)
{
    if (archive.isEmpty()) {
        return;
    }
   
    actionExtract->setEnabled(false);
    actionDelete->setEnabled(false);
    actionAdd->setEnabled(false);
   
    // here we set additional per-plugin headers
    AkuPlugin *sender = static_cast<AkuPlugin*>(this->sender());
    m_model->setAdditionalHeaders(sender->additionalHeaderStrings());
 
    if (sender->canExtract()) {
        actionExtract->setEnabled(true);
        actionExtract->setToolTip(i18n("Not supported by the current plugin"));
    }

    if (sender->canDelete()) {
        actionDelete->setEnabled(true);
        actionDelete->setToolTip(i18n("Not supported by the current plugin"));
    }  
  
    if (sender->canAdd()) {
        actionAdd->setEnabled(true);
        actionAdd->setToolTip(i18n("Not supported by the current plugin"));
    }    

    currentArchive = archive;

    m_model->setSourceData(currentArchive);
}

void MainWindow::handleError(const QString &error)
{
    AkuPlugin *sender = static_cast<AkuPlugin *>(this->sender());
    KMessageBox::error(this, error, i18n("Plugin error"));
}

void MainWindow::extractionCompleteSlot()
{
    KMessageBox::information(this, i18n("Extraction process complete"), i18n("Extraction"));
}

void MainWindow::handleProgress(double current, double total)
{
    if (!m_progressBar->isVisible()) {
        m_progressBar->show();
    }
    kDebug() << current << total;
    double percent = (double(100) * current) / total;
    m_progressBar->setValue((uint)percent);
    kDebug() << (uint)percent;
}

void MainWindow::completeOperations()
{
    m_progressBar->hide();
}
