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

#include "PCNOptions.h"

PCNOptions::PCNOptions() {

#ifdef Q_OS_LINUX
  DeviceName          = "/dev/navtex";
#else
#ifdef Q_OS_WIN32
  DeviceName          = "COM1";
#else
  ++++++++
  place correct code here
#endif
#endif
  deleteMessagesOlderThan = 100;

} //=========================================== PCNOptions::PCNOptions()


void PCNOptions::readOptions(int , char **) {
QDir      Dir;
QSettings settings;

  settings.setIniCodec("UTF-8");

  settings.beginGroup("Preferences");
  PathData = settings.value("PathData", QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation)[0]).toString();
  if (!Dir.exists(PathData))
    Dir.mkpath(PathData);
  Locale = settings.value("Locale", "de").toString();
  if (Locale.isEmpty() || (Locale != "de" && Locale != "en"))
    Locale = "de";
  settings.endGroup();

  settings.beginGroup("MainWindow");
  MainWindow_xy       = settings.value("xy", QPoint(40, 40)).toPoint();
  MainWindow_wh       = settings.value("wh", QSize(400, 300)).toSize();
  MainWindow_format   = settings.value("format", "normal").toString();
  MainWindow_Col1     = settings.value("Col1",  80).toInt();
  MainWindow_Col2     = settings.value("Col2",  80).toInt();
  MainWindow_FontSize = settings.value("FontSize", 12).toInt();
  if (MainWindow_FontSize < 4 || MainWindow_FontSize > 32)
    MainWindow_FontSize = 12;
  lstToolButtons      = settings.value("ToolButtons").toStringList();
  settings.endGroup();

  MessageFilter.readFromSettings(settings);

  readStations(490, lstStations490, settings);
  readStations(518, lstStations518, settings);

  settings.beginGroup("Preferences");
  DeviceName           = settings.value("DeviceName", DeviceName).toString();
  deleteMessagesOlderThan = settings.value("deleteMessagesOlderThan", 100).toInt();
  settings.endGroup();

} //=========================================== PCNOptions::readOptions()


void PCNOptions::save() {
QSettings settings;

  settings.setIniCodec("UTF-8");
  settings.beginGroup("MainWindow");
  settings.setValue("xy", MainWindow_xy);
  settings.setValue("wh", MainWindow_wh);
  settings.setValue("format", MainWindow_format);
  settings.setValue("Col1", MainWindow_Col1);
  settings.setValue("Col2", MainWindow_Col2);
  settings.setValue("ToolButtons", lstToolButtons);
  settings.endGroup();

  writeStations(490, lstStations490, settings);
  writeStations(518, lstStations518, settings);

  settings.beginGroup("Preferences");
  settings.setValue("DeviceName", DeviceName);
  settings.setValue("deleteMessagesOlderThan", deleteMessagesOlderThan);
  settings.endGroup();

  MessageFilter.writeToSettings(settings);

} //=========================================== PCNOptions::save()


void PCNOptions::readStations(int Type, QList<PCNStation> & lstStations, QSettings & settings) {
const char   Keys[26] = { "ABCDEFGHIJKLMNOPQRSTUVWXZ" };

  settings.beginGroup("Stations-" + QString::number(Type));
  for (int i = 0; i < 25; i++) {
    QChar Key(Keys[i]);
    QString name = settings.value(Key, Key == 'Z' ? '*' : Key).toString();
    lstStations.append(PCNStation(Key, name));
  }
  settings.endGroup();
} //=========================================== PCNOptions::readStations()


void PCNOptions::writeStations(int Type, QList<PCNStation> & lstStations, QSettings & settings) {
int nStations = lstStations.count();

  settings.beginGroup("Stations-" + QString::number(Type));
  for (int iStation = 0; iStation < nStations; iStation++) {
    PCNStation & Station = lstStations[iStation];
    settings.setValue(Station.Letter, lstStations[iStation].Name);
  }
  settings.endGroup();
} //=========================================== PCNOptions::writeStations()


const QString PCNOptions::DatabaseName() {
  return QStandardPaths::standardLocations(QStandardPaths::AppDataLocation)[0] + ".db";
} //=========================================== PCNOptions::DatabaseName()


const QString PCNOptions::StationName(const QString & Key) {
QList<PCNStation> * lst = NULL;
int   nStations, iStation;
QChar Letter = Key[1];

  switch (Key[0].toLatin1()) {
  case 'N' :
    lst = &lstStations490;
    break;
  case 'I' :
    lst = &lstStations518;
    break;
  default :
    return "";
  }
  nStations = lst->count();

  for (iStation = 0; iStation < nStations; iStation++)
    if (lst->at(iStation).Letter == Letter)
      return lst->at(iStation).Name;
  return "";
} //=========================================== PCNOptions::StationName()


int PCNOptions::maxMessageAgeToQuery() {
  return MessageFilter.maxDaysOld;
} //=========================================== PCNOptions::


int PCNOptions::maxMessageCountToQuery() {
  return MessageFilter.maxSelectCount;
} //=========================================== PCNOptions::


void PCNOptions::setMaxMessageAgeToQuery(int maxMessageAge) {
  MessageFilter.maxDaysOld = maxMessageAge;
} //=========================================== PCNOptions::


void PCNOptions::setMaxMessageCountToQuery(int maxMessageCount) {
  MessageFilter.maxSelectCount = maxMessageCount;
} //=========================================== PCNOptions::
