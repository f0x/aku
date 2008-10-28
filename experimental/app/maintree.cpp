#include "maintree.h"

#include <KLocale>

MainTree::MainTree(QWidget *parent) : QTreeView(parent)
{
  model = new QStandardItemModel();
  model -> setHorizontalHeaderItem(0, new QStandardItem(i18n("File Name")));
  model -> setHorizontalHeaderItem(1, new QStandardItem(i18n("Size")));
  model -> setHorizontalHeaderItem(2, new QStandardItem(i18n("Packed Size")));
  model -> setHorizontalHeaderItem(3, new QStandardItem(i18n("Ratio")));
  model -> setHorizontalHeaderItem(4, new QStandardItem(i18n("Modified")));
  model -> setHorizontalHeaderItem(5, new QStandardItem(i18n("Attributes")));
  model -> setHorizontalHeaderItem(6, new QStandardItem(i18n("Owner/Group")));
  model -> setHorizontalHeaderItem(7, new QStandardItem(i18n("CRC")));
  model -> setHorizontalHeaderItem(8, new QStandardItem(i18n("Method")));
  model -> setHorizontalHeaderItem(9, new QStandardItem(i18n("Version")));
  model -> setHorizontalHeaderItem(10, new QStandardItem(i18n("Mimetype")));
  setModel(model);
}

MainTree::~MainTree()
{
}