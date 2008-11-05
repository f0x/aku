#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <KXmlGuiWindow>
#include <KRecentFilesAction>
#include <KUrl>
#include <QSplitter>

class KDialog;
class AkuPlugin;
class PluginView;
class QTreeView;
class QListView;
class AkuTreeView;
class AkuIconWidget;
class AkuTreeModel;
class OpenArchive;

class MainWindow : public KXmlGuiWindow
{
  Q_OBJECT

  public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();

  private:
    QSplitter *splitter;
    AkuTreeView *treeView;
    AkuIconWidget *iconWidget;
    AkuTreeModel *m_model;
    OpenArchive *openArchive;
    KDialog *m_optionDialog;
    PluginView *m_pluginView;
    QMap<QString, AkuPlugin*> m_plugins;
    QVector<QStringList> currentArchive;
    KRecentFilesAction *actionRecentFiles;
 

    void setupActions();
    void setupOptionsWidget();
    void setupConnections();

  private slots:
    void openDialog();
    void addPlugin(AkuPlugin*);
    void showArchive(const QVector<QStringList> &archive);
    void changeView();
    void handleError(const QString &);
    void addRecentFile(KUrl);
    
};

#endif