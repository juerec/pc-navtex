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

#include "PCNStationFilterDialog.h"

#include "PCNStation.h"
#include "PCNMessageFilter.h"
#include "PCNOptions.h"

PCNStationFilterDialog::PCNStationFilterDialog(QWidget *parent)
                      : QDialog(parent) {
  setupUi(this);

  tbtA->installEventFilter(this);
  tbtB->installEventFilter(this);
  tbtC->installEventFilter(this);
  tbtD->installEventFilter(this);
  tbtE->installEventFilter(this);
  tbtF->installEventFilter(this);
  tbtG->installEventFilter(this);
  tbtH->installEventFilter(this);
  tbtI->installEventFilter(this);
  tbtJ->installEventFilter(this);
  tbtK->installEventFilter(this);
  tbtL->installEventFilter(this);
  tbtM->installEventFilter(this);
  tbtN->installEventFilter(this);
  tbtO->installEventFilter(this);
  tbtP->installEventFilter(this);
  tbtQ->installEventFilter(this);
  tbtR->installEventFilter(this);
  tbtS->installEventFilter(this);
  tbtT->installEventFilter(this);
  tbtU->installEventFilter(this);
  tbtV->installEventFilter(this);
  tbtW->installEventFilter(this);
  tbtX->installEventFilter(this);
  tbtZ->installEventFilter(this);
} //=========================================== PCNStationFilterDialog::PCNStationFilterDialog()


void PCNStationFilterDialog::setData(int Type) {
PCNMessageFilter::FilterType whichFilter = (Type == 490) ? PCNMessageFilter::STATIONS_490 : PCNMessageFilter::STATIONS_518;
const PCNMessageFilter & Filter = pOptions->MessageFilter;
const QList<PCNStation> & lstStations = (Type == 490) ? pOptions->lstStations490 : pOptions->lstStations518;
int nStations, iStation;
QString IconResource(":/resources/icon32x32/");


  setWindowTitle(tr("Station Filter for") + " " + QString::number(Type) + " kHz");

  IconResource += (Type == 490 ? "N" : "I");
  nStations = lstStations.count();
  for (iStation = 0; iStation < nStations; iStation++) {
    const PCNStation & Station = lstStations[iStation];
    QToolButton * pToolButton = findChild<QToolButton*>(QString("tbt") + Station.Letter);
    pToolButton->setChecked(Filter.isFilterElement(whichFilter, Station.Letter));
    pToolButton->setToolTip(QString(Station.Letter) + " : " +Station.Name);
    QIcon icon;
    icon.addFile(IconResource + Station.Letter + "p.png", QSize(), QIcon::Normal, QIcon::Off);
    icon.addFile(IconResource + Station.Letter + "a.png", QSize(), QIcon::Normal, QIcon::On);
    pToolButton->setIcon(icon);
  }
} //=========================================== PCNStationFilterDialog::setData()


void PCNStationFilterDialog::getData(int Type) {
PCNMessageFilter::FilterType whichFilter = (Type == 490) ? PCNMessageFilter::STATIONS_490 : PCNMessageFilter::STATIONS_518;
PCNMessageFilter & Filter = pOptions->MessageFilter;
const QList<PCNStation> & lstStations = (Type == 490) ? pOptions->lstStations490 : pOptions->lstStations518;
int nStations, iStation;

  nStations = lstStations.count();
  for (iStation = 0; iStation < nStations; iStation++) {
    const PCNStation & Station = lstStations[iStation];
    QToolButton * pToolButton = findChild<QToolButton*>(QString("tbt") + Station.Letter);
    Filter.setFilterElement(whichFilter, Station.Letter, pToolButton->isChecked());
  }
} //=========================================== PCNStationFilterDialog::getData()


bool PCNStationFilterDialog::eventFilter(QObject *obj, QEvent *event) {
// https://stackoverflow.com/questions/41549207/how-to-recognize-qmouseevent-inside-child-widgets

  if (event->type() == QEvent::Enter)
    labInfo->setText( ((QToolButton*)obj)->toolTip());

  if (event->type() == QEvent::Leave)
    labInfo->setText("");

  return QDialog::eventFilter(obj, event);
} //=========================================== PCNStationFilterDialog::eventFilter()
