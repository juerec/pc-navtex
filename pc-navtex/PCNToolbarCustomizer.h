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

#ifndef PCNTOOLBARCUSTOMIZER_H
#define PCNTOOLBARCUSTOMIZER_H

#include <QDialog>

#include "ui_PCNToolbarCustomizer.h"

namespace Ui {
class PCNToolbarCustomizer;
}

class PCNToolbarCustomizer : public QDialog, private Ui::PCNToolbarCustomizer {
  Q_OBJECT

public:
  explicit PCNToolbarCustomizer(QWidget *parent = nullptr, int maxToolButtons = 12);
  ~PCNToolbarCustomizer();

public slots :

  void on_pbtAdd_clicked();
  void on_pbtSub_clicked();
//  void on_pbtOk_clicked();
//  void on_pbtCancel_clicked();
  void on_pbbBox_accepted();
  void on_pbbBox_rejected();


protected :

  int maxButtonsCount;
};

#endif // PCNTOOLBARCUSTOMIZER_H
