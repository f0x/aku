#include "zipprocess.h"

zipProcess::zipProcess(QWidget *parent, QString ziparchiver, QStringList zipoptions, QString ziparchivename, QStringList zipfiles, QString zipdestination) : QObject(parent)
{
  parentWidget = parent;
  archiver = ziparchiver;
  options = zipoptions;
  archivename = ziparchivename;
  files = zipfiles;
  destination = zipdestination;
  noproblem = false;
  headercrypted = false;
  errorDialog = new akuErrorDialog();
}

zipProcess::~zipProcess()
{
  qDeleteAll(children());
}

void zipProcess::start()
{
  initProcess();
}

void zipProcess::initProcess()
{
  kDebug() << "Avvio del threadProcess (ZIP)...";
  thread = new threadProcess(this);
  // setUp firstTime connections
  connect(thread, SIGNAL(readyReadStandardError()), this, SLOT(getError()));
  connect(thread, SIGNAL(readyReadStandardOutput()), this, SLOT(showProgress()));
  connect(thread, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(giveOutput(int, QProcess::ExitStatus)));
  
  if (options.isEmpty()) {
  //     rarprogressdialog = new akuProgressDialog(parentWidget, files.size());
//     connect(rarprogressdialog, SIGNAL(canceled()), this, SLOT(handleCancel()));
//     connect(rarprogressdialog, SIGNAL(paused()), this, SLOT(handlePaused()));
//     connect(rarprogressdialog, SIGNAL(continued()), this, SLOT(handleContinued()));
//     rarprogressdialog -> setArchiveName(archivename);
    kDebug() << "estrazione zip";
   
    QStringList options;
    options << archivename;
    if (!files.isEmpty())
      options << files;
    options << "-d" << destination;
    thread -> start(archiver, options);
    kDebug() << options;
  }

  else if (options[0] == "-Zl") 
  { 
    thread -> start(archiver, QStringList() << options << archivename);
  }

  else if (options[0] == "-z") {
    thread -> start(archiver, QStringList() << options[0] << archivename );
    QString text = options[1];
    kDebug() << text;
    text.append("\n.\n");
    kDebug() << text;
    thread -> write(text.toUtf8());
    thread -> waitForFinished();
  }

  else if (options[0] == "-d") {
    thread -> start(archiver, QStringList() << options << archivename << files);
  }
 
  else if (options[0] == "-q") {
    thread -> start(archiver, QStringList() << options << archivename << files << "-d" << destination);
    thread -> waitForFinished();
  } 

  kDebug() << "initProcess (ZIP) terminato";
}

void zipProcess::showProgress() 
{
 
  QByteArray gotOutput = thread -> readAllStandardOutput();
  stdoutput.append(gotOutput);
  rawoutput.append(gotOutput);
  
}

void zipProcess::giveOutput(int, QProcess::ExitStatus)
{
  kDebug() << "process terminated (ZIP)";
  emit outputReady(stdoutput, headercrypted);
  if (streamerror.isEmpty()) {
    kDebug() << "no problem";
    noproblem = true;
  }
  else {
    noproblem = false;
    showError(streamerror);
  }
  emit processCompleted(noproblem); //check the bool 
}

void zipProcess::showError(QByteArray streamerror)
{
  if (!streamerror.isEmpty()) {
    QByteArray error = streamerror;
    QByteArray original = QString("Permission denied").toAscii();
    QByteArray translated = QString("<b>" + i18n("Permission denied") + "</b><br>").toAscii();
    error.replace(original, translated);
    original = QString("zip I/O error:").toAscii();
    translated = QString("<b><font color=red>" + i18n("zip I/O error:") + "</b></font>").toAscii();
    error.replace(original, translated);
    original = QString("caution:").toAscii();
    translated = QString("<b>" + i18n("caution:") + "</b>").toAscii();
    original = QString("zip error:").toAscii();
    translated = QString("<b><font color=red>" + i18n("zip error:") + "</font").toAscii();
    error.replace(original, translated);
    original = QString("Nothing to do!").toAscii();
    translated = QString("<b>" + i18n("Nothing to do!") + "</b>").toAscii();
    error.replace(original, translated);
    original = QString("Could not create output file").toAscii();
    translated = QString("<b>" + i18n("Could not create output file") + "</b><br>").toAscii();
    error.replace(original, translated);
    original = QString("zip warning:").toAscii();
    translated = QString("<b><font color=red>" + i18n("zip warning:") + "</b></font>").toAscii();
    error.replace(original, translated);
    errorDialog -> setError(error);
    errorDialog -> show();
  }
}

void zipProcess::getError()
{ 
  QByteArray temp = thread -> readAllStandardError();
  // invalid option(s) used with -d; ignored --> warning relativo alla cancellazione di directory  
  if (temp.contains("invalid option(s) used with -d; ignored")) { 
    temp.remove(0, (temp.indexOf("\n") + 1));
  } 
  // stringa relativa all'eliminazione di tutti i file dell'archivio
  if (temp.contains("zip warning: zip file empty")) {
    temp.remove(0, (temp.indexOf("\n") + 1));
  } 

  if (temp.contains("caution: filename not matched:")) {
    
  }
 
  streamerror.append(temp);
}


