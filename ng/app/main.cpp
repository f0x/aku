/***************************************************************************
 *   Copyright 2009 by Francesco Grieco <fgrieco@gmail.com>                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>

#include "mainwindow.h"

int main ( int argc, char *argv[] )
{

  KAboutData aboutData("aku", 0, ki18n("aKu"),
                       "0.2", ki18n("An archiving application for KDE4"), KAboutData::License_GPL_V3,
                       ki18n("Copyright (c) 2009"));

  aboutData.addAuthor(ki18n("Francesco Grieco"),
                      ki18n("Main Developer"),
                            "fgrieco@gmail.com");

  aboutData.addAuthor(ki18n("Alessandro Diaferia"),
                      ki18n("Main Developer"),
                            "alediaferia@gmail.com");

  aboutData.setBugAddress("fgrieco@gmail.com");

  KCmdLineOptions options;
  options.add("+[file]", ki18n("Archive to load"));
  KCmdLineArgs::init( argc, argv, &aboutData );
  KCmdLineArgs::addCmdLineOptions( options );   
  KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

  KApplication app;

  MainWindow *mainwindow = new MainWindow();

  for (int i = 0; i < args->count(); i++) {
    mainwindow->load(KUrl(args->arg(i)));
  }

  mainwindow -> show();  

  return app.exec();
}
