#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <KXmlGuiWindow>
#include <QSplitter>

class KDialog;
class AkuPlugin;
class PluginView;
class QTreeView;
class QListView;
class AkuTreeView;
class AkuIconView;
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
    AkuIconView *iconView;
    AkuTreeModel *m_model;
    OpenArchive *openArchive;
    KDialog *m_optionDialog;
    PluginView *m_pluginView;
    QMap<QString, AkuPlugin*> m_plugins;
    QVector<QStringList> currentArchive;
 

    void setupActions();
    void setupOptionsWidget();

  private slots:
    void openDialog();
    void addPlugin(AkuPlugin*);
    void showArchive(const QVector<QStringList> &archive);
    void changeView();
    void handleError(const QString &);
    
};

#endif