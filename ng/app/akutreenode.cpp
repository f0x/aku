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

#include "akutreenode.h"

#include <KMimeType>

AkuTreeNode::AkuTreeNode(const QStringList &data, AkuTreeNode *parent) :
                                                    m_itemData(data),
                                                    m_parentNode(parent),
                                                    m_folder(true),
                                                    m_size(0),
                                                    m_packedSize(0)
{
}

AkuTreeNode::~AkuTreeNode()
{
    qDeleteAll(m_childNodes);
    m_childNodes.clear();

    delete m_size;
    delete m_packedSize;
}

void AkuTreeNode::appendChild(AkuTreeNode *child)
{
    m_childNodes << child;
}

int AkuTreeNode::childCount() const
{
    return m_childNodes.size();
}

int AkuTreeNode::columnCount() const
{
    return m_itemData.size();
}

AkuTreeNode* AkuTreeNode::parent()
{
    return m_parentNode;
}

QString AkuTreeNode::data(int column) const
{
    // NOTE: maybe this first if is unuseful
    if (m_itemData.isEmpty()) {
        return QString();
    }

    if (column >= m_itemData.size()) {
        return QString();
    }

    return m_itemData[column];
}

int AkuTreeNode::row() const
{
    if (m_parentNode) {
        return m_parentNode->m_childNodes.indexOf(const_cast<AkuTreeNode*>(this));
    }

    return 0;
}

AkuTreeNode* AkuTreeNode::child(int row)
{
    if (row > m_childNodes.size()) {
        return 0;
    }

    return m_childNodes[row];
}

bool AkuTreeNode::isFolder() const
{
    return m_folder;
}

void AkuTreeNode::setFolder(bool set)
{
    m_folder = set;
}

AkuTreeNode* AkuTreeNode::findChildFolder(QString name)
{
    for (int i = 0; i < childCount(); i++) {
        if (child(i)->data(0) == name && child(i)->isFolder()) {
            return child(i);
        }
    }
    return 0;
}

QString AkuTreeNode::name() const
{
    if (m_itemData.count() < 1) {
        return QString();
    }

    return m_itemData.at(0);
}

double AkuTreeNode::size()
{
    if (isFolder()) {
        double size = 0;
        QList<AkuTreeNode*>::const_iterator it = m_childNodes.begin();
        for (; it != m_childNodes.end(); ++it) {
            size += static_cast<AkuTreeNode*>(*it)->size();
        }
        return size;
    }

    if (2 <= m_itemData.count() && !m_size) {
        m_size = new double(m_itemData.at(1).toDouble());
    }
    return *m_size;
}

double AkuTreeNode::packedSize()
{
    if (m_itemData.count() < 3) {
        return 0;
    }

    if (3 <= m_itemData.count() && !m_packedSize) {
        m_packedSize = new double(m_itemData.at(2).toDouble());
    }
    return *m_packedSize;
}

QString AkuTreeNode::mimeType() const
{
    if (m_itemData.count() < 1) {
        return QString();
    }

    if (isFolder()) {
        return "inode/directory";
    }

    return KMimeType::findByPath(m_itemData.at(0))->name();
}
