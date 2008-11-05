 /* 
   Copyright (C) 2008 Francesco Grieco <fgrieco@gmail.com>
   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/ 

#ifndef OPENARCHIVE_H
#define OPENARCHIVE_H

#include <KUrl>
#include <KMimeType>

class AkuPlugin;

class OpenArchive : public QObject
{
  Q_OBJECT

  public:
    OpenArchive(QWidget *parent);
    ~OpenArchive();

    void setAvailablePlugins(const QMap<QString, AkuPlugin*> &plugins);

  private:
    KMimeType::Ptr mimetype;
    QMap<QString, AkuPlugin*> m_availablePlugins;

  public slots:
    void load (const KUrl &url);

  signals:
    void fileLoaded(const KUrl &);
};

#endif
