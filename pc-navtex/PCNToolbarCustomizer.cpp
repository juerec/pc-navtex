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

#include "PCNToolbarCustomizer.h"
#include "ui_PCNToolbarCustomizer.h"
#include "PCNOptions.h"

PCNToolbarCustomizer::PCNToolbarCustomizer(QWidget *parent, int maxToolButtons)
                    : QDialog(parent),
                      maxButtonsCount(maxToolButtons) {
  QListWidgetItem * item;
  QIcon icon;
  QString  ResourcePath(":/resources/icon32x32/");
  char  c;

  setupUi(this);

  labTooManySelected->hide();

  icon = QIcon(ResourcePath + "@I.png");
  item = new QListWidgetItem(icon, tr("StationDlg518"), pOptions->lstToolButtons.contains("@I") ? lwgSelected : lwgAvailable);
  item->setData(Qt::UserRole, QString("@I"));

  icon = QIcon(ResourcePath + "@N.png");
  item = new QListWidgetItem(icon, tr("StationDlg490"), pOptions->lstToolButtons.contains("@N") ? lwgSelected : lwgAvailable);
  item->setData(Qt::UserRole, QString("@N")); // iItem);

  for (c = 'A'; c <= 'Z'; c++) {
    if (c == 'Y')
      continue;
    QChar Letter(c);
    icon = QIcon(ResourcePath + "I" + Letter + "a.png");
    item = new QListWidgetItem(icon, QString(Letter) + " - " + tr("international"), pOptions->lstToolButtons.contains(QString("I") + Letter) ? lwgSelected : lwgAvailable);
    item->setData(Qt::UserRole, QString("I") + Letter);
  }

  for (c = 'A'; c <= 'Z'; c++) {
    if (c == 'Y')
      continue;
    QChar Letter(c);
    icon = QIcon(ResourcePath + "N" + Letter + "a.png");
    item = new QListWidgetItem(icon, QString(Letter) + " - " +  tr("national"), pOptions->lstToolButtons.contains(QString("N") + Letter) ? lwgSelected : lwgAvailable);
    item->setData(Qt::UserRole, QString("N") + Letter);
  }
} //=========================================== PCNToolbarCustomizer::PCNToolbarCustomizer()


PCNToolbarCustomizer::~PCNToolbarCustomizer() {

} //=========================================== PCNToolbarCustomizer::~PCNToolbarCustomizer()


void PCNToolbarCustomizer::on_pbtAdd_clicked() {
int iAvailable;
int nSelected, iSelected;
bool ok;

  iAvailable = 0;
  while (iAvailable < lwgAvailable->count()) {
    QListWidgetItem * item = lwgAvailable->item(iAvailable);
    if (item->isSelected()) {
      nSelected = lwgSelected->count();
      item = lwgAvailable->takeItem(iAvailable);
      QString Key = item->data(Qt::UserRole).toString();
      for (iSelected = 0; iSelected < nSelected; iSelected++) {
        if (lwgSelected->item(iSelected)->data(Qt::UserRole).toString() > Key) {
          break;
        }
      }
      lwgSelected->insertItem(iSelected, item);
      continue;
    }
    iAvailable++;
  }
  ok = lwgSelected->count() <= maxButtonsCount;
  pbbBox->button(QDialogButtonBox::Ok)->setEnabled(ok);
  // pbtOk->setEnabled(ok);
  labTooManySelected->setVisible(!ok);
} //=========================================== PCNToolbarCustomizer::on_pbtAdd_clicked()


void PCNToolbarCustomizer::on_pbtSub_clicked() {
int nAvailable, iAvailable;
int iSelected;
bool ok;

  iSelected = 0;
  while (iSelected < lwgSelected->count()) {
    QListWidgetItem * item = lwgSelected->item(iSelected);
    if (item->isSelected()) {
      nAvailable = lwgAvailable->count();
      item = lwgSelected->takeItem(iSelected);
      QString Key = item->data(Qt::UserRole).toString();
      for (iAvailable = 0; iAvailable < nAvailable; iAvailable++) {
        if (lwgAvailable->item(iAvailable)->data(Qt::UserRole).toString() > Key) {
          break;
        }
      }
      lwgAvailable->insertItem(iAvailable, item);
      continue;
    }
    iSelected++;
  }
  ok = lwgSelected->count() <= maxButtonsCount;
  pbbBox->button(QDialogButtonBox::Ok)->setEnabled(ok);
//  pbtOk->setEnabled(ok);
  labTooManySelected->setVisible(!ok);
} //=========================================== PCNToolbarCustomizer::on_pbtSub_clicked()


void PCNToolbarCustomizer::on_pbbBox_accepted() {
int nSelected = lwgSelected->count();
int iSelected;

  pOptions->lstToolButtons.clear();
  for (iSelected = 0; iSelected < nSelected; iSelected++)
    pOptions->lstToolButtons.append(lwgSelected->item(iSelected)->data(Qt::UserRole).toString());
  accept();
} //=========================================== PCNToolbarCustomizer::on_pbbBox_accepted()


void PCNToolbarCustomizer::on_pbbBox_rejected() {
  reject();
} //=========================================== PCNToolbarCustomizer::on_pbbBox_rejected()
