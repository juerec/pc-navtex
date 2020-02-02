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

#ifndef PCNMESSAGEFILTER_H
#define PCNMESSAGEFILTER_H

#include <QString>
#include <QSettings>

class PCNMessageFilter {

public:

  enum FilterType {
    MSG_TYPES,
    STATIONS_490,
    STATIONS_518
  } ;

  PCNMessageFilter();

  void readFromSettings(QSettings & settings);
  void writeToSettings(QSettings & settings);

protected :

  void addElement(QString & active, QChar Element);
  void remElement(QString & active, QChar Element);

public :

  void setFilterElement(FilterType filter, QChar Element, bool check);
  void setFilterElement(const QString & Key, bool check);
  bool isFilterElement(FilterType filter, QChar Element) const;
  bool isFilterElement(const QString & Key) const;
  QString SqlInExpression(FilterType filter) const;

protected :

  QString activeStations490;
  QString activeStations518;
  QString activeMessageTypes;

public :
  int     maxDaysOld;
  int     maxSelectCount;


  static const char * AllStations;
  static const char * AllTypes;

};

#endif // PCNMESSAGEFILTER_H
