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

#include "PCNStationNamesDialog.h"
#include "ui_PCNStationNamesDialog.h"
#include "Logger.h"

#include <QTableWidgetItem>
#include <QStringList>
#include <QTabWidget>


PCNStationNamesDialog::PCNStationNamesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PCNStationNamesDialog)
{
    ui->setupUi(this);
    ui->tawStationNames490->horizontalHeader()->setStretchLastSection(true);
    ui->tawStationNames518->horizontalHeader()->setStretchLastSection(true);
    ui->twgStationNames->setCurrentIndex(0);
}

PCNStationNamesDialog::~PCNStationNamesDialog()
{
    delete ui;
}

void PCNStationNamesDialog::setData(const QList<PCNStation> & lstStations518, const QList<PCNStation> & lstStations490) {
int nStations, iStation;
QStringList TableHeaders;

  TableHeaders.append(tr("Key"));
  TableHeaders.append(tr("Name"));

  nStations = lstStations518.count();
  ui->tawStationNames518->setRowCount(nStations);
  ui->tawStationNames518->setColumnCount(2);
  ui->tawStationNames518->setHorizontalHeaderLabels(TableHeaders);
  ui->tawStationNames518->verticalHeader()->setVisible(false);

  for (iStation = 0; iStation < nStations; iStation++) {
    QString Letter = lstStations518[iStation].Letter;
    QTableWidgetItem * item = new QTableWidgetItem;
    QIcon icon(":/resources/icon30x30/" + Letter + "-518.png");
    item->setIcon(icon);
    item->setFlags(Qt::ItemIsEnabled);
    item->setData(Qt::UserRole, Letter);
    ui->tawStationNames518->setItem(iStation, 0, item);
    ui->tawStationNames518->setItem(iStation, 1, new QTableWidgetItem(lstStations518[iStation].Name));
  }

  nStations = lstStations490.count();
  ui->tawStationNames490->setRowCount(nStations);
  ui->tawStationNames490->setColumnCount(2);
  ui->tawStationNames490->setHorizontalHeaderLabels(TableHeaders);
  ui->tawStationNames490->verticalHeader()->setVisible(false);

  for (iStation = 0; iStation < nStations; iStation++) {
    // item->setFlags(item->flags() &  ~Qt::ItemIsEditable);
    QString Letter = lstStations518[iStation].Letter;
    QTableWidgetItem * item = new QTableWidgetItem;
    QIcon icon(":/resources/icon30x30/" + Letter + "-490.png");
    item->setIcon(icon);
    item->setFlags(Qt::ItemIsEnabled);
    item->setData(Qt::UserRole, Letter);
    ui->tawStationNames490->setItem(iStation, 0, item);
    ui->tawStationNames490->setItem(iStation, 1, new QTableWidgetItem(lstStations490[iStation].Name));
  }
  ui->twgStationNames->tabBar()->setTabTextColor(0, QColor(0x80, 0x55, 0x40));
  ui->twgStationNames->tabBar()->setTabTextColor(1, QColor(0x20, 0x80, 0x20));
}

void PCNStationNamesDialog::getData(QList<PCNStation> & lstStations518, QList<PCNStation> & lstStations490) {
int nRows, iRow, nStations, iStation;

  nRows = ui->tawStationNames518->rowCount();
  nStations = lstStations518.count();
  for (iRow = 0; iRow < nRows; iRow++) {
    const QString & Letter = ui->tawStationNames518->item(iRow, 0)->data(Qt::UserRole).toString();
    for (iStation = 0; iStation < nStations; iStation++) {
      PCNStation & Station = lstStations518[iStation];
      if (Letter == Station.Letter) {
        Station.Name = ui->tawStationNames518->item(iRow, 1)->text();
        break;
      }
    }
  }

  nRows = ui->tawStationNames490->rowCount();
  nStations = lstStations490.count();
  for (iRow = 0; iRow < nRows; iRow++) {
    const QString & Letter = ui->tawStationNames490->item(iRow, 0)->data(Qt::UserRole).toString();
    for (iStation = 0; iStation < nStations; iStation++) {
      PCNStation & Station = lstStations490[iStation];
      if (Letter == Station.Letter) {
        Station.Name = ui->tawStationNames490->item(iRow, 1)->text();
        break;
      }
    }
  }
}
