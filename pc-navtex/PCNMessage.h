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

#ifndef PCNMESSAGE_H
#define PCNMESSAGE_H

#include <QString>
#include <QDateTime>
#include <QFontMetrics>

#include "PCNMessageFilter.h"

class PCNMessage
{
public:
  PCNMessage();
  PCNMessage(const QStringList lstLines, bool isFromDownload);
  ~PCNMessage();

protected:

  bool      isDownload;      // downloaded from device -- needed for timestamp generation
                             // false:  message comes fresh from receiver:
                             //         Check for existing equivalent in database.
                             //         if exists MsgReceiveDate is updated
                             //         else message is added
                             //         A equivalent message ist designed by MsgCode, MsgDateTime, MsgTitle, MsgBody.
                             //         MsgHash, MsgReceiveDate, MsgCheckCode may differ.
                             // true:   message comes from device buffer or file import.
                             //         Only added to the database if the hash does not already exists.
  QDateTime MsgReceiveDate;  // date/time of message income. Current date/time us used.
                             // May updated for repeadly broadcasted messages.
  QString   MsgCode;         // PA66
  QString   MsgInfo;         // The first 3 lines
  QString   MsgBody;         // The whole message
  QString   MsgCheckCode;    // a9

  QChar     MsgType;
  QChar     MsgStation490;
  QChar     MsgStation518;

  int       widthMsgCode;    // Width of text representation in pixel
  int       widthMsgInfo;    // Width of text representation in pixel
                             // It's used for calculation width for TableView's column

  static const QString Digits;
  static const QString Letters;

public:

  bool  fitsToFilter(const PCNMessageFilter & Filter);
  bool  isDownloaded() const { return isDownload; }
  const QDateTime msgReceiveDate() const { return MsgReceiveDate; }
  const QString   msgCode() const { return MsgCode; }
  const QString   msgInfo() const { return MsgInfo; }
  const QString   msgBody() const { return MsgBody; }
  const QString   msgCheckCode() const { return MsgCheckCode; }

  // void setMsgHash(const QString & msgHash) { MsgHash = msgHash; }
  void setDownloaded(bool isFromDownload) { isDownload = isFromDownload; }
  void setMsgReceiveDate(const QDateTime & msgReceiveDate) { MsgReceiveDate = msgReceiveDate; }
  void setMsgCode(const QString & msgCode) { MsgCode = msgCode; }
  void setMsgInfo(const QString & msgInfo) { MsgInfo = msgInfo; }
  void setMsgBody(const QString & msgBody) { MsgBody = msgBody; }
  void setMsgCheckCode(const QString & msgCheckCode) { MsgCheckCode = msgCheckCode; }
  void calcTextWidth(const QFontMetrics & fm, int * pWidthCode, int * pWidthInfo);

  bool isValid();
  bool operator==(const PCNMessage & msg) const;

};

extern QList<PCNMessage*> * pNewMessages;

#endif // PCNMESSAGE_H
