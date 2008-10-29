 /* This file is part of the KDE project

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
#ifndef AKUTREENODE_H
#define AKUTREENODE_H

#include <QStringList>

class AkuTreeNode
{
    public:
        AkuTreeNode(const QStringList &data, AkuTreeNode *parent = 0);
        ~AkuTreeNode();

        void appendChild(AkuTreeNode *);

        AkuTreeNode* child(int row);
        int childCount() const;
        int columnCount() const;
        QString data(int column) const;
        int row() const;
        AkuTreeNode* parent();

    private:
        QList<AkuTreeNode*> childNodes;
        QStringList itemData;
        AkuTreeNode *parentNode;
        
};

#endif
