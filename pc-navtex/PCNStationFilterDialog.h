/****************************************************************************
**
**  This file is a part of the program "PC NAVTEX".
**
**  Copyright © 2020 Jürgen Reche
**
** PC NAVTEX is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PC NAVTEX is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program. If not, see <https://www.gnu.org/licenses/>.
**
****************************************************************************/

#ifndef PCNSTATIONFILTERDIALOG_H
#define PCNSTATIONFILTERDIALOG_H

#include <QList>

#include "ui_PCNStationFilterDialog.h"

#include "PCNStation.h"


class PCNStationFilterDialog : public QDialog, private Ui::PCNStationFilterDialog
{
    Q_OBJECT

public:
    explicit PCNStationFilterDialog(QWidget *parent = 0);

    void setData(int Type);
    void getData(int Type);

protected:

   virtual bool eventFilter(QObject *obj, QEvent *event) override;

};

#endif // PCNSTATIONFILTERDIALOG_H
