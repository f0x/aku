#ifndef AKUADDDIR_H
#define AKUADDDIR_H

#include <KDialog>
#include <KFileTreeView>

class akuAddDir : public KDialog
{
 Q_OBJECT

  public:
    akuAddDir(QWidget *parent = 0);
    ~akuAddDir();

    KFileTreeView *treeView;

  public slots:
    virtual void updateTreeViewSelection(QString);

  private slots:
    void urlSelected(const KUrl&);
};

#endif