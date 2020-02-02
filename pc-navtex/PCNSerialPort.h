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

#ifndef PCNSERIALPORT_H
#define PCNSERIALPORT_H

#define BUFFER_SIZE    256

#define JOB_NULL              0
#define JOB_STOP              1
#define JOB_READYREAD         2
#define JOB_STORED_MESSAGES   4
#define JOB_VERSION           8
#define JOB_DATE             16
#define JOB_CHANNEL490       32
#define JOB_CHANNEL518       64

#include <QSerialPort>


#include <QSerialPort>
#include <QString>
#include <QTimer>

#include "PCNMessage.h"

Q_DECLARE_METATYPE(QSerialPort::SerialPortError);



class PCNSerialPort : public QObject {

    Q_OBJECT

public:
  explicit PCNSerialPort(QObject *parent = nullptr);

signals:

  void receivedMessage();      // emitted only when isStoredMessages == false
                               // the new message is complete and stored in pNewMessages list
                               // connected in MainWindow, INSERT in database, insert in TableView - if not downloaded
  void startStoredMessages();  // emitted only when isStoredMessages == true
                               // the first letter '>' of the first message is received
                               // connected in MainWindow, switch on the icon
  void endOfStoredMessages();  // emitted when the line "end" is read or time out
                               // connected in MainWindow, switch off the icon, execute database query
  void startMessage();         // emitted only when isStoredMessages == false
                               // the first letter '>' of a message is received
                               // connected in MainWindow, switch on the icon
  void endOfMessage();         // emitted only when isStoredMessages == false
                               // the last letter of a message is received or time out
                               // connected in MainWindow, switch on the icon
  void receivedVersion(QString Version); // emitted when (isMessage == true and sufficient characters read) or time out

  void errorTimeOut(QString ErrorInfo);  // informs GUI about time out errors

public slots:

  void readData();
  bool writeData(const char *data);
  void handleError(QSerialPort::SerialPortError error);
  void TimeOut();

protected:

  const char * ErrorText(QSerialPort::SerialPortError Error);

public :
  bool openSerialPort(const QString & PortName);
  void closeSerialPort();
  bool queryVersion();
  bool queryStoredMessages();

protected:

  QSerialPort  SerialPort;
  QByteArray   LineBuffer;
  QStringList  lstLineBuffer;
  QTimer       TimeOutClock;
  int          Job;
  bool         isMessage;
  bool         isStoredMessages;
  bool         isVersion;
  bool         isVersionWanted;

};

extern PCNSerialPort * pSerialPort;

#endif // PCNSERIALPORT_H
