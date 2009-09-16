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
#include <KSharedPtr>

class KUrl;
class QStringList;


class KArchiveUtils : public QObject, public QSharedData
{
    Q_OBJECT
    public:

        typedef KSharedPtr< KArchiveUtils > Ptr;

        virtual ~KArchiveUtils() {}

        QString formatPermissions(mode_t permissions);
        void extractArchive(KArchive *archive, const KUrl &destination, const QStringList &files = QStringList());
        int currentExtractionIndex();

        static KArchiveUtils::Ptr self();

    private:
        int m_current;

    protected:
        KArchiveUtils(QObject *parent = 0) : QObject(parent), m_current(0)
        {}
};

#endif 
