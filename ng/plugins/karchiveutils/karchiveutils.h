/***************************************************************************
 *   Copyright 2009 by Alessandro Diaferia <alediaferia@gmail.com>         *
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
