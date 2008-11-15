#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <KXmlGuiWindow>
#include <KRecentFilesAction>
#include <KUrl>
#include <KAction>
#include <KActionMenu>
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
class AkuViewOptionWidget;
class KPluginInfo;
class QProgressBar;
class AkuExtractionDialog;

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
    KActionMenu *actionViewChoose;
    KActionMenu *actionAdd;
    KActionMenu *actionExtract;
    QAction *viewTree;
    QAction *viewIcon;
    QAction *addFile;
    QAction *addDir;
    AkuViewOptionWidget *m_optionView;
    QProgressBar *m_progressBar;
    AkuExtractionDialog *m_extractionDialog;

    KAction *actionDelete;

    void setupActions();
    void setupOptionsWidget();
    void setupConnections();

  private slots:
    void openDialog();
    void addPlugin(AkuPlugin*, const KPluginInfo &);
    void showArchive(const QVector<QStringList> &archive);
    void handleError(const QString &);
    void addRecentFile(KUrl);
    void changeView();
    void handleProgress(double current, double total);
    void completeOperations();
    void extractionSlot();
    
};

#endif