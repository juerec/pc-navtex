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

#ifndef PCNDATABASE_H
#define PCNDATABASE_H

#include <QThread>
#include <QSqlDatabase>
#include <QDateTime>
#include <QString>


#include "PCNMessage.h"

//class PCNDatabase : public QThread {
class PCNDatabase : public QObject {

  Q_OBJECT

public:
    PCNDatabase();
    ~PCNDatabase();

    bool init(const QString & FileName);
    bool firstInit(const QString & FileName);

    int           countOfMessages();
    const QString oldestMessageDate();
    const QString newestMessageDate();

    int           existsMessage(PCNMessage * Msg, QString & Id, QString & ReceiveDate);
    int           receivedMessage(PCNMessage * Msg, bool isDownload);
    int           updateReceiveDate(const QString & Id, const QString & ReceiveDate);
    int           insertMessage(PCNMessage * Msg);
    void          queryMessages(QList<PCNMessage *> &lstMessages, const PCNMessageFilter &Filter);

protected:

  QSqlDatabase db;
  QString      Statement;

public:
  static  QString    DateTimeToString(const QDateTime & dt);
  static  QDateTime  StringToDateTime(const QString & dtString);

signals:
  void MessageRead(PCNMessage);
  void newMessages();

protected slots:

};

extern PCNDatabase * pDatabase;

#endif // PCNDATABASE_H
