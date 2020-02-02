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

#ifndef PCNPREFERENCES_H
#define PCNPREFERENCES_H

#include "ui_PCNPreferences.h"
#include "PCNOptions.h"

class PCNPreferences : public QDialog, private Ui::PCNPreferences
{
    Q_OBJECT

public:
  explicit PCNPreferences(QWidget *parent = 0);

  void setData(PCNOptions * pOptions);
  void getData(PCNOptions * pOptions);

public slots:
  void DeviceVersion(QString Version);

protected slots:

  void on_devicePbtQueryVersion_clicked();

};

#endif // PCNPREFERENCES_H
