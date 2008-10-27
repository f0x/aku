#include "maintree.h"

MainTree::MainTree(QWidget *parent) : QTreeView(parent)
{
  model = new QStandardItemModel();
  model -> setHorizontalHeaderItem(0, new QStandardItem( "Name" ));
  setModel(model);
}

MainTree::~MainTree()
{
}