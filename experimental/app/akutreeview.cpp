#include "akutreeview.h"
#include "akutreemodel.h"

#include <KLocale>

#include <QVector>

AkuTreeView::AkuTreeView(QWidget *parent) : QTreeView(parent)
{
//     setModel(new AkuTreeModel(QVector<QStringList>()));
    setAlternatingRowColors(true);
}

AkuTreeView::~AkuTreeView()
{
}

AkuTreeModel* AkuTreeView::model()
{
    return static_cast<AkuTreeModel*>(QTreeView::model());
}
