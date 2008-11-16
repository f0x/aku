/*
   Copyright (C) 2008 Francesco Grieco <fgrieco@gmail.com>
   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/ 
#include "akuextractiondialog.h"

#include <KLocale>

AkuExtractionDialog::AkuExtractionDialog(QWidget *parent) : KDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    QWidget *widget = new QWidget(this);
    ui.setupUi(widget);
    setMainWidget(widget);

    dirView = new KFileTreeView(widget);
    dirView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    ui.horizontalLayout->insertWidget(0, dirView);
    
    setButtons(KDialog::Ok | KDialog::Cancel | KDialog::Default);
    setCaption(i18n("Extraction"));
}

AkuExtractionDialog::~AkuExtractionDialog()
{}
