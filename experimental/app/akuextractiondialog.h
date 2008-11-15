/*

   Copyright (C) 2008 Francesco Grieco <fgrieco@gmail.com>
   Copyright (C) 2008 Alessandro Diaferia <alediaferia@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/ 
#ifndef AKUEXTRACTIONDIALOG_H
#define AKUEXTRACTIONDIALOG_H

#include <KDialog>

#include "ui_extractiondialog.h"

class AkuExtractionDialog : public KDialog
{
    Q_OBJECT
    public: 
        AkuExtractionDialog(QWidget *parent = 0);
        ~AkuExtractionDialog();

    private:
        Ui::ExtractionDialog ui;

};
#endif
