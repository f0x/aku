 /* This file is part of the KDE project

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
#include "akutreenode.h"

AkuTreeNode::AkuTreeNode(const QStringList &data, AkuTreeNode *parent) :
                                                    itemData(data),
                                                    parentNode(parent),
                                                    folder(true)
{
}

AkuTreeNode::~AkuTreeNode()
{
    qDeleteAll(childNodes);
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
