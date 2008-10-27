#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>

#include "mainwindow.h"

int main ( int argc, char *argv[] )
{

  KAboutData aboutData("aku", 0, ki18n("aKu"),
                       "1.1", ki18n("The RAR/ZIP/TAR application for KDE4"), KAboutData::License_GPL_V3,
                       ki18n("Copyright (c) 2008"));

  aboutData.addAuthor(ki18n("Francesco Grieco aka \"The Japanese GUI stylist\""),
                      ki18n("Main Developer"),
                            "fgrieco@gmail.com");
   
  aboutData.addCredit(ki18n("Alessandro Diaferia aka \"The Speedy Coder\""), 
                      ki18n("Main Developer"),
                            "alediaferia@gmail.com");

  aboutData.addCredit(ki18n("Riccardo \"Ruphy\" Iaconelli"),
                      ki18n("The beautiful ratiobar"), 
                            "riccardo@kde.org");

  KCmdLineArgs::init( argc, argv, &aboutData );

  KApplication app;

  MainWindow *mainwindow = new MainWindow();
  mainwindow -> show();  

  return app.exec();

}