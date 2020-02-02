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

#ifndef PCNOPTIONS_H
#define PCNOPTIONS_H

#include <QtCore>


#include "PCNStation.h"
#include "PCNMessageFilter.h"

#define MAX_TOOLBUTTONS   12


class PCNOptions {

public:

  PCNOptions();

  void readOptions(int argc, char * argv[]);
  void save();

  void writeWindowOptions();

  void readStations(int Type, QList<PCNStation> & lstStations, QSettings & settings);
  void writeStations(int Type, QList<PCNStation> & lstStations, QSettings & settings);
  const QString DatabaseName(); 
  const QString StationName(const QString & Key);
  int   maxMessageAgeToQuery();
  int   maxMessageCountToQuery();
  void  setMaxMessageAgeToQuery(int maxMessageAge);
  void  setMaxMessageCountToQuery(int maxMessageCount);


  QString     PathData;

  QPoint      MainWindow_xy;
  QSize       MainWindow_wh;
  QString     MainWindow_format;
  int         MainWindow_Col1;
  int         MainWindow_Col2;
  int         MainWindow_FontSize;
  QString     Locale;

  PCNMessageFilter  MessageFilter;

  QList<PCNStation> lstStations490;
  QList<PCNStation> lstStations518;
  QStringList       lstMsgTypeFilter;
  QStringList       lstToolButtons;

  QString     DeviceName;
  int         deleteMessagesOlderThan;
};


extern PCNOptions * pOptions;

#endif // PCNOPTIONS_H
