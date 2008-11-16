 /*

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/

#include "karchiveutils.h"

#include <QStringList>

#include <KUrl>

namespace KArchiveUtils
{
QString formatPermissions(mode_t permissions)
{
    QString pString = "---------"; // permission string

    // we should convert mode_t to human readable permissions

    // Owner permissions:
    // S_IRWXU = rwx
    // S_IRUSR = r
    // S_IWUSR = w
    // S_IXUSR = x

    // Group permissions
    // S_IRWXG = rwx
    // S_IRGRP = r
    // S_IWGRP = w
    // S_IXGRP = x

    // Others permissions
    // S_IRWXO = rwx
    // S_IROTH = r
    // S_IWOTH = w
    // S_IXOTH = x

    // owner
    uint own = permissions & S_IRWXU;
    if (own & S_IRUSR) {
        pString.replace(0, 1, "r");
    }
    if (own & S_IWUSR){
        pString.replace(1, 1, "w");
    }
    if (own & S_IXUSR) {
        pString.replace(2, 1, "x");
    }

    uint grp = permissions & S_IRWXG;
    if (grp& S_IRGRP) {
        pString.replace(3, 1, "r");
    }
    if (grp & S_IWGRP){
        pString.replace(4, 1, "w");
    }
    if (grp & S_IXGRP) {
        pString.replace(5, 1, "x");
    }

    uint oth = permissions & S_IRWXO;
    if (oth & S_IROTH) {
        pString.replace(6, 1, "r");
    }
    if (oth & S_IWOTH){
        pString.replace(7, 1, "w");
    }
    if (oth & S_IXOTH) {
        pString.replace(8, 1, "x");
    }

    
    return pString;
}

void extractArchive(KArchive *m_archive, const KUrl &destination, const QStringList &files)
{
    if (files.isEmpty()) {
        const KArchiveDirectory *mainDir = static_cast<const KArchiveDirectory*>(m_archive->directory());
        mainDir->copyTo(destination.pathOrUrl());
        return;
    }
}

}
