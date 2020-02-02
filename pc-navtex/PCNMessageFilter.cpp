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

#include "PCNMessageFilter.h"

// static
const char * PCNMessageFilter::AllStations = "ABCDEFGHIJKLMNOPQRSTUVWXZ";
// static
const char * PCNMessageFilter::AllTypes = "ABCDEZ";



PCNMessageFilter::PCNMessageFilter()
                : activeStations490(AllStations),
                  activeStations518(AllStations),
                  activeMessageTypes(AllTypes),
                  maxDaysOld(0),
                  maxSelectCount(100) {
} //=========================================== PCNMessageFilter::PCNMessageFilter()


void PCNMessageFilter::readFromSettings(QSettings & settings) {
QString str;
int     x;

  settings.beginGroup("MessageFilter");
  // filter out any stange or twice characters and sort
  activeStations490 = "";
  str = settings.value("activeStations490", QString(AllStations).toLower()).toString();
  for (x = 0; x < str.length(); x++)
    if (str[x] >= 'A' && str[x] <= 'Z')
      addElement(activeStations490, str[x]);
  activeStations518 = "";
  str = settings.value("activeStations518", QString(AllStations).toLower()).toString();
  for (x = 0; x < str.length(); x++)
    if (str[x] >= 'A' && str[x] <= 'Z')
      addElement(activeStations518, str[x]);
  activeMessageTypes = "";
  str = settings.value("activeMessageTypes", QString(AllTypes).toLower()).toString();
  for (x = 0; x < str.length(); x++)
    if (str[x] >= 'A' && str[x] <= 'Z')
      addElement(activeMessageTypes, str[x]);

  maxDaysOld = settings.value("maxDaysOld", 100).toInt();
  maxSelectCount = settings.value("maxSelectCount", 100).toInt();
  settings.endGroup();
} //=========================================== PCNMessageFilter::readFromSettings()


void PCNMessageFilter::writeToSettings(QSettings & settings) {

  settings.beginGroup("MessageFilter");
  settings.setValue("activeStations490", activeStations490);
  settings.setValue("activeStations518", activeStations518);
  settings.setValue("activeMessageTypes", activeMessageTypes);
  settings.setValue("maxDaysOld", maxDaysOld);
  settings.setValue("maxSelectCount", maxSelectCount);
  settings.endGroup();
} //=========================================== PCNMessageFilter::writeToSettings()


void PCNMessageFilter::addElement(QString & active, QChar Element) {
int length = active.length();
int x = 0;

  while (x < length && active[x] < Element)
    x++;
  if (x < length) {
    if (active[x] != Element)
      active.insert(x, Element);
  } else {
    active.append(Element);
  }
} //=========================================== PCNMessageFilter::addElement()


void PCNMessageFilter::remElement(QString & active, QChar Element) {
  active.replace(QString(Element), "");
} //=========================================== PCNMessageFilter::remElement()


void PCNMessageFilter::setFilterElement(FilterType filter, QChar Element, bool check) {

  switch (filter) {
  case MSG_TYPES :
    if (check)
      addElement(activeMessageTypes, Element);
    else
      remElement(activeMessageTypes, Element);
    break;
  case STATIONS_490 :
    if (check)
      addElement(activeStations490, Element);
    else
      remElement(activeStations490, Element);
    break;
  case STATIONS_518 :
    if (check)
      addElement(activeStations518, Element);
    else
      remElement(activeStations518, Element);
    break;
  }
} //=========================================== PCNMessageFilter::setFilterElement()


void PCNMessageFilter::setFilterElement(const QString &Key, bool check) {

  switch (Key[0].toLatin1()) {
  case 'Y' :
    setFilterElement(MSG_TYPES, Key[1], check);
    break;
  case 'N' :
    setFilterElement(STATIONS_490, Key[1], check);
    break;
  case 'I' :
    setFilterElement(STATIONS_518, Key[1], check);
    break;
  }
} //=========================================== PCNMessageFilter::setFilterElement()


bool PCNMessageFilter::isFilterElement(FilterType filter, QChar Element) const {
bool    ok;

  switch (filter) {
  case MSG_TYPES :
    ok = activeMessageTypes.contains(Element);
    break;
  case STATIONS_490 :
    ok = activeStations490.contains(Element);
    break;
  case STATIONS_518 :
    ok = activeStations518.contains(Element);
    break;
  }
  return ok;
} //=========================================== PCNMessageFilter::isFilterElement()


bool PCNMessageFilter::isFilterElement(const QString & Key) const {
bool ok = false;

  switch(Key[0].toLatin1()) {
  case 'T' :
    ok = isFilterElement(MSG_TYPES, Key[1]);
    break;
  case 'N' :
    ok = isFilterElement(STATIONS_490, Key[1]);
    break;
  case 'I' :
    ok = isFilterElement(STATIONS_518, Key[1]);
    break;
  }
  return ok;
} //=========================================== PCNMessageFilter::isFilterElement()


QString PCNMessageFilter::SqlInExpression(FilterType filter) const {
const QString & active = (filter == MSG_TYPES) ? activeMessageTypes :
                         ((filter == STATIONS_490) ? activeStations490 : activeStations518);
QString ret("'°'");   // For a valid SQL expression, we need at least one element even the filter is empty
int     length = active.length();
int     x;

  for (x = 0; x < length; x++) {
    ret += QString(",'") + active[x] + "'";
  }
  return ret;
} //=========================================== PCNMessageFilter::SqlInExpression()
