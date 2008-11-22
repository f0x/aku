 /* This file is part of the KDE project

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
#ifndef AKUTOOLTIPMANAGER_H
#define AKUTOOLTIPMANAGER_H

#include <QSharedData>
#include <QObject>
#include <QList>
#include <QPair>
#include <QDebug>

#include <KSharedPtr>

class QWidget;
class AkuTooltip;
class TipContainer;

class AkuTooltipManager : public QObject , public QSharedData
{
    Q_OBJECT
    public:
        typedef KSharedPtr<AkuTooltipManager> Ptr;
        typedef QList< TipContainer > TipQueue;

        ~AkuTooltipManager();

        static AkuTooltipManager::Ptr instance();

        /**
         * @param widget the widget that "hosts" the tooltip.
         * The tooltip will be autmatically unregistered when gets deleted.
         */
        void registerTooltip(AkuTooltip *toolTip, QWidget *widget);

        void unregisterTooltip(AkuTooltip *toolTip);

        void showTip(AkuTooltip *);

        /**
         * @return true if the tooltip is correctly registered.
         */
        bool isRegistered(AkuTooltip*);

    protected:
        AkuTooltipManager(QObject *parent = 0);
        void enqueue(AkuTooltip *tip, QWidget *w);
        void dequeue(AkuTooltip *tip);
        bool canShow(AkuTooltip *tip);

    protected slots:
        void unregisteringSlot(QObject *obj);
        void containerWidgetDeleted(QObject *obj);
        void showPendingTips();
        void deleteTooltip(AkuTooltip *);

    private:

        class Private;
        Private *d;
};

class TipContainer
{
    public:
        TipContainer();

        TipContainer(QWidget* widget);
        ~TipContainer();

         QWidget* tipWidget() const;
         int tipCount() const;

         bool operator ==(const TipContainer &v) const;
         void operator <<(AkuTooltip *tip);

         bool removeTip(AkuTooltip *tip);
         bool contains(AkuTooltip *tip) const;
         bool append(AkuTooltip *tip, QWidget *w);
         AkuTooltip * last();
         QList <AkuTooltip*> tipList() const;

    private:
        QWidget *widget;
        QList<AkuTooltip*> akutipList;
        int count;
};

#endif