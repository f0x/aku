/***************************************************************************
 *   Copyright 2009 by Francesco Grieco <fgrieco@gmail.com>                *
 *                     Alessandro Diaferia <alediaferia@gmail.com>         *
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

#ifndef RARPLUGIN_H
#define RARPLUGIN_H

#include <akuplugin.h>
#include <aku_macros.h>

#include <QVariantList>

#include <KUrl>

class RarPlugin : public AkuPlugin
{
    Q_OBJECT
    public:
        RarPlugin(QObject *parent, const QVariantList &args);
        ~RarPlugin();

        QStringList mimeTypeNames();
        QStringList additionalHeaderStrings();

        bool canExtract();
        bool canCreate();
        bool canRename();
        bool canDelete();
        bool canAdd();
        bool canEncrypt();
        bool canAddComment();
        bool canLock();

        bool isInstalled();

        void loadArchive();
        void init(const KUrl &fileName, const QString &password = 0);

        void lockArchive();
        void extractArchive(const KUrl &destination, const QStringList &files);

    private:
        KUrl m_fileName;
        QString m_password;

};

#endif
