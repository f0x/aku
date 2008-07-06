#ifndef QUICKEXTRACT_H
#define QUICKEXTRACT_H

#include <QHBoxLayout>
#include <QLabel>
#include <QDir>
#include <QHeaderView>
#include <QCheckBox>
#include <QGroupBox>
#include <QSizePolicy>

#include <KDialog>
#include <KVBox>
#include <KLocale>
#include <KAction>
#include <KFileTreeView>
#include <KUrlCompletion>
#include <KHistoryComboBox>
#include <KDialogButtonBox>
#include <KGuiItem>
#include <KPushButton>


#include <KDebug>

class quickExtract : public KDialog
{
 Q_OBJECT

  public:
   quickExtract (QWidget *parent = 0);
   ~quickExtract();

   KFileTreeView *treeView;
   KHistoryComboBox *khistory;
  
  public slots:
   virtual void hiddenFiles(bool);
   virtual void updateCombo(KUrl);
   virtual void updateTreeViewSelection(QString);

  private:
   KAction *showhiddenAction;
};

#endif
