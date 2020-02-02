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

#include "PCNDatabase.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QFileInfo>
#include <QDir>
#include <string.h>

#include "Logger.h"

PCNDatabase::PCNDatabase() {
  db = QSqlDatabase::addDatabase("QSQLITE");
} //=========================================== PCNDatabase::PCNDatabase()

PCNDatabase::~PCNDatabase() {
  if (db.isOpen())
    db.close();
} //=========================================== PCNDatabase::~PCNDatabase()

bool PCNDatabase::init(const QString &FileName) {
bool ret = false;
bool isFirstInit = false;

  if (!QFile::exists(FileName)) {
    if (!firstInit(FileName))
      return ret;
    isFirstInit = true;
    ret = true;
  } else {
    if (db.isOpen())
      db.close();
  }
  if (!isFirstInit) {
    db.setDatabaseName(FileName);
    ret = db.open();
  }
  if (!ret)
    qDebug("Error: connection with database fail");
  else
    qDebug("Database: connection ok");
  return ret;
} //=========================================== PCNDatabase::init()


bool PCNDatabase::firstInit(const QString & FileName) {
QFileInfo FileInfo(FileName);
const QString & absolutePath = FileInfo.absolutePath();
QDir  Dir;

  if (!FileInfo.exists()) {
    if (!Dir.exists(absolutePath))
      Dir.mkpath(absolutePath);
    if (!Dir.exists(absolutePath)) // unable to create?
      return false;
  }

  db.setDatabaseName(FileName);
  if (!db.open()) {
    QSqlError Error = db.lastError();
    logger.error("PCNDatabase::firstInit()", __LINE__, "SQL Error: %s", qPrintable(Error.text()));
    return false;
  }
  QSqlQuery query;
  query.exec("CREATE TABLE MESSAGES ("
             "ID              INTEGER, "
             "RECEIVEDATE     VARCHAR(24), "
             "CHANNEL         CHAR(1), "
             "TYPE            CHAR(1), "
             "STATION490      CHAR(1), "
             "STATION518      CHAR(1), "
             "CODE            VARCHAR(32), "
             "INFO            VARCHAR(64), "
             "BODY            VARCHAR(512), "
             "CHECKCODE       VARCHAR(8))");
  QSqlError Error = db.lastError();
  if (Error.type() != QSqlError::NoError) {
    logger.error("PCNDatabase::firstInit()", __LINE__, "SQL Error: %s", qPrintable(Error.text()));
    return false;
  }
  logger.info("PCNDatabase::firstInit()", __LINE__, "New database created. at: %s", qPrintable(FileName));
  return true;
} //=========================================== PCNDatabase::firstInit()


int PCNDatabase::countOfMessages() {
QSqlQuery query;

  query.prepare("SELECT COUNT(*) FROM MESSAGES");
  if (!query.exec()) {
    QSqlError SqlError = query.lastError();
    QString ErrTxt = SqlError.text();
    qDebug("Error: SELECT COUNT(*) FROM MESSAGES\n%s", qPrintable(ErrTxt));
    return -1;
  }

  if (!query.next()) {
    QSqlError SqlError = db.lastError();
    QString ErrTxt = SqlError.text();
    qDebug("Error: SELECT COUNT(*) FROM MESSAGES\n%s", qPrintable(ErrTxt));
    return -1;
  }
  return query.value(0).toInt();
} //=========================================== PCNDatabase::countOfMessages()


const QString PCNDatabase::oldestMessageDate() {
QSqlQuery query;

  query.prepare("SELECT MIN(RECEIVEDATE) FROM MESSAGES");
  if (!query.exec()) {
    QSqlError SqlError = query.lastError();
    QString ErrTxt = SqlError.text();
    qDebug("Error: SELECT MIN(RECEIVEDATE) FROM MESSAGES\n%s", qPrintable(ErrTxt));
    return "-1";
  }

  if (!query.next()) {
    QSqlError SqlError = db.lastError();
    QString ErrTxt = SqlError.text();
    qDebug("Error: SELECT MIN(RECEIVEDATE) FROM MESSAGES\n%s", qPrintable(ErrTxt));
    return "-1";
  }
  QString strDateTime = query.value(0).toString();
  if (strDateTime.isEmpty())
    return strDateTime;
  QDateTime MinMessageDate = StringToDateTime(query.value(0).toString());
  return MinMessageDate.toString(tr("yyyy/dd/MM HH:mm"));
} //=========================================== PCNDatabase::oldestMessageDate()


const QString PCNDatabase::newestMessageDate() {
QSqlQuery query;

  query.prepare("SELECT MAX(RECEIVEDATE) FROM MESSAGES");
  if (!query.exec()) {
    QSqlError SqlError = query.lastError();
    QString ErrTxt = SqlError.text();
    qDebug("Error: SELECT MIN(RECEIVEDATE) FROM MESSAGES\n%s", qPrintable(ErrTxt));
    return "-1";
  }

  if (!query.next()) {
    QSqlError SqlError = db.lastError();
    QString ErrTxt = SqlError.text();
    qDebug("Error: SELECT MIN(RECEIVEDATE) FROM MESSAGES\n%s", qPrintable(ErrTxt));
    return "-1";
  }
  QString strDateTime = query.value(0).toString();
  if (strDateTime.isEmpty())
    return strDateTime;
  QDateTime MaxMessageDate = StringToDateTime(query.value(0).toString());
  return MaxMessageDate.toString(tr("yyyy/dd/MM HH:mm"));
} //=========================================== PCNDatabase::newestMessageDate()


/**************************************************************
 * existsMessage(PCNMessage * Msg, QString & Id, QString & ReceiveDate)
 *
 * Looks if Msg exists in database
 * Parameter: Msg IN        message to looking for
 *            Id Out        message-ID - if found
 *            ReceiveDate   message-ReceiveDate - if found
 * Return values:
 *     -1    SQL-error
 *      0    not found
 *      1    equivalent message found
 *
 *************************************************************/

int PCNDatabase::existsMessage(PCNMessage * Msg, QString & Id, QString & ReceiveDate) {
QString Channel = Msg->msgCheckCode().left(1);
QString Code = Msg->msgCode();
QString Info = Msg->msgInfo();
QString Body = Msg->msgBody();

  QSqlQuery query(db);
  query.prepare("SELECT ID, RECEIVEDATE "
                "  FROM MESSAGES "
                " WHERE CHANNEL  = :channel "
                "   AND CODE     = :code "
                "   AND BODY     = :body "
                "ORDER BY RECEIVEDATE DESC");
  query.bindValue(":channel", QVariant(Channel));
  query.bindValue(":code", QVariant(Code));
  query.bindValue(":body", QVariant(Body));

  if (!query.exec()) {
    QSqlError SqlError = query.lastError();
    logger.error("PCNDatabase::existsMessage()", __LINE__, "SQL Error: %s", qPrintable(SqlError.text()));
    logger.error("PCNDatabase::existsMessage()", __LINE__, "SQL Error: %s", qPrintable(SqlError.databaseText()));
    logger.error("PCNDatabase::existsMessage()", __LINE__, "SQL Error: %s", qPrintable(SqlError.driverText()));
    return -1;
  }

  if (query.next()) {
    Id = query.value("ID").toString();
    ReceiveDate = query.value("RECEIVEDATE").toString();
    return 1;
  }
  return 0;
} //=========================================== PCNDatabase::existsMessage()


/**************************************************************
*  insertMessage(PCNMessage * Msg)
*
*  adds Msg to database. No check for existing message ist performed.
*
*  Parameter:
*    Msg        the message to add
*
*  Return values:
*   -1     SQL-error
*   >0     Id for new message
*
*************************************************************/

int PCNDatabase::insertMessage(PCNMessage * Msg) {
int ret = -1;
QSqlQuery query;
int     Rows = -1;
int     Id = -1;
QString ReceiveDate = DateTimeToString(Msg->msgReceiveDate());
QString Code = Msg->msgCode();
QString Channel = Msg->msgCheckCode().left(1);
QString Station490 = Channel == "a" ? "@" : Code.left(1);
QString Station518 = Channel == "b" ? "@" : Code.left(1);
QString Type = Code.mid(1, 1);
QString Info = Msg->msgInfo();
QString Body = Msg->msgBody();
QString CheckCode = Msg->msgCheckCode();

  query.prepare("SELECT COUNT(*) AS 'ROWS' FROM MESSAGES");
  if (!query.exec()) {
    QSqlError SqlError = query.lastError();
    logger.error("PCNDatabase::insertMessage()", __LINE__, "SQL Error: %s", qPrintable(SqlError.text()));
    return ret;
  }
  if (query.next())
    Rows = query.value("ROWS").toInt();
  if (Rows < 0)
    return ret;
  if (Rows == 0) {
    Id = 1;
  } else {
    query.prepare("SELECT MAX(ID) AS 'MAX_ID' FROM MESSAGES");
    if (!query.exec()) {
      QSqlError SqlError = query.lastError();
      logger.error("PCNDatabase::insertMessage()", __LINE__, "SQL Error: %s", qPrintable(SqlError.text()));
      return ret;
    }

    if (query.next())
      Id = query.value("MAX_ID").toInt();

    if (Id < 0)
      return ret;
    Id++;
  }

  query.prepare("INSERT INTO MESSAGES ( ID,  RECEIVEDATE,  CODE,  CHANNEL,  TYPE,  STATION490,  STATION518,  INFO,  BODY,  CHECKCODE) "
                             " VALUES (:id, :receivedate, :code, :channel, :type, :station490, :station518, :info, :body, :checkcode)");
  query.bindValue(":id", Id);
  query.bindValue(":receivedate", ReceiveDate);
  query.bindValue(":code", Code);
  query.bindValue(":channel", Channel);
  query.bindValue(":station490", Station490);
  query.bindValue(":station518", Station518);
  query.bindValue(":type", Type);
  query.bindValue(":info", Info);
  query.bindValue(":body", Body);
  query.bindValue(":checkcode", CheckCode);

  if (query.exec()) {
    db.commit();
    ret = Id;
  } else {
    QSqlError SqlError = query.lastError();
    // QString ErrTxt = SqlError.text();
    logger.error("PCNDatabase::insertMessage()", __LINE__, "SQL Error: %s\n",
                 "     INSERT INTO MESSAGES (ID, RECEIVEDATE, CODE, TYPE, STATION490, STATION518, INFO, BODY, CHECKCODE)\n"
                 "     VALUES (%i, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')\n",
           qPrintable(SqlError.text()),
           Id,
           qPrintable(ReceiveDate),
           qPrintable(Code),
           qPrintable(Type),
           qPrintable(Station490),
           qPrintable(Station518),
           qPrintable(Info),
           qPrintable(Body),
           qPrintable(CheckCode));
  }
  return ret;
} //=========================================== PCNDatabase::insertMessage()


/**************************************************************
* updateReceiveDate(const QString & Id, const QString & ReceiveDate)
*
* updates receivedate in message with Id
*
* Parameter:
*   Id            message to update
*   ReceiveDate  Receivedate to set
*
* Return :
*   -1            SQL-error
*    1            Ok
**************************************************************/

int PCNDatabase::updateReceiveDate(const QString & Id, const QString & ReceiveDate) {

  logger.debug("PCNDatabase::updateReceiveDate()", __LINE__, "Id = %s   ReceiveDate = %s.\n",
                                                             qPrintable(Id),
                                                             qPrintable(ReceiveDate));
  logger.error("PCNDatabase::updateReceiveDate()", __LINE__, "Not implemented.\n");

  return 0;
}//=========================================== PCNDatabase::updateReceiveDate()


/**************************************************************
 * receivedMessage(PCNMessage * Msg, bool isDownload)
 *
 * Do anything with a fresh received message
 *
 * Parameter:
 *   Msg          the message
 *   isDownload   type of message income
 *        true    message is a donwload of all messages stored in device
 *        false   fresh received message
 *
 * Return values:
 *   -1     SQL-error
 *    1     Ok
 *
 * if isDownload     : message is stored only if not already exist
 * if not isDownload : if message already exists, receivedate is updated
 *                     otherwise message is stored
 * == If a message comes repeated, it's receivedate is updated only ==
 * In case of Download, we assume its the old one.
 *************************************************************/

int PCNDatabase::receivedMessage(PCNMessage * Msg, bool isDownload) {
int      ret;
QString  Id("");
QString  ReceiveDate("");
QString  msgReceiveDate = DateTimeToString(Msg->msgReceiveDate());

  ret = existsMessage(Msg, Id, ReceiveDate);
  if (ret == -1) // SQL-error
    return ret;
  if (ret == 0)  // not found
    return insertMessage(Msg);
  // message exits
  if (isDownload) {
    ret = 1;
  } else {
    if (ReceiveDate < msgReceiveDate)
      ret = updateReceiveDate(Id, msgReceiveDate);
    else
      ret = 1;
  }
  return ret;
} //=========================================== PCNDatabase::insertMessage()


void PCNDatabase::queryMessages(QList<PCNMessage*> & lstMessages, const PCNMessageFilter & Filter) {
QSqlQuery query;
QString   minReceiveDate;
int       maxDaysOld = Filter.maxDaysOld ;
int       nMessages = 0;
int       maxMessages = Filter.maxSelectCount;

  Statement = "SELECT CODE, INFO, BODY, CHECKCODE FROM MESSAGES "
              " WHERE TYPE IN (" + Filter.SqlInExpression(PCNMessageFilter::MSG_TYPES) + ") "
              "   AND (STATION490 IN (" + Filter.SqlInExpression(PCNMessageFilter::STATIONS_490) + ") OR "
              "        STATION518 IN (" + Filter.SqlInExpression(PCNMessageFilter::STATIONS_518) + ")) "
              "   AND RECEIVEDATE >= :minreceivedate "
              "ORDER BY RECEIVEDATE DESC";

  logger.debug("PCNDatabase::queryMessages()", __LINE__, "Statement: %s\n", qPrintable(Statement));
  if (Filter.maxDaysOld <= 0)
    maxDaysOld = 20000;
  QDateTime dt = QDateTime::currentDateTime().addDays(0 - maxDaysOld);
  minReceiveDate = DateTimeToString(dt);

  query.prepare(Statement);
  query.bindValue(":minreceivedate", minReceiveDate);

  if (!query.exec()) {
    QSqlError SqlError = query.lastError();
    logger.error("PCNDatabase::queryMessages()", __LINE__, "SQL Error: %s\nStatement: %s\n", qPrintable(SqlError.text()), qPrintable(Statement));
    return;
  }

  if (maxMessages <= 0)
    maxMessages = 1000000;
  while (query.next()) {
    PCNMessage * Msg = new PCNMessage();
    Msg->setMsgCode(query.value("CODE").toString());
    Msg->setMsgInfo(query.value("INFO").toString());
    Msg->setMsgBody(query.value("BODY").toString());
    Msg->setMsgCheckCode(query.value("CHECKCODE").toString());
    lstMessages.append(Msg);

    nMessages++;
    if (nMessages >= maxMessages)
      break;
  }
  emit newMessages();
} //=========================================== PCNDatabase::queryMessages()

// static
QString PCNDatabase::DateTimeToString(const QDateTime & dt) {
char s[28];
const QDate & d = dt.date();
const QTime & t = dt.time();

  sprintf(s, "%04i-%02i-%02i-%02i-%02i-%02i-%03i", d.year(), d.month(), d.day(), t.hour(), t.minute(), t.second(), t.msec());
  return QString(s);
} //=========================================== PCNDatabase::DateTimeToString()

// static
QDateTime PCNDatabase::StringToDateTime(const QString & dtString) {
QStringList lStr = dtString.split('-');

  if (lStr.count() != 7)
    return QDateTime::currentDateTime();

  QDate d(lStr[0].toInt(), lStr[1].toInt(), lStr[2].toInt());
  QTime t(lStr[3].toInt(), lStr[4].toInt(), lStr[5].toInt(), lStr[6].toInt());

  return QDateTime(d, t);
} //=========================================== PCNDatabase::StringToDateTime()
