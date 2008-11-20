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
        if (tip == tipin) {
            return true;
        }
    }
    return false;
}

bool TipContainer::append(AkuTooltip *tip, QWidget *w)
{
    if (!widget) {
        kDebug() << "widget not set.. appending tip" << tip << "and setting widget" << w;
        akutipList << tip;
        count = akutipList.count();
        widget = w;
        return true;
    }
    if (w != widget) {
        kDebug() << "different widgets, use another container";
        return false;
    }

    akutipList << tip;
    count = akutipList.count();
    return true;
}

AkuTooltip * TipContainer::last() {
    return akutipList.last();
}

QList <AkuTooltip*> TipContainer::tipList() const
{
    return akutipList.isEmpty() ? QList<AkuTooltip*>() : akutipList;
}

/////////////////// Private /////////////////////////////////////////

class AkuTooltipManager::Private
{
public:
    Private(AkuTooltipManager *q) :
    q(q), lastTip(0)
    {
    }

    AkuTooltipManager *q;
    TipQueue tipQueue;
    AkuTooltip *lastTip;

    void updateLastTip();
};

//////////////////// AkuTooltipManager////////////////////////////////////

AkuTooltipManager::AkuTooltipManager(QObject *parent) : QObject(parent),
                                                        d(new Private(this))
{
}

AkuTooltipManager::~AkuTooltipManager()
{
    delete d;
}

AkuTooltipManager::Ptr AkuTooltipManager::instance()
{
    static AkuTooltipManager::Ptr instance(0);
    if (!instance) {
        instance = new AkuTooltipManager;
    }

    return instance;
}

void AkuTooltipManager::registerTooltip(AkuTooltip *tip, QWidget *widget)
{
    enqueue(tip, widget);
    connect(tip, SIGNAL(destroyed(QObject*)), this, SLOT(unregisteringSlot(QObject *)));
    connect(widget, SIGNAL(destroyed(QObject *)), this, SLOT(containerWidgetDeleted(QObject *)));
    connect(tip, SIGNAL(tooltipClosed(AkuTooltip*)), this, SLOT(deleteTooltip(AkuTooltip*)));

    kDebug() << d->tipQueue.isEmpty();
    if (d->tipQueue.isEmpty()) return;

    // kDebug() << d->tipQueue; WARNING: Crash!
}

void AkuTooltipManager::deleteTooltip(AkuTooltip *tip)
{
    tip->deleteLater();
}

void AkuTooltipManager::enqueue(AkuTooltip *tip, QWidget *w)
{
    d->lastTip = tip;
    foreach (TipContainer c, d->tipQueue) {
        if (c.append(tip, w)) {
            kDebug() << "append successful";
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
    foreach (TipContainer c, d->tipQueue) {
        if (c.removeTip(tip)) {
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
    showTip(d->lastTip);
}

bool AkuTooltipManager::isRegistered(AkuTooltip *tip)
{
    foreach (const TipContainer &c, d->tipQueue) {
        if (c.contains(tip)) {
            return true;
         }
    }
    return false;
}


QDebug operator<<(QDebug dbg, const TipContainer &c)
{
    dbg.nospace() << "TipContainer (" << c.tipCount()
                  << ", " << c.tipWidget() << "," << c.tipList() << ")";
    return dbg.space();
}
