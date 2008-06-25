#include "mainwindow.h"

//#include <QIODevice>

// stringa contenente il tipo di compressione
// 1. rar
// 2. zip
// 3. tar
QString compressor;
// nome dell'archivio in uso
QString archive;

MainWindow::MainWindow (QWidget* parent, Qt::WFlags flags): KXmlGuiWindow (parent, flags)
{
  baseWindowWidget = new KVBox(this);
  splitter = new QSplitter(this);
  table = new akuMainTable(baseWindowWidget);
  splitter -> addWidget(baseWindowWidget);
  setCentralWidget(splitter);
  
  // a tip widget to show info
  tip = new akuTip(baseWindowWidget);
  tip->setVisible(false);
  tip->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

  //adding metaWidget to splitter
  metaWidget = new akuMetaWidget(this);
  splitter -> addWidget(metaWidget);
  metaWidget -> setVisible(false);

  setupSearchBar();
  setupStatusBar();
  setupActions();
  setupConnections();
  setupGUI (QSize(650,460));
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupStatusBar()
{
  archiveInfo = new QLabel(this); 
  archiveInfo ->  setFont(KGlobalSettings::generalFont());
  statusBar() -> addPermanentWidget(archiveInfo, 0);
  //infoLock = new QLabel(this);
  //infoLock -> setMaximumSize(22,18);
  ratioBar = new akuRatioWidget(0,this);
  ratioBar -> setMinimumSize(120,0);
  ratioBar -> setVisible(false);
  statusWidget = new QWidget(this);
  QGridLayout *statusLayout = new QGridLayout(statusWidget);
  statusLayout -> addWidget(archiveInfo,1,1);
  statusLayout -> addWidget(ratioBar,1,2);
  //statusLayout -> addWidget(infoLock,1,3);
  statusBar() -> addPermanentWidget(statusWidget);
  
}

void MainWindow::setupSearchBar()
{
  searchWidget = new akuSearchWidget(table, baseWindowWidget);
  searchWidget -> setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
 // searchWidget -> setMaximumSize(655555, 35);
 // baseWindowLayout->addWidget( searchWidget);
  searchWidget -> setVisible (false);	
  buttonFind = searchWidget -> getCloseAction();
}


void MainWindow::setupActions()
{
  buttonNew = KStandardAction::openNew(this, SLOT(createNewArchive()), actionCollection());
  actionCollection()->addAction("file_new",buttonNew );
  buttonOpen = KStandardAction::open(this, SLOT(openDialog()), actionCollection());
  actionCollection()->addAction("file_open", buttonOpen );
  KStandardAction::quit(kapp, SLOT(quit()), actionCollection());
  buttonInfo = new KAction(this);
  buttonInfo -> setIcon(KIcon("help-about"));
  buttonInfo -> setText(i18n("Information"));
  buttonInfo -> setCheckable(true);
  buttonInfo -> setEnabled(false);
  buttonInfo -> setShortcut(Qt::CTRL + Qt::Key_I);
  actionCollection() -> addAction("info", buttonInfo);
  recentFilesAction = KStandardAction::openRecent( this, SLOT(openUrl(const KUrl&)), actionCollection());
  recentFilesAction->setToolBarMode( KRecentFilesAction::MenuMode );
  recentFilesAction->setToolButtonPopupMode( QToolButton::DelayedPopup );
  recentFilesAction->loadEntries( KGlobal::config()->group("Recent Files"));
  KAction *lasttip = tip->actionTip();
  actionCollection() -> addAction("lasttip", lasttip);
  connect(lasttip, SIGNAL(triggered()),tip, SLOT(show()));
  buttonFind -> setShortcut(Qt::CTRL + Qt::Key_F);
  actionCollection() -> addAction("find", buttonFind);
  //buttonExtract = new KAction(this);
  //buttonExtract -> setIcon(KIcon("archive-extract.png")); 
  //buttonExtract -> setText(i18n("Extract"));
  //actionCollection() -> addAction("extract", buttonExtract);
  //buttonDelete = new KAction(this);
  //buttonDelete -> setIcon(KIcon("archive-remove.png"));
  //buttonDelete -> setText(i18n("Remove"));
  //actionCollection() -> addAction("delete", buttonDelete);
  buttonView = new KAction(this);
  buttonView -> setIcon(KIcon("document-preview-archive"));
  buttonView -> setText(i18n("Preview"));
  buttonView -> setEnabled(false);
  actionCollection() -> addAction("preview", buttonView);
  //recents = KStandardAction::openRecent(this, SLOT(openRecentFile(KUrl)), actionCollection());
  

  //KMenu *tool = new KMenu(i18n("Tools"), menuBar());
  //actionCollection() -> addAction("tools", tool->menuAction());
  //buttonLock = new KAction(this);
  //buttonLock -> setText(i18n("Lock Archive"));
  //buttonLock -> setIcon(KIcon("document-encrypt"));
  //actionCollection() -> addAction("lock", buttonLock);
  //KMenu *commands = new KMenu(i18n("Commands"), menuBar());
  //actionCollection() -> addAction("commands", commands->menuAction());
  //buttonAddComment = new KAction(this);
  //buttonAddComment -> setText(i18n("Add Archive Comment"));
  //buttonAddComment -> setIcon(KIcon("kontact-notes"));
  //actionCollection() -> addAction("add_comment", buttonAddComment);
  //buttonAddFolder = new KAction(this);
  //buttonAddFolder -> setText(i18n("Add Folder"));
  //buttonAddFolder -> setIcon(KIcon("archive-insert-directory"));
  //actionCollection() -> addAction("add_folder", buttonAddFolder);
  //buttonAddFile = new KAction(this);
  //buttonAddFile -> setText(i18n("Add File"));
  //buttonAddFile -> setIcon(KIcon("archive-insert"));
  //actionCollection() -> addAction("add_file", buttonAddFile);
  /*buttonEncryptArchive = new KAction(this);
  buttonEncryptArchive -> setText(i18n("Encrypt this archive"));
  buttonEncryptArchive -> setIcon(KIcon("dialog-password"));
  actionCollection() -> addAction("encrypt_archive", buttonEncryptArchive);
  buttonAddFilePwd = new KAction(this);
  buttonAddFilePwd -> setText(i18n("Add file with password"));
  buttonAddFilePwd -> setIcon(KIcon("archive-insert"));
  actionCollection() -> addAction("add_file_pwd", buttonAddFilePwd);
  buttonAddFolderPwd = new KAction(this);
  buttonAddFolderPwd -> setText(i18n("Add folder with password"));
  buttonAddFolderPwd -> setIcon(KIcon("archive-insert-directory"));
  actionCollection() -> addAction("add_folder_pwd", buttonAddFolderPwd);
  
  //connect(lasttip, SIGNAL(triggered()),tip, SLOT(show()));
  //setupGUI (QSize(650,460));
  //checkRarExe(); */
}

void MainWindow::setupConnections()
{
  connect (buttonInfo, SIGNAL (toggled (bool)), this, SLOT(viewInformation(bool)));
  connect (buttonView, SIGNAL (triggered()), this, SLOT (embeddedViewer()));
  connect (table, SIGNAL (itemSelectionChanged()), this, SLOT (metaBar()));
  connect (table, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(openItemUrl(QTreeWidgetItem *, int)));
}

void MainWindow::enableActions(bool enable)
{
  buttonInfo -> setEnabled(enable);
  buttonOpen -> setEnabled(enable);
  buttonNew -> setEnabled(enable);
  buttonView -> setEnabled (enable);
  if (enable) setCursor(QCursor());
  else setCursor(Qt::WaitCursor);
}

void MainWindow::openDialog()
{
  KUrl url = KFileDialog::getOpenUrl(KUrl(QDir().homePath()), i18n("*.rar *.zip *.bz2 *.gz|All supported types\n*.rar|Rar archives\n*.zip|Zip archives\n*.bz2|Tar archives (bzip)\n*.gz|Tar archives (gzip)\n*.*|All files"), this);
  if (!url.isEmpty())
    openUrl(url);
}

void MainWindow::openUrl(const KUrl& url)
{
  // determino l'estensione dell'archivio
  KMimeType::Ptr mimetype = KMimeType::findByUrl(url);
  //QString string = mimetype -> name();
  //puts (string.toAscii());

  if (mimetype -> name() == "application/x-rar") {
    compressor = "rar";
    enableActions(false);
    statusBar()->showMessage(i18n("Loading archive..."));
    rarprocess = new rarProcess(this, "rar", QStringList() << "v", url.pathOrUrl());
    connect(rarprocess, SIGNAL(outputReady(QString, bool)), this, SLOT(buildRarTable(QString, bool)));
    rarprocess -> start();
    metaWidget -> clear();
    metaWidget -> setFileName("application/x-rar");
    QPixmap mimeIcon = KIcon("application-x-rar").pixmap(128,128);
    metaWidget -> setMimeIcon( mimeIcon );
    metaWidget -> setMime("");
  }

  else if (mimetype -> name() == "application/zip") {
    compressor = "zip";
    enableActions(false);
    statusBar()->showMessage(i18n("Loading archive..."));
    zipprocess = new zipProcess(this, "unzip", QStringList() << "-Zl", url.pathOrUrl());
    connect(zipprocess, SIGNAL(outputReady(QString, bool)), this, SLOT(buildZipTable(QString, bool)));
    zipprocess -> start();
    metaWidget -> clear();
    metaWidget -> setFileName("application/zip");
    QPixmap mimeIcon = KIcon("application-zip").pixmap(128,128);
    metaWidget -> setMimeIcon( mimeIcon );
    metaWidget -> setMime("");
  }

  else if ((mimetype -> name() == "application/x-bzip-compressed-tar") || (mimetype -> name() == "application/x-compressed-tar")) { 
    compressor = "tar";
    enableActions(false);
    statusBar()->showMessage(i18n("Loading archive..."));
    tarprocess = new tarProcess(this, "tar", QStringList() << "-tvf", url.pathOrUrl());
    connect(tarprocess, SIGNAL(outputReady(QString, bool)), this, SLOT(buildTarTable(QString)));
    tarprocess -> start();
    metaWidget -> clear();
    metaWidget -> setFileName(mimetype -> name());
    QPixmap mimeIcon = KIcon("application-x-bzip").pixmap(128,128);
    metaWidget -> setMimeIcon( mimeIcon );
    metaWidget -> setMime("");     
  }

  else compressor.clear();
  // costruisco la tabella
  if (!compressor.isEmpty()) {
    archive = url.pathOrUrl();
    ratioBar -> setVisible(true);
    setCaption(archive);
  }
  else {
    tip->setTip(i18n("The file is not a supported archive") + " (" + (mimetype -> comment()) + ")");
    tip->show();
  }
}

void MainWindow::buildZipTable(QString zipoutput, bool crypted)
{
   disconnect(zipprocess, SIGNAL(outputReady(QString, bool)), this, SLOT(buildZipTable(QString, bool)));
   if (!zipoutput.isEmpty())
   { 
     table -> setFormat("zip");
     table -> clear(); //ripulisco la lista
     zip nzip;
     nzip.parse(table, zipoutput, ratioBar);
  
     QStringList archivedetails = nzip.getArchiveDetails();
     archiveInfo -> setText(archivedetails[0] + " " + "<b>" + i18n("file(s)") + "  " + i18n("size:") + "</b> " + archivedetails[1] + " <b> " + i18n("packed:") + "</b> " + archivedetails[2] + "  "); 

     table -> header() -> setResizeMode(0, QHeaderView::ResizeToContents);
     table -> header() -> setResizeMode(3, QHeaderView::ResizeToContents);
     table -> header() -> setResizeMode(4, QHeaderView::ResizeToContents);
     table -> header() -> setResizeMode(7, QHeaderView::ResizeToContents);
     table -> header() -> setResizeMode(8, QHeaderView::ResizeToContents);
     expandTopLevelItems();
     table -> sortItems ( 0, Qt::AscendingOrder );
     setFolderIcons();
   }
   enableActions(true);
   statusBar()->clearMessage();
}

void MainWindow::buildTarTable(QString taroutput)
{  
   disconnect(tarprocess, SIGNAL(outputReady(QString, bool)), this, SLOT(buildTarTable(QString)));
   if (!taroutput.isEmpty()) {
     table -> setFormat("tar");
     table -> clear();
     tar ntar;
     ntar.parse(table, taroutput);

     QStringList archivedetails = ntar.getArchiveDetails();
   
     KFileItem file(KFileItem::Unknown, KFileItem::Unknown, KUrl(archive));
     float ratio (100 - ((100.0 * file.size()) / archivedetails[1].toULongLong()));
   
     archiveInfo -> setText(archivedetails[0] + " " + "<b>" + i18n("file(s)") + "  " + i18n("size:") + "</b> " + KLocale(archivedetails[1]).formatByteSize(archivedetails[1].toULong()) + " <b> " + i18n("packed:") + "</b> " + KLocale(QString().setNum(file.size())).formatByteSize((QString().setNum(file.size())).toULong()) + "  ");

     ratioBar -> setRatio(int(ratio + 0.5f));    
 
     table -> header() -> setResizeMode(0, QHeaderView::ResizeToContents);
     table -> header() -> setResizeMode(4, QHeaderView::ResizeToContents);
     table -> header() -> setResizeMode(11, QHeaderView::ResizeToContents);
     expandTopLevelItems();
     table -> sortItems ( 0, Qt::AscendingOrder );
     setFolderIcons();
     
   }
   enableActions(true);
   statusBar()->clearMessage();
}

void MainWindow::buildRarTable(QString raroutput, bool crypted)
{
  disconnect(rarprocess, SIGNAL(outputReady(QString, bool)), this, SLOT(buildRarTable(QString, bool)));
  if (!raroutput.isEmpty())
  { 
    table -> setFormat("rar");
    table -> clear(); //ripulisco la lista
    rar nrar;
    nrar.parse (table, raroutput, ratioBar);  //metaWidget -> ratioBar() 
    
    QStringList archivedetails = nrar.getArchiveDetails();
    
    archiveInfo -> setText(archivedetails[0] + " " + "<b>" + i18n("file(s)") + "  " + i18n("size:") + "</b> " + archivedetails[1] + " <b> " + i18n("packed:") + "</b> " + archivedetails[2] + "  ");

//     //if(crypted){
//     //globalArchivePassword = newProcHandler -> getArchivePassword();
//     //actionAddFolderPwd->setEnabled(false);
//     //actionAddFilePwd->setEnabled(false);
//     //}
//     //else 
//     //globalArchivePassword.clear();
//     
     //setWindowTitle ( i18n( "aKu - " ) + KUrl(namex).pathOrUrl() );
//     setCaption(KUrl(namex).pathOrUrl());
     table -> header() -> setResizeMode(0, QHeaderView::ResizeToContents); // filename
     table -> header() -> setResizeMode(3, QHeaderView::ResizeToContents); // ratio
     table -> header() -> setResizeMode(4, QHeaderView::ResizeToContents); // modified
     table -> header() -> setResizeMode(7, QHeaderView::ResizeToContents); // method
     table -> header() -> setResizeMode(8, QHeaderView::ResizeToContents); // version
     table -> sortItems (0, Qt::AscendingOrder );
//     if(globalRestrictions) handleRestrictions(namex, rarout);
//     if ( fromNewArchive == true )   //ripristiniamo la gui se proveniamo da un new archive
//     {
//       closeNewArchiveGUI(false);
//       fromNewArchive = false;
//     }
//     showStatusInfo(true);
      expandTopLevelItems();
      setFolderIcons();
      statusWidget -> setVisible(true);
  }
  enableActions(true);
  statusBar()->clearMessage();
}

void MainWindow::setFolderIcons()
{
  for (int i = 0; i < table -> topLevelItemCount(); i++ )
    if ((table -> topLevelItem(i)) -> text ( 5 ).isEmpty())
    {
      (table -> topLevelItem ( i ) ) -> setIcon (0, KIcon( "inode-directory" ));
       recursiveFolderIcons (table -> topLevelItem (i));
    }
}

void MainWindow::recursiveFolderIcons (QTreeWidgetItem *checkParent)
{
  for ( int i = 0; i < checkParent -> childCount(); i++ )
    if ( ( checkParent -> child ( i ) ) -> text ( 5 ).isEmpty() )
    {
      (checkParent -> child ( i ) ) -> setIcon ( 0, KIcon ( "inode-directory" ));
      recursiveFolderIcons (checkParent -> child ( i ));
    } 
}

void MainWindow::viewInformation (bool visible)
{
  metaWidget -> setVisible(visible);
  if (visible) metaBar();
}

void MainWindow::metaBar()
{
  QList<QTreeWidgetItem*> checkSelected = table -> selectedItems();
  if (checkSelected.size() != 0) {
    if (checkSelected.size() == 1) {
      getMetaInfo(checkSelected[0]);
      if ((checkSelected[0] -> text(9).contains("image")) && metaWidget -> isVisible() ) {
        //if the file is an image we make a preview
        QString itemPath = rebuildFullPath(checkSelected[0]);
        QProcess imagepreview;
        if (compressor == "rar") imagepreview.start(compressor, QStringList() << "p" << "-inul" << archive << itemPath);
        if (compressor == "zip") imagepreview.start("unzip", QStringList() << "-p" << "-qq" << archive << itemPath);
        if (compressor == "tar") imagepreview.start(compressor, QStringList()<< "-xOf" << archive <<itemPath);
        imagepreview.waitForFinished();
        imagepreview.terminate();
        QByteArray preview = imagepreview.readAllStandardOutput();
        metaWidget -> setPreview(preview);
      }
    }
    else {
      int folders = 0;
      int files = 0;
      for (int i = 0; i < checkSelected.size(); i++) {
        if (checkSelected[i] -> text(1).isEmpty()) folders ++;
        else files++;
      }
      metaWidget -> setFileName (QString().setNum (folders) + " " +i18n("folder(s)") + "\n" + QString().setNum(files) + " " + i18n("file(s)"));
      metaWidget -> handleItemSelections(checkSelected); //testing multiple files selections
      sumSelectedItemsSize();
    }
  }
  //else metaWidget -> setFileName(i18n("No item(s) selected"));
}

void MainWindow::sumSelectedItemsSize()
{
  QStringList sizes;
  QList<QTreeWidgetItem*> selectedItems = table -> selectedItems();
  QList<double> rawSizeSum;
  for ( int i = 0; i < selectedItems.size(); i++ ) { 
     //acquisisco le dimensioni con le opportune conversioni
    if ( selectedItems[i] -> text ( 1 ) != "" ) {
      sizes = ( selectedItems[i] -> text ( 1 ) ).split ( " " );
      if ( sizes[1] == "KiB" ) rawSizeSum << ( sizes[0].toFloat() * 1024 );
      else if ( sizes[1] ==  "MiB" ) rawSizeSum << ( sizes[0].toFloat() * 1048576 );
      else if ( sizes[1] == "GiB" ) rawSizeSum << ( sizes[0].toFloat() * 1073741824 );
      else rawSizeSum << sizes[0].toFloat();
    }  
  }
  double result = 0;
  for ( int j=0; j < rawSizeSum.size(); j++ ) {
    result = result + rawSizeSum[j];
  }
  metaWidget -> setFileSize( KLocale("").formatByteSize(result));
}

void MainWindow::getMetaInfo (QTreeWidgetItem *item) //setta le informazioni della metabar
{
  if(!item -> text(1).isEmpty()) {
    metaWidget -> setFileName (item -> text (0));
    metaWidget -> setFileSize (item -> text (1));
    if (compressor == "rar") metaWidget-> setDateTime((item->data(4, Qt::UserRole)).toDateTime());
    else metaWidget-> setDateTxt((item -> text(4)));
    if (compressor != "tar") metaWidget -> setRatio(item->text(3).remove("%").toFloat());
  }
  else
    metaWidget -> setFileName(item -> text(0), true );
}

QString MainWindow::rebuildFullPath(QTreeWidgetItem *toRebuild)
{
  QString rebuilded = rebuildPath(toRebuild);
  if (!rebuilded.isEmpty()) return rebuilded + QDir().separator() + toRebuild -> text(0);
  else return toRebuild -> text(0);
}

QString MainWindow::rebuildPath (QTreeWidgetItem *toRebuild) //missing item name.. see rebuildFullPath for complete string
{
  QTreeWidgetItem *tmp; //elemento temporaneo
  QStringList pathlist; //lista delle cartelle di percorso
  tmp = toRebuild -> parent();
  while (tmp != NULL) {
    pathlist << tmp -> text (0);
    tmp = tmp -> parent();
  }
  QString path;
  for ( int i = pathlist.size()-1; i>=0; i-- )   {
    path.append (pathlist[i]);
    if ( i!=0 ) path.append (QDir().separator());
  }
  return path;
}

void MainWindow::expandTopLevelItems()
{
  for (int i = 0; i < table -> topLevelItemCount(); i++ )
    table -> setItemExpanded (table -> topLevelItem(i), true);
}

void MainWindow::embeddedViewer()
{
  if (table -> selectedItems().size() == 1) {
    QTreeWidgetItem *toView = table -> selectedItems()[0];
    if (!toView -> text ( 1 ).isEmpty())  { //&& toView -> icon(10).isNull()) {
      QString path = rebuildFullPath(toView);
      QProcess view;
      //if(globalArchivePassword.isEmpty())
      // view.start ( archiver, QStringList() << "p" << "-inul" << namex << path << QDir::tempPath() );
      //else
      if (compressor == "rar") view.start (compressor, QStringList() << "p" << "-inul" << archive << path); //<< QDir::tempPath());
      if (compressor == "zip") view.start("unzip", QStringList() << "-p" << "-qq" << archive << path); //<< QDir::tempPath());
      if (compressor == "tar") view.start(compressor, QStringList() << "-xOf" << archive << path);
      view.waitForFinished();
      view.terminate();

      QByteArray buffer = view.readAllStandardOutput(); //leggo il file temporaneo e memorizzo in buffer
      akuViewer *embedded = new akuViewer ( this ); //chiamiamo il viewer e passiamogli il testo
      embedded -> setAttribute(Qt::WA_DeleteOnClose);
      embedded -> setData(buffer);
      embedded -> setFileName ( toView -> text ( 0 ) );
      buffer.clear();
      embedded -> setGeometry ( this -> x() +200, this -> y() +200, 600,600 ); // centriamo e ingrandiamo a sufficienza
      embedded -> show();
    }
    //else if(!toView -> icon(10).isNull()) {
    //tip->setTip(i18n("Cannot preview a password-protected file"));
    //tip->show();
    //}
  }
  else {
    tip->setTip(i18n("Only one item at time can be viewed"));
    tip->show(); 
  }
}

void MainWindow::openItemUrl(QTreeWidgetItem *toOpen, int) //apriamo l'elemento con la relativa applicazione associata
{
  if(!toOpen -> text(1).isEmpty())   {
    QString tempPath = QDir().tempPath();
    if(!tempPath.endsWith(QDir().separator())) tempPath.append(QDir().separator()); //controlliamo che la stringa termini con un separator
    QString fileToExtract= rebuildFullPath(toOpen);
    QFile tempFile(tempPath + fileToExtract);
    rarProcess *rarprocess;
    if(!tempFile.exists()) {
      rarprocess = new rarProcess(this, "rar", QStringList() << "x", archive ,QStringList() << fileToExtract, tempPath); //estraiamo il file nella cartella temporanea
      rarprocess -> start();
    }
    QString forUrl;
    forUrl = tempPath + fileToExtract;
    KUrl url(tempPath + fileToExtract);
    QDesktopServices::openUrl(url);
  }
}
