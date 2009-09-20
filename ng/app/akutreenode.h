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

#ifndef AKUTREENODE_H
#define AKUTREENODE_H

#include <QStringList>
#include <QVariant>

class AkuTreeNode
{
    public:
        AkuTreeNode(const QStringList &data, AkuTreeNode *parent = 0);
        ~AkuTreeNode();

        void appendChild(AkuTreeNode *);

        AkuTreeNode* child(int row);
        AkuTreeNode* parent();

        int childCount() const;
        int columnCount() const;
        int row() const;

        QString name() const;
        double size();
        double packedSize();
        QString mimeType() const;

        QString data(int column) const;
        /**
         * Set this to true if the current node is a folder.
         */
        void setFolder(bool);
        bool isFolder() const;

        /**
         * Searches through the child folder nodes comparing the given string
         * to the first column data. Returns the pointer to che child
         * or 0 if no child is found.
         */
        AkuTreeNode* findChildFolder(QString);

    private:
        QList<AkuTreeNode*> m_childNodes;
        QStringList m_itemData;

        AkuTreeNode *m_parentNode;

        bool m_folder;
        double *m_size;
        double *m_packedSize;
        
};

Q_DECLARE_METATYPE(AkuTreeNode*)

#endif
