#include "quickextract.h"

quickExtract::quickExtract(QString args, QWidget *parent) : KDialog(parent)
{  
  archivename = args;
  setCaption(i18n("Extract the archive to"));
  setInitialSize(QSize(540, 350));
  QSplitter *mainlayout = new QSplitter(this);
  setMainWidget(mainlayout);
  
  KVBox *v1layout = new KVBox(mainlayout);
  KVBox *v2layout = new KVBox(mainlayout);
  
  v1layout -> setSpacing(10);
  v2layout -> setSpacing(10);

  treeView = new KFileTreeView(v2layout); 
  treeView->setContextMenuPolicy(Qt::ActionsContextMenu);
   
  showhiddenAction = new KAction(treeView);
  showhiddenAction -> setText(i18n("Show hidden files"));
  showhiddenAction -> setCheckable(true);
  
  QLabel *places = new QLabel(v1layout);
  places -> setText("<b>" + i18n("Places") + "</b>");
  KFilePlacesView *list = new KFilePlacesView(v1layout);
  KFilePlacesModel *model = new KFilePlacesModel(this);
  list -> setModel(model);
  list -> setAutoResizeItemsEnabled(true);
  list -> setRowHidden(0, true); // TODO: does not work so, try using the signal setupDone from model to set rowHidden

  treeView -> addAction (showhiddenAction);
  treeView -> setColumnHidden (1, true);
  treeView -> setColumnHidden (2, true);
  treeView -> setColumnHidden (3, true);
  treeView -> setColumnHidden (4, true);
  treeView -> setColumnHidden (5, true);
  treeView -> setColumnHidden (6, true);
  treeView -> header() -> setResizeMode ( 0, QHeaderView::ResizeToContents );
  treeView -> resizeColumnToContents (0);
  treeView -> setDirOnlyMode(true);
  treeView -> setEditTriggers(QAbstractItemView::NoEditTriggers);
  treeView -> setCurrentUrl(KUrl(QDir::homePath()));

//   QLabel *space = new QLabel(v1layout);
//   KDialogButtonBox *buttonBox = new KDialogButtonBox(v1layout, Qt::Vertical);
//   KGuiItem guiHome(i18n("Home"), "user-home");
//   buttonBox -> addButton(guiHome, QDialogButtonBox::ActionRole);
//   KGuiItem guiDesktop(i18n("Desktop"), "user-desktop");
//   buttonBox -> addButton(guiDesktop, QDialogButtonBox::ActionRole);
//   KGuiItem guiRoot(i18n("Root"), "folder-red");
//   KPushButton *buttonRoot = new KPushButton(guiRoot);
//   buttonBox -> addButton(guiRoot, QDialogButtonBox::ActionRole);
  
//   KGuiItem guiNewdir(i18n("New Directory"), "folder-new");
//   KPushButton *buttonNewdir = new KPushButton(guiNewdir); 
//   buttonBox -> addButton(guiNewdir, QDialogButtonBox::ActionRole);
//   buttonBox -> setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  //buttonBox -> addButton(guiRoot, QDialogButtonBox::ActionRole);
  //QGroupBox *groupBox = new QGroupBox(v1layout);
  //QVBoxLayout *vbox = new QVBoxLayout(v1layout);
  QCheckBox *opendestination = new QCheckBox(i18n("Open destination path"), v1layout);
  QCheckBox *deletearchive = new QCheckBox(i18n("Delete archive after extracting"), v1layout);

  KUrlCompletion *comp = new KUrlCompletion(KUrlCompletion::DirCompletion);
  khistory = new KHistoryComboBox(v2layout);
  khistory -> setCompletionObject(comp);
  khistory -> setAutoDeleteCompletionObject(true);
  khistory -> setCompletionMode(KGlobalSettings::CompletionPopupAuto);

  connect (showhiddenAction, SIGNAL (toggled(bool)), this, SLOT (hiddenFiles(bool)));
  connect (treeView, SIGNAL (currentChanged (KUrl)), this, SLOT (updateCombo(KUrl)));
  connect (khistory, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateTreeViewSelection(QString)));
  connect (khistory, SIGNAL(returnPressed(QString)), this, SLOT(updateTreeViewSelection(QString)));
  
  connect (list , SIGNAL(urlChanged(const KUrl&)), SLOT(urlSelected(const KUrl&)));
  //  connect(list, SIGNAL(clicked(const QModelIndex&)), SLOT(urlSelected(const QModelIndex&)));

}

quickExtract::~quickExtract()
{
}

void quickExtract::extract() 
{
  rarProcess *process = new rarProcess(this, "rar", QStringList() << "x", archivename, QStringList(), khistory -> currentText());
  connect(process, SIGNAL(processCompleted(bool)), this, SIGNAL(processFinished(bool)));
  process -> start();
}

void quickExtract::slotButtonClicked(int button) 
{
  if (button == KDialog::Ok) {
    //return (khistory -> currentText());
    extract();
    //accept();
  }
  else if (button == KDialog::Cancel) reject();
}

void quickExtract::hiddenFiles(bool status)
{
  KUrl current = treeView -> currentUrl();
  treeView -> setShowHiddenFiles(status);
  treeView -> setCurrentUrl(current);
}

void quickExtract::updateCombo(KUrl url)
{
  khistory -> setCurrentItem(url.pathOrUrl(), true);
}

void quickExtract::updateTreeViewSelection(QString path)
{
  KUrl url(path);
  treeView -> setCurrentUrl(KUrl(path)); 
}

void quickExtract::urlSelected(const KUrl& url)
{
  QString string = url.pathOrUrl();
  updateTreeViewSelection(string);
}
