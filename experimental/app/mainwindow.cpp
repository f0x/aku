#include "mainwindow.h"
#include "pluginloader.h"
#include "pluginview.h"
#include "akutreemodel.h"

#include <akuplugin.h>

#include <KStandardAction>
#include <KActionCollection>
#include <KLocale>
#include <KApplication>
#include <KAction>
#include <KUrl>
#include <KFileDialog>
#include <KPageWidget>
#include <KPageWidgetItem>
#include <KDialog>
#include <KMimeType>
#include <KDebug>

#include <QListView>
#include <QHeaderView>

MainWindow::MainWindow (QWidget* parent): KXmlGuiWindow (parent),
                                          m_pluginView(new PluginView())
{
  PluginLoader *loader = new PluginLoader(this);
  connect(loader, SIGNAL(pluginLoaded(AkuPlugin*)), this, SLOT(addPlugin(AkuPlugin*)));
  loader->loadAllPlugins();

  splitter = new QSplitter(this);
  setCentralWidget(splitter);
//   tree = new MainTree(splitter);


  AkuTreeModel *model = new AkuTreeModel(QVector<QStringList>());
  tree = new QTreeView(splitter);
  tree->setModel(model);

//   QVector<QStringList> testVector;
//   testVector << (QStringList()<<"file1"<<"size1"<<"packed1") << (QStringList()<<"file2"<<"size2"<<"packed2");
//   model->setSourceData(testVector);

  //splitter -> addWidget(tree);
  openArchive = new OpenArchive(this);
  openArchive->setAvailablePlugins(m_plugins);
  setupOptionsWidget();
  setupActions();
  setupGUI(QSize(650,460)); 
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupActions()
{
  KAction* newAction = new KAction(this);
  newAction = KStandardAction::openNew(this, SLOT(createNewArchive()), actionCollection());
  actionCollection() -> addAction("file_new", newAction);
  KAction* openAction = new KAction(this);
  openAction = KStandardAction::open(this, SLOT(openDialog()), actionCollection());
  actionCollection() -> addAction("file_open", openAction);
  KStandardAction::quit(kapp, SLOT(quit()), actionCollection());

  KAction *configAction = new KAction(this);
  configAction->setText(i18n("Configure Aku"));
  actionCollection()->addAction("config", configAction);
  connect(configAction, SIGNAL(triggered()), m_optionDialog, SLOT(exec()));
}

void MainWindow::openDialog()
{
  KUrl url = KFileDialog::getOpenUrl(KUrl("kfiledialog:///AkuOpenDir"), i18n("*.rar *.7z *.zip *.bz2 *.gz *.tar|All supported types\n*.rar|Rar archives\n*.7z|7-zip archives\n*.zip|Zip archives\n*.bz2|Tar archives (bzip)\n*.gz|Tar archives (gzip)\n*.tar|Tar archives\n*.*|All files"), this);
  if (!url.isEmpty()) {
    openArchive -> load(url);
  }
}

void MainWindow::setupOptionsWidget()
{
    m_optionDialog = new KDialog(this);
    KPageWidget *optionsWidget = new KPageWidget(m_optionDialog);

    KPageWidgetItem *plugins = new KPageWidgetItem( m_pluginView, i18n( "Plugins" ) );
    plugins->setHeader( i18n( "Aku Loaded Plugins" ) );
    plugins->setIcon( KIcon( "configure" ) );

    KPageWidgetItem *viewopt = new KPageWidgetItem( new QWidget(), i18n( "Setup the View" ) );
    viewopt->setHeader( i18n( "Tree View Settings" ) );
    viewopt->setIcon( KIcon( "view-choose" ) );

    optionsWidget->addPage(plugins);
    optionsWidget->addPage(viewopt);
    m_optionDialog->setMainWidget(optionsWidget);
    m_optionDialog->setCaption(i18n("Configuration"));
}

void MainWindow::addPlugin(AkuPlugin *plugin)
{
    kDebug()<<plugin->mimeTypeName();
    KMimeType::Ptr mime = KMimeType::mimeType(plugin->mimeTypeName());

    if (!mime) {
        kDebug()<<"Could not retrieve mimetype. Maybe wrong plugin implementation.";
        return;
    }

    connect(plugin, SIGNAL(archiveLoaded(const QVector<QStringList> &)),
            this, SLOT(showArchive(const QVector<QStringList> &)));

    m_plugins.insert(mime->name(), plugin);

    m_pluginView->addPluginInfo(
                  mime->name(),
                  mime->comment(),
                  plugin->canExtract(),
                  plugin->canDelete(),
                  plugin->canCreate(),
                  plugin->canRename()
                 );
}

void MainWindow::showArchive(const QVector<QStringList> &archive)
{
    static_cast<AkuTreeModel*>(tree->model())->setSourceData(archive);
}
