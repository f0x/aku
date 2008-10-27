#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <KXmlGuiWindow>
#include <QSplitter>
#include <QTreeView>

#include "openarchive.h"

class MainWindow : public KXmlGuiWindow
{
  Q_OBJECT

  public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();

  private:
    QSplitter *splitter;
    QTreeView *tree;
    OpenArchive *openArchive;

    void setupActions();

  private slots:
    void openDialog();
    
};

#endif