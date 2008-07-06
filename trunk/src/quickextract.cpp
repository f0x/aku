#include "quickextract.h"

quickExtract::quickExtract(QWidget *parent) : KDialog(parent)
{  
  setCaption(i18n("Extract the archive to"));
  setInitialSize(QSize(550, 350));
  KHBox *mainlayout = new KHBox(this);
  setMainWidget(mainlayout);
  
  mainlayout -> setSpacing(20);
  
  KVBox *v1layout = new KVBox(mainlayout);
  //KVBox *vspace = new KVBox(mainlayout);
  //mainlayout -> addSpacing(100);
  //QVBoxLayout *spacelayout = new QVBoxLayout(mainlayout);
  //spacelayout -> addSpacing(20);
  KVBox *v2layout = new KVBox(mainlayout);
  
  v1layout -> setSpacing(5);
  v2layout -> setSpacing(10);

  treeView = new KFileTreeView(v2layout); 
  treeView->setContextMenuPolicy(Qt::ActionsContextMenu);
   
  showhiddenAction = new KAction(treeView);
  showhiddenAction -> setText(i18n("Show hidden files"));
  showhiddenAction -> setCheckable(true);

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

  QLabel *space = new QLabel(v1layout);
  KDialogButtonBox *buttonBox = new KDialogButtonBox(v1layout, Qt::Vertical);
  KGuiItem guiHome(i18n("Home"), "user-home");
  buttonBox -> addButton(guiHome, QDialogButtonBox::ActionRole);
  KGuiItem guiDesktop(i18n("Desktop"), "user-desktop");
  buttonBox -> addButton(guiDesktop, QDialogButtonBox::ActionRole);
  KGuiItem guiRoot(i18n("Root"), "folder-red");
  KPushButton *buttonRoot = new KPushButton(guiRoot);
  buttonBox -> addButton(guiRoot, QDialogButtonBox::ActionRole);
  
  KGuiItem guiNewdir(i18n("New Directory"), "folder-new");
  KPushButton *buttonNewdir = new KPushButton(guiNewdir); 
  buttonBox -> addButton(guiNewdir, QDialogButtonBox::ActionRole);
  buttonBox -> setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  //buttonBox -> addButton(guiRoot, QDialogButtonBox::ActionRole);
  //QGroupBox *groupBox = new QGroupBox(v1layout);
  //QVBoxLayout *vbox = new QVBoxLayout(v1layout);
  QCheckBox *opendestination = new QCheckBox(i18n("Open destination path"), v1layout);
  QCheckBox *deletearchive = new QCheckBox(i18n("Delete archive after extracting"), v1layout);
  QLabel *spacebottom1 = new QLabel(v1layout);
  QLabel *spacebottom2 = new QLabel(v1layout);
  KUrlCompletion *comp = new KUrlCompletion(KUrlCompletion::DirCompletion);
  khistory = new KHistoryComboBox(v2layout);
  khistory -> setCompletionObject(comp);
  khistory -> setAutoDeleteCompletionObject(true);
  khistory -> setCompletionMode(KGlobalSettings::CompletionPopupAuto);

  connect (showhiddenAction, SIGNAL (toggled(bool)), this, SLOT (hiddenFiles(bool)));
  connect (treeView, SIGNAL (currentChanged (KUrl)), this, SLOT (updateCombo(KUrl)));
  connect (khistory, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateTreeViewSelection(QString)));
  connect (khistory, SIGNAL(returnPressed(QString)), this, SLOT(updateTreeViewSelection(QString)));
}

quickExtract::~quickExtract()
{
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
