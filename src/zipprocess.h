#ifndef ZIPPROCESS_H
#define ZIPPROCESS_H

#include <QWidget>

#include <KFileItem>
#include <KDebug>

#include "threadprocess.h"
#include "akuerrordialog.h"

class zipProcess : public QObject
{
  Q_OBJECT

  public: 
    zipProcess(QWidget* parent = 0, QString = "", QStringList = QStringList(), QString = "", QStringList = QStringList(), QString = "");
    ~zipProcess();
   
    void showError(QByteArray);
   
    QString standardOutput();
 
  private:
    QWidget *parentWidget;
   
    QString archiver;
    QString archivename;
    QString destination;
    QString stdoutput;

    QStringList options;
    QStringList files;
    QStringList filesToHandle;

    bool noproblem;
    bool headercrypted;  
   
    QByteArray streamerror;
    QByteArray rawoutput;

    akuErrorDialog *errorDialog;

    threadProcess *thread;

  signals:
    void processCompleted(bool);
    void processCanceled();
    void outputReady(QString, bool);
 
  public slots:
    virtual void start();
    virtual void showProgress();
    virtual void getError();
 
   protected slots:
     virtual void initProcess();
     virtual void giveOutput(int, QProcess::ExitStatus);

};
#endif
