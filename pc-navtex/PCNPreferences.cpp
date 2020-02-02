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

#include "PCNPreferences.h"
#include "PCNDatabase.h"
#include "PCNSerialPort.h"

PCNPreferences::PCNPreferences(QWidget *parent)
              : QDialog(parent) {
  setupUi(this);
}

void PCNPreferences::setData(PCNOptions * pOptions) {


  databaseLabLastMessageFrom->setText("987");
  databaseLabMessageCount->setText("1234");
  databaseSpbOlderThan->setValue(pOptions->deleteMessagesOlderThan);
  databaseSpbMaxAge->setValue(pOptions->maxMessageAgeToQuery());
  databaseSpbMaxRead->setValue(pOptions->maxMessageCountToQuery());
  deviceLedDevice->setText(pOptions->DeviceName);
  databaseLabMessageCount->setText(QString::number(pDatabase->countOfMessages()));
  databaseLabLastMessageFrom->setText(pDatabase->oldestMessageDate() + " -- " + pDatabase->newestMessageDate());
}

void PCNPreferences::getData(PCNOptions * pOptions) {

  pOptions->setMaxMessageAgeToQuery(databaseSpbMaxAge->value());
  pOptions->setMaxMessageCountToQuery(databaseSpbMaxRead->value());
  pOptions->deleteMessagesOlderThan = databaseSpbOlderThan->value();
  pOptions->DeviceName = deviceLedDevice->text();

}

void PCNPreferences::DeviceVersion(QString Version) {
  deviceLabVersion->setText(Version);
}

void PCNPreferences::on_devicePbtQueryVersion_clicked() {
QString DeviceName(deviceLedDevice->text());

  deviceLabVersion->setText("?");
  pSerialPort->queryVersion();
}
