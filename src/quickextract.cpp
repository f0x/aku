#include "quickextract.h"

quickExtract::quickExtract(QString args, QString value, QWidget *parent) : KDialog(parent)
{  
  setModal(true);
  parentWidget = parent;
  archivename = args;
  function = value;
  
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

  treeView -> addAction (showhiddenAction);
  treeView -> setColumnHidden (3, true);
  treeView -> setColumnHidden (4, true);
  treeView -> setColumnHidden (5, true);
  treeView -> setColumnHidden (6, true);
  treeView -> header() -> setResizeMode ( 0, QHeaderView::ResizeToContents );
  treeView -> resizeColumnToContents (0);
  treeView -> setDirOnlyMode(true);
  treeView -> setEditTriggers(QAbstractItemView::NoEditTriggers);
  treeView -> setCurrentUrl(KUrl(QDir::homePath()));

  KUrlCompletion *comp = new KUrlCompletion(KUrlCompletion::DirCompletion);
  khistory = new KHistoryComboBox(v2layout);
  khistory -> setCompletionObject(comp);
  khistory -> setAutoDeleteCompletionObject(true);
  khistory -> setCompletionMode(KGlobalSettings::CompletionPopupAuto);

  QLabel *places = new QLabel(v1layout);
  places -> setText("<b>" + i18n("Places") + "</b>");
  KFilePlacesView *list = new KFilePlacesView(v1layout);
  KFilePlacesModel *model = new KFilePlacesModel(this);
  list -> setModel(model);
  list -> setAutoResizeItemsEnabled(true);
  opendestination = new QCheckBox(i18n("Open destination path"), v1layout);
  deletearchive = new QCheckBox(i18n("Delete archive after extracting"), v1layout);
  opendestination -> setVisible(false);
  deletearchive -> setVisible(false);

  if (function == "extractTo") {
    KConfig config;
    KConfigGroup history(&config, "Extraction dialog");
    QStringList list = history.readEntry("destination dirs", QStringList());
    khistory -> setHistoryItems(list);

    opendestination -> setVisible(true);
    //opendestination -> setChecked(true);
    deletearchive -> setVisible(true);
    deletearchive -> setEnabled(false);
    setCaption(i18n("Extract the archive to"));
    setInitialSize(QSize(540, 350));
    treeView -> setColumnHidden (1, true);
    treeView -> setColumnHidden (2, true);
    checkPassword();
  }

  if (function == "addDir") {
    setInitialSize(QSize(650, 400));
  }
    
  if (function == "extractHere") {
    checkPassword();
  }
  
  //list -> setRowHidden(0, true); // TODO: does not work so, try using the signal setupDone from model to set rowHidden
 
  //  connect(list, SIGNAL(clicked(const QModelIndex&)), SLOT(urlSelected(const QModelIndex&)));
  connect (showhiddenAction, SIGNAL (toggled(bool)), this, SLOT (hiddenFiles(bool)));
  connect (treeView, SIGNAL (currentChanged (KUrl)), this, SLOT (updateCombo(KUrl)));
  connect (khistory, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateTreeViewSelection(QString)));
  connect (khistory, SIGNAL(returnPressed(QString)), this, SLOT(updateTreeViewSelection(QString)));
  connect (list , SIGNAL(urlChanged(const KUrl&)), SLOT(urlSelected(const KUrl&)));
 

}

quickExtract::~quickExtract()
{
}

void quickExtract::checkPassword()
{
  QStringList options;
  options << "v";
  KUrl url(archivename);
  passwordprocess = new rarProcess(this, "rar", options, url.pathOrUrl());
  if (function == "extractHere") {
    connect(passwordprocess, SIGNAL(passwordCanceled()), kapp, SLOT(quit()));
    connect(passwordprocess, SIGNAL(processCompleted(bool)), this, SLOT(extractHere(bool)));
  }
  if (function == "extractTo") {
    connect(passwordprocess, SIGNAL(passwordCanceled()), this, SLOT(reject()));
  }
  passwordprocess -> start();
}

void quickExtract::openFolder()
{
  KRun::runUrl(KUrl::fromLocalFile(khistory -> currentText()), "inode/directory", 0); 
}

void quickExtract::password(QString pass)
{ 
   headerpass = pass;
   //kDebug() << headerpass;
   //extractHere();
}

void quickExtract::extractHere(bool status)
{ 
  if (status) {
    headerpass = passwordprocess -> getArchivePassword();
    QStringList options;
    options << "x";
    if (!headerpass.isEmpty()) options << "-p" + headerpass; 
    QDir herepath(archivename);
    KUrl url = herepath.absolutePath();
   
    rarProcess *process = new rarProcess(parentWidget, "rar", options, archivename, QStringList(), url.directory());
    connect(process, SIGNAL(processCompleted(bool)), kapp, SLOT(quit()));
    process -> start(headerpass);
  }
}

void quickExtract::extract() 
{
  headerpass = passwordprocess -> getArchivePassword();
  QStringList options;
  options << "x";
  if (!headerpass.isEmpty()) options << "-p" + headerpass;
  rarProcess *process = new rarProcess(parentWidget, "rar", options, archivename, QStringList(), khistory -> currentText());
  if (opendestination -> isChecked()) {
    connect(process, SIGNAL(processCompleted(bool)), this, SLOT(openFolder()));
  }
  connect(process, SIGNAL(processCompleted(bool)), this, SLOT(quitApplication(bool)));
 
  process -> start(headerpass);  
}

// funzione necessaria per evitare che alla comparsa di errori, aku non faccia in tempo a mostrarli
// a causa della chiamata kapp -> quit()
void quickExtract::quitApplication(bool noproblem)
{
  if (function == "extractTo") {
    if (noproblem) kapp -> quit();
    else {
      kapp -> setQuitOnLastWindowClosed(false);
    }
  }
}

void quickExtract::slotButtonClicked(int button) 
{
  if (button == KDialog::Ok) {
    if (function == "extractTo") {
      extract();
    }
    else if (function == "addDir") {
      emit destination(KUrl(khistory -> currentText())); 
      accept();
    }
  }
  else if (button == KDialog::Cancel) {
    if (function == "extractTo") {
      kapp -> quit();
    }
    else 
      reject();
  }
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
