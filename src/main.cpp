#include <QTextCodec>

#include <KApplication>
#include <KCmdLineArgs>
#include <KIconLoader>
#include <KAboutData>
#include <KLocale>
#include <KIcon>
#include <KUrl>

#include "mainwindow.h"

int main ( int argc, char *argv[] )
{

  KAboutData aboutData("aku", 0, ki18n("aKu"),
                       "1.0beta1", ki18n("The RAR/ZIP/TAR application for KDE4"), KAboutData::License_GPL_V3,
                       ki18n("Copyright (c) 2008"));

  aboutData.addAuthor(ki18n("Francesco Grieco aka \"The Japanese GUI stylist\""),
                      ki18n("Main Developer"),
                            "fgrieco@gmail.com");
   
  aboutData.addAuthor(ki18n("Alessandro Diaferia aka \"The Speedy Coder\""), 
                      ki18n("Main Developer"),
                            "alediaferia@gmail.com");

  aboutData.addCredit(ki18n("Riccardo \"Ruphy\" Iaconelli"),
                      ki18n("The beautiful ratiobar"), 
                            "riccardo@kde.org");

  // chiamata per disattivare il report dei bugs a kde4
  aboutData.setCustomAuthorText(ki18n(0), ki18n(0));

  KCmdLineArgs::init(argc, argv, &aboutData);
  KCmdLineOptions options;
  options.add("+[archive]", ki18n("Makes aKu open the archive specified"));
  options.add("extracthere", ki18n("Extracts all files in the archive path")); // FIXME: extracthere <destination>
  options.add("extractto", ki18n("Extracts all files in a selected path"));
  KCmdLineArgs::addCmdLineOptions(options);

  KApplication app;

  QString loIcon = KIconLoader().iconPath("aku",-KIconLoader::SizeEnormous);
  QApplication::setWindowIcon(KIcon(loIcon));

  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
  MainWindow *mainwindow = new MainWindow();

  KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

  if (args -> isSet("extracthere")) {
    quickExtract *dirextract = new quickExtract(args -> arg(0), "extractHere", mainwindow);
    app.setQuitOnLastWindowClosed(false);
  }

  else if (args -> isSet("extractto")) {
    // code to extract the archive
    quickExtract *dirextract = new quickExtract(args -> arg(0), "extractTo", mainwindow);
    dirextract -> show();
  }

  else {
    for (int i=0; i < args -> count(); i++) mainwindow -> openUrl(args -> url(i));
    mainwindow -> show();
  }

  args -> clear();

  return app.exec();

}
