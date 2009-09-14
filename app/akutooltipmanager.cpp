 /* This file is part of the KDE project

   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
#include "akutooltipmanager.h"
#include "akutooltip.h"

#include <QWidget>
#include <QTimer>
#include <QVariant>

#include <KDebug>
#include <KGlobal>

class TooltipManagerHelper
{
public:
    AkuTooltipManager instance;
};

K_GLOBAL_STATIC(TooltipManagerHelper, globalHelper)

//////////////////////// TipContainer ////////////////////////////////

Q_DECLARE_METATYPE(TipContainer)
TipContainer::TipContainer() : widget(0), count(0)
{
    qRegisterMetaType<TipContainer>("TipContainer");
}

 TipContainer::TipContainer(QWidget *widget) : widget(widget), count(0)
{
    qRegisterMetaType<TipContainer>("TipContainer");
}

 TipContainer::TipContainer(const TipContainer &c) : widget(0), count(0)
{
    qRegisterMetaType<TipContainer>("TipContainer");
    widget = c.widget;
    count = c.count;
    akutipList = c.akutipList;
}

TipContainer::~TipContainer()
{}

QWidget* TipContainer::tipWidget() const
{
    return widget;
}

int TipContainer::tipCount() const
{
    return count;
}

bool TipContainer::operator ==(const TipContainer &v) const
{
    if (widget == v.widget && count == v.count) {
        return true;
    }
    return false;
}

TipContainer& TipContainer::operator =(const TipContainer &c)
{
    widget = c.widget;
    akutipList = c.akutipList;
    count = c.count;
    return *this;
}

void TipContainer::operator <<(AkuTooltip *tip)
{
    if (!contains(tip)) {
        akutipList << tip;
        count = akutipList.count();
    }
}

bool TipContainer::removeTip(AkuTooltip *tip)
{
    if (akutipList.removeAll(tip)) {
        count = akutipList.count();
        return true;
    }
    return false;
}
    

bool TipContainer::contains(AkuTooltip *tip) const
{
    foreach (AkuTooltip *tipin, akutipList) {
//         kDebug() << tip << tipin;
        if (tip == tipin) {
            return true;
        }
    }
    return false;
}

void TipContainer::append(AkuTooltip *tip, QWidget *w)
{
    if (!widget) {
        kDebug() << "widget not set.. appending tip" << tip << "and setting widget" << w;
        akutipList << tip;
        count = akutipList.count();
        widget = w;
        return;
    }
    if (w != widget) {
        kDebug() << "different widgets, use another container";
        return;
    }

    akutipList << tip;
    kDebug() << akutipList;
    count = akutipList.count();
}

bool TipContainer::canAppend(AkuTooltip *tip, QWidget *w)
{
    if (w != widget) {
        return false;
    }
    return true;
}

AkuTooltip * TipContainer::last()
{
    return akutipList.isEmpty() ? 0 : akutipList.last();
}

QList <AkuTooltip*> TipContainer::tipList() const
{
    return akutipList;
}

/////////////////// Private /////////////////////////////////////////

class AkuTooltipManager::Private
{
public:
    Private() :
    lastTip(0)
    {
    }

    TipQueue tipQueue;
    AkuTooltip *lastTip;

    void updateLastTip();
};

//////////////////// AkuTooltipManager////////////////////////////////////

AkuTooltipManager::AkuTooltipManager(QObject *parent) : QObject(parent),
                                                        d(new Private())
{
}

AkuTooltipManager::AkuTooltipManager(const AkuTooltipManager &p) : QObject()
{
    d = p.d;
}

AkuTooltipManager& AkuTooltipManager::operator =(const AkuTooltipManager &c)
{
    d = c.d;

    return *this;
}

AkuTooltipManager::~AkuTooltipManager()
{
    delete d;
}

AkuTooltipManager* AkuTooltipManager::instance()
{
//     if (!m_instance) {
//         m_instance = new AkuTooltipManager();
//     }

//     m_instance = globalAkuTooltipManager;

    return &globalHelper->instance;
}

void AkuTooltipManager::registerTooltip(AkuTooltip *tip, QWidget *widget)
{
    enqueue(tip, widget);
    connect(tip, SIGNAL(destroyed(QObject*)), this, SLOT(unregisteringSlot(QObject *)));
    connect(widget, SIGNAL(destroyed(QObject *)), this, SLOT(containerWidgetDeleted(QObject *)));
    connect(tip, SIGNAL(tooltipClosed(AkuTooltip*)), this, SLOT(deleteTooltip(AkuTooltip*)));

//     kDebug() << d->tipQueue.isEmpty();
//     if (d->tipQueue.isEmpty()) return;

    // kDebug() << d->tipQueue; WARNING: Crash!
}

void AkuTooltipManager::deleteTooltip(AkuTooltip *tip)
{
    kDebug() << "deleting tooltip";
    tip->deleteLater();
}

void AkuTooltipManager::enqueue(AkuTooltip *tip, QWidget *w)
{
    d->lastTip = tip;
    foreach (TipContainer c, d->tipQueue) {
        if (c.canAppend(tip, w)) {
            d->tipQueue.removeAll(c);
            c.append(tip, w);
            d->tipQueue << c;
//             kDebug() << d->tipQueue;
            return;
        }
    }

    TipContainer co;
    co.append(tip, w);
    d->tipQueue << co;
}

void AkuTooltipManager::containerWidgetDeleted(QObject *obj)
{
    QWidget *widget = static_cast<QWidget*>(obj);
    foreach (const TipContainer &c, d->tipQueue) {
        if (c.tipWidget() == widget) {
            d->tipQueue.removeAll(c);
            d->updateLastTip();
            return;
         }
     }
}

void AkuTooltipManager::Private::updateLastTip()
{
    if (tipQueue.isEmpty()) {
        lastTip = 0;
        return;
    }

    lastTip = tipQueue.last().last(); // TODO: this is not really the last added
}

void AkuTooltipManager::unregisteringSlot(QObject *obj)
{
    AkuTooltip *tip = static_cast<AkuTooltip*>(obj);
    unregisterTooltip(tip);
}

void AkuTooltipManager::unregisterTooltip(AkuTooltip *tip)
{
    kDebug() << "unregistering" << tip;
    dequeue(tip);
}

void AkuTooltipManager::dequeue(AkuTooltip *tip)
{
    kDebug() << "dequeueing" << tip;
    foreach (TipContainer c, d->tipQueue) {
        if (c.contains(tip)) {
            d->tipQueue.removeAll(c);
            c.removeTip(tip);
            d->tipQueue.append(c);
            kDebug() << d->tipQueue;
            d->updateLastTip();
            return;
        }
    }
}

bool AkuTooltipManager::canShow(AkuTooltip *tip)
{
    foreach (const TipContainer &c, d->tipQueue) {
        if (c.contains(tip) && c.tipCount() == 1) {
           return true;
        }
     }
     return false;         
}

void AkuTooltipManager::showTip(AkuTooltip *tip)
{
    kDebug() << "trying to show";
    if (!isRegistered(tip)) {
        kError() << "cannot show unregistered tooltip" << tip;
        return;
    }

    if (canShow(tip)) {
        kDebug() << "no tooltips showing so we can show this one";
        tip->showTip();
        return;
    }

    QTimer::singleShot(1000, this, SLOT(showPendingTips()));
}

void AkuTooltipManager::showPendingTips()
{
    if (!d->lastTip) {
        return;
    }

    showTip(d->lastTip);
}

bool AkuTooltipManager::isRegistered(AkuTooltip *tip)
{
    foreach (const TipContainer &c, d->tipQueue) {
        if (c.contains(tip)) {
            kDebug() << "contained";
            return true;
         }
    }
    kDebug() << "not contained";
    return false;
}


QDebug operator<<(QDebug dbg, const TipContainer &c)
{
    dbg.nospace() << "TipContainer (" << c.tipCount()
                  << ", " 
                  << c.tipWidget() 
                  << "," 
                  << c.tipList()
                  << ")";
    return dbg.space();
}
