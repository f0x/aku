#ifndef MAINTREE_H
#define MAINTREE_H

#include <QStandardItemModel>
#include <QTreeView>

class MainTree : public QTreeView
{
  Q_OBJECT

  public:
    MainTree(QWidget *parent);
    ~MainTree();
  
  private:
    QStandardItemModel *model;
};

#endif