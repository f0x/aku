#include "mainwindow.h"
#include "pluginloader.h"
#include "pluginview.h"
#include "akutreemodel.h"
#include "akutreeview.h"
#include "openarchive.h"
#include "akuiconview.h"
#include "akuviewoptionwidget.h"
#include "akuiconwidget.h"

#include <akuplugin.h>

#include <KStandardAction>
#include <KActionCollection>
#include <KApplication>
#include <KLocale>
#include <KAction>
#include <KFileDialog>
#include <KPageWidget>
#include <KPageWidgetItem>
#include <KDialog>
#include <KMimeType>
#include <KDebug>
#include <KIcon>
#include <KMessageBox>
#include <KConfigGroup>

#include <QListView>
#include <QTreeView>

MainWindow::MainWindow (QWidget* parent): KXmlGuiWindow (parent),
                                          m_pluginView(new PluginView())
{
  m_model = new AkuTreeModel(QVector<QStringList>());
  PluginLoader *loader = new PluginLoader(this);
  connect(loader, SIGNAL(pluginLoaded(AkuPlugin*)), this, SLOT(addPlugin(AkuPlugin*)));
  loader->loadAllPlugins();

  splitter = new QSplitter(this);
  setCentralWidget(splitter);

  treeView = new AkuTreeView(splitter);
  iconWidget = new AkuIconWidget(splitter);
  treeView->setModel(m_model);
  iconWidget->view()->setModel(m_model);

  openArchive = new OpenArchive(this);
  openArchive->setAvailablePlugins(m_plugins);
  setupOptionsWidget();
  setupActions();
  setupConnections();
  setupGUI(QSize(650,460)); 
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
  actionRecentFiles = KStandardAction::openRecent( openArchive, SLOT(load(const KUrl&)), actionCollection());
  actionRecentFiles->loadEntries(KConfigGroup(KGlobal::config()->group("RecentFiles")));
}

void MainWindow::setupConnections()
{
  connect(openArchive, SIGNAL(fileLoaded(KUrl)), this, SLOT(addRecentFile(KUrl)));
}

void MainWindow::openDialog()
{
  // TODO: base suffixes on available plugins
  KUrl url = KFileDialog::getOpenUrl(KUrl("kfiledialog:///AkuOpenDir"), 
                          i18n("*.rar *.7z *.zip *.bz2 *.gz *.tar|All supported types\n"
                               "*.rar|Rar archives\n*.7z|7-zip archives\n*.zip|Zip archives\n*.bz2|Tar archives (bzip)"
                               "\n*.gz|Tar archives (gzip)\n*.tar|Tar archives\n*.*|All files"), this);
  if (!url.isEmpty()) {
    openArchive -> load(url);
  }
}

void MainWindow::addRecentFile(KUrl recent)
{
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

    AkuViewOptionWidget *optionView = new AkuViewOptionWidget(m_optionDialog);
    optionView->setViews(treeView, iconWidget);

    connect(m_optionDialog, SIGNAL(applyClicked()), optionView, SLOT(applySettings()));
    connect(m_optionDialog, SIGNAL(okClicked()), optionView, SLOT(applySettings()));
    connect(m_optionDialog, SIGNAL(defaultClicked()), optionView, SLOT(restoreDefaults()));

    KPageWidgetItem *viewopt = new KPageWidgetItem( optionView, i18n( "Setup the View" ) );
    viewopt->setHeader( i18n( "Tree View Settings" ) );
    viewopt->setIcon( KIcon( "view-choose" ) );

    optionsWidget->addPage(plugins);
    optionsWidget->addPage(viewopt);
    m_optionDialog->setMainWidget(optionsWidget);
    m_optionDialog->setCaption(i18n("Configuration"));

    m_optionDialog->resize(600, 400);
}

void MainWindow::addPlugin(AkuPlugin *plugin)
{
    KMimeType::Ptr mime = KMimeType::mimeType(plugin->mimeTypeName());

    if (!mime) {
        kError()<<"Could not retrieve mimetype. Maybe wrong plugin implementation.";
        return;
    }

    connect(plugin, SIGNAL(archiveLoaded(const QVector<QStringList> &)),
            this, SLOT(showArchive(const QVector<QStringList> &)));
    connect(plugin, SIGNAL(error(const QString &)), this, SLOT(handleError(const QString &)));


    m_plugins.insert(mime->name(), plugin);

    m_pluginView->addPluginInfo(
                  mime->name(),
                  mime->comment(),
                  plugin->canExtract(),
                  plugin->canDelete(),
                  plugin->canCreate(),
                  plugin->canRename(),
                  plugin->isWorkingProperly()
                 );
}

void MainWindow::showArchive(const QVector<QStringList> &archive)
{
    if (archive.isEmpty()) {
        return;
    }

    // here we set additional per-plugin headers
    AkuPlugin *sender = static_cast<AkuPlugin*>(this->sender());
    m_model->setAdditionalHeaders(sender->additionalHeaderStrings());

    currentArchive = archive;

    m_model->setSourceData(currentArchive);
}

void MainWindow::handleError(const QString &error)
{
    AkuPlugin *sender = static_cast<AkuPlugin *>(this->sender());
    KMessageBox::error(this, error, sender->mimeTypeName() +" "+ i18n("plugin error"));
}
