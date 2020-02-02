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

#include "PCNMessage.h"
// #include <QCryptographicHash>
#include <QByteArray>
#include <QStringList>
#include <QDateTime>
#include <QFontMetrics>


const QString PCNMessage::Digits("*0123456789");
const QString PCNMessage::Letters("*ABCDEFGHIJKLMNOPQRSTUVWZ");

PCNMessage::PCNMessage()
          : isDownload(true),
            MsgReceiveDate(QDateTime::currentDateTime()),
            MsgCode(""),
            MsgInfo(""),
            MsgBody(""),
            MsgCheckCode(""),
            MsgType('@'),
            MsgStation490('@'),
            MsgStation518('@'),
            widthMsgCode(0),
            widthMsgInfo(0) {
}

PCNMessage::PCNMessage(const QStringList lstLines, bool isFromDownload)
          : isDownload(isFromDownload),
            MsgReceiveDate(QDateTime::currentDateTime()),
            MsgCode(""),
            MsgInfo(""),
            MsgBody(""),
            MsgCheckCode(""),
            MsgType('@'),
            MsgStation490('@'),
            MsgStation518('@'),
            widthMsgCode(0),
            widthMsgInfo(0) {
int  iLine = 0;
int  nLines = lstLines.count();
int  nEmptyLines;
int  nInfoLines;

  if (lstLines.count() < 3)
    return;

  MsgReceiveDate = QDateTime::currentDateTime();
  MsgCode = lstLines[iLine].mid(1);

  // Title
  iLine = 1;
  nInfoLines = 0;
  while (iLine < nLines && nInfoLines < 3) {
    if (!lstLines[iLine].isEmpty()) {
      if (!MsgInfo.isEmpty())
        MsgInfo += " - ";
      MsgInfo += lstLines[iLine];
      nInfoLines++;
    }
    iLine++;
  }

  // Body
  iLine = 1;
  nEmptyLines = 0;
  while (iLine < nLines) {
    QString line = lstLines[iLine];
    if (line.isEmpty()) {
      if (nEmptyLines == 1)
        MsgBody += "\n";
      nEmptyLines++;
    } else {
      if (line[0] == 'a' || line[0] == 'b') {
        MsgCheckCode = line;
        break;
      }
      MsgBody += line + "\n";
      nEmptyLines = 0;
    }
    iLine++;
  }
  if (MsgCheckCode.length() > 0) {
    QChar Station = MsgCode[1];
    if (Letters.indexOf(Station) < 0)
      Station = '*';

    if (MsgCheckCode.left(1) == "a") {
      MsgStation490 = '@';
      MsgStation518 = Station;
    }
    if (MsgCheckCode.left(1) == "b") {
      MsgStation490 = Station;
      MsgStation518 = '@';
    }
  }
}

PCNMessage::~PCNMessage() {
  // qDebug("PCNMessage::~PCNMessage() this = %p", this);
}


#ifdef NIX
/**********************************************
 * Date consist of "dddddd UTC mmm" or "dddddd UTC mmm dd"
 * We check only "dddddd UTC mmm" and return count of errors
 */
// static
int PCNMessage::isDate(const QString & Line, int StartIndex) {
int errors = 0;
int x;

  if (Line.length() < StartIndex + 14) {
    errors = 14;
  } else {
    for (x = StartIndex; x <  StartIndex + 6; x++)
      if (Digits.indexOf(Line[x]) < 0)
        errors++;
    if (Line[x] != ' ' && Line[x] != '*')
      errors++;
    x++;
    if (Line[x] != 'U' && Line[x] != '*')
      errors++;
    x++;
    if (Line[x] != 'T' && Line[x] != '*')
      errors++;
    x++;
    if (Line[x] != 'C' && Line[x] != '*')
      errors++;
    x++;
    if (Line[x] != ' ' && Line[x] != '*')
      errors++;
    x++;
    for (; x <  StartIndex + 3; x++)
      if (Letters.indexOf(Line[x]) < 0)
        errors++;
  }
  return errors;
}

/**********************************************
 * Short date - publisher consist of "dddddd publischer"
 * We check "dddddd LLL" (at least 3 letters) and return error count.
 */
// static
int PCNMessage::isShortDatePublisher(const QString & Line) {
int errors = 0;
int x, z;

  if (Line.length() < 10) {
    errors = 10;
  } else {
    for (x = 0; x <  6; x++)
      if (Digits.indexOf(Line[x]) < 0)
        errors++;
    if (Line[x] != ' ' && Line[x] != '*')
      errors++;
    x++;

    for (z = x + 3; x < z; x++)
      if (Letters.indexOf(Line[x]) < 0)
        errors++;
  }
  return errors;
}

/**********************************************
 * Title / Date is "xxxxxx dddddd UTC mmm"
 * At end of line is a date expected, at start any characters, at least 3.
 */
// static
bool PCNMessage::isTitleDate(const QString & Line, QString & msgDateTime, QString & msgTitle) {
int errors = 0;
int minErrorIndex = 0;
int DateErrors[4] = { 0, 0, 0, 0 };
bool ret = false;

  if (Line.length() > 18) {
    DateErrors[0] = isDate(Line, Line.length() - 14);
    DateErrors[1] = isDate(Line, Line.length() - 15);
    DateErrors[2] = isDate(Line, Line.length() - 16);
    DateErrors[3] = isDate(Line, Line.length() - 17);

    errors = DateErrors[0];
    for (int y = 1; y < 3; y++)
      if (errors > DateErrors[y]) {
        errors = DateErrors[y];
        minErrorIndex = y;
      }
    if (errors < 3) {
      msgDateTime = Line.mid(Line.length() - 14 - minErrorIndex);
      msgTitle    = Line.left(Line.length() - 14 - minErrorIndex);
      ret = true;
    }
  }
  return ret;
}

/***********************************************************
 * A publisher starts with letters and not with digits
 */
// static
bool PCNMessage::isPublisher(const QString & Line) {
QString str = Line + "      ";

  return Line[0] != ' ' && !(Line[0] >= '0' && Line[0] <= '9') &&
         Line[1] != ' ' && !(Line[1] >= '0' && Line[1] <= '9') &&
         Line[2] != ' ' && !(Line[2] >= '0' && Line[2] <= '9') &&
         Line[3] != ' ' && !(Line[3] >= '0' && Line[3] <= '9');
}
#endif

bool PCNMessage::fitsToFilter(const PCNMessageFilter & Filter) {

  return (Filter.isFilterElement(PCNMessageFilter::STATIONS_490, MsgStation490) ||
          Filter.isFilterElement(PCNMessageFilter::STATIONS_518, MsgStation518)) &&
          Filter.isFilterElement(PCNMessageFilter::MSG_TYPES, MsgCode[1]) &&
          (Filter.maxDaysOld == 0 || QDateTime::currentDateTime() < MsgReceiveDate.addDays(Filter.maxDaysOld));
}

void PCNMessage::calcTextWidth(const QFontMetrics & fm, int *pWidthCode, int * pWidthInfo) {

  if (!MsgCode.isEmpty() && !(MsgCode.indexOf("*") >= 0))
    widthMsgCode = fm.width(MsgCode);
  *pWidthCode = widthMsgCode;
  if (!MsgInfo.isEmpty())
    widthMsgInfo = fm.width(MsgInfo);
  *pWidthInfo = widthMsgInfo;
}

bool PCNMessage::isValid() {

  return !MsgCode.isEmpty() &&
         !MsgInfo.isEmpty() &&
         !MsgBody.isEmpty() &&
         !MsgCheckCode.isEmpty();
}

bool PCNMessage::operator==(const PCNMessage & msg) const {

  return MsgReceiveDate == msg.MsgReceiveDate &&
         MsgCode        == msg.MsgCode &&
         MsgInfo        == msg.MsgInfo &&
         MsgBody        == msg.MsgBody &&
         MsgCheckCode   == msg.MsgCheckCode;
}
