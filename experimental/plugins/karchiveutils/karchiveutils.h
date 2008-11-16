 /*

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
#ifndef KARCHIVEUTILS_H
#define KARCHIVEUTILS_H

#include <KArchive>

class KUrl;
class QStringList;

namespace KArchiveUtils
{
    QString formatPermissions(mode_t permissions);
    void extractArchive(KArchive *archive, const KUrl &destination, const QStringList &files = QStringList());
    
};

#endif 
