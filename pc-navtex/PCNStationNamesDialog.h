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

#ifndef PCNSTATIONNAMESDIALOG_H
#define PCNSTATIONNAMESDIALOG_H

#include <QDialog>
#include <QList>

#include "PCNStation.h"


namespace Ui {
class PCNStationNamesDialog;
}

class PCNStationNamesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PCNStationNamesDialog(QWidget *parent = 0);
    ~PCNStationNamesDialog();

    void setData(const QList<PCNStation> & lstStations518, const QList<PCNStation> & lstStations490);
    void getData(QList<PCNStation> & lstStations518, QList<PCNStation> & lstStations490);

private:
    Ui::PCNStationNamesDialog *ui;
};

#endif // PCNSTATIONNAMESDIALOG_H
