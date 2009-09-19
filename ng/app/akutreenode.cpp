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
                                                    itemData(data),
                                                    parentNode(parent),
                                                    folder(true)
{
}

AkuTreeNode::~AkuTreeNode()
{
    qDeleteAll(childNodes);
    childNodes.clear();
}

void AkuTreeNode::appendChild(AkuTreeNode *child)
{
    childNodes << child;
}

int AkuTreeNode::childCount() const
{
    return childNodes.size();
}

int AkuTreeNode::columnCount() const
{
    return itemData.size();
}

AkuTreeNode* AkuTreeNode::parent()
{
    return parentNode;
}

QString AkuTreeNode::data(int column) const
{
    // NOTE: maybe this first if is unuseful
    if (itemData.isEmpty()) {
        return QString();
    }

    if (column >= itemData.size()) {
        return QString();
    }

    return itemData[column];
}

int AkuTreeNode::row() const
{
    if (parentNode) {
        return parentNode->childNodes.indexOf(const_cast<AkuTreeNode*>(this));
    }

    return 0;
}

AkuTreeNode* AkuTreeNode::child(int row)
{
    if (row > childNodes.size()) {
        return 0;
    }

    return childNodes[row];
}

bool AkuTreeNode::isFolder()
{
    return folder;
}

void AkuTreeNode::setFolder(bool set)
{
    folder = set;
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
    if (itemData.count() < 1) {
        return QString();
    }

    return itemData.at(0);
}

double AkuTreeNode::size() const
{
    if (itemData.count() < 2) {
        return 0;
    }

    return itemData.at(1).toDouble();
}

double AkuTreeNode::packedSize() const
{
    if (itemData.count() < 3) {
        return 0;
    }

    return itemData.at(2).toDouble();
}

QString AkuTreeNode::mimeType() const
{
    if (itemData.count() < 1) {
        return QString();
    }

    if (isFolder()) {
        return "inode/directory";
    }

    return KMimeType::findByPath(itemData.at(0))->name();
}
