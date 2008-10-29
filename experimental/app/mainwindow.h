#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <KXmlGuiWindow>
#include <QSplitter>

#include "maintree.h"
#include "openarchive.h"

class KDialog;
class AkuPlugin;
class PluginView;
class QTreeView;
class QListView;

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
    KDialog *m_optionDialog;
    PluginView *m_pluginView;
    QMap<QString, AkuPlugin*> m_plugins;

    void setupActions();
    void setupOptionsWidget();

  private slots:
    void openDialog();
    void addPlugin(AkuPlugin*);
    void showArchive(const QVector<QStringList> &archive);
    
};

#endif