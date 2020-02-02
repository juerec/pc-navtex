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

#include "PCNSerialPort.h"

#include "Logger.h"
#include <QTimer>

PCNSerialPort::PCNSerialPort(QObject *parent)
             : QObject(parent) {

  Job              = JOB_NULL;
  isMessage        = false;
  isStoredMessages = false;
  isVersion        = false;
  isVersionWanted  = false;
  qRegisterMetaType<QSerialPort::SerialPortError>();
  TimeOutClock.setInterval(500);
  TimeOutClock.setTimerType(Qt::CoarseTimer);
  TimeOutClock.setSingleShot(true);

  connect(&SerialPort, SIGNAL(readyRead()), this, SLOT(readData()));
  connect(&SerialPort, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(handleError(QSerialPort::SerialPortError)));
  connect(&TimeOutClock, SIGNAL(timeout()), this, SLOT(TimeOut()));
} //=========================================== PCNSerialPort::PCNSerialPort()


void PCNSerialPort::readData() {
int           BytesAvailable;
char          ReadBuffer[BUFFER_SIZE];
int           ReadBufferPos;
int           BytesToRead;
int           BytesRead;
int           SumBytesRead;

  TimeOutClock.stop();
  BytesAvailable = (int)SerialPort.bytesAvailable();
  logger.debug(__PRETTY_FUNCTION__, __LINE__, "BytesAvailable = %i", BytesAvailable);
  if (BytesAvailable < 1)
    return;
  SumBytesRead = 0;
  while (SumBytesRead < BytesAvailable) {
    BytesToRead = BytesAvailable - SumBytesRead;
    if (BytesToRead > BUFFER_SIZE)
      BytesToRead = BUFFER_SIZE;
    BytesRead = (int)SerialPort.read(ReadBuffer, BytesToRead);
    if (BytesRead < 0) {
      logger.error(__PRETTY_FUNCTION__, __LINE__, "BytesRead = %i", BytesRead);
      break;
    }
    SumBytesRead += BytesRead;

    ReadBufferPos = 0;
    while (ReadBufferPos < BytesRead) {
      // end of a line reached?
      if (ReadBuffer[ReadBufferPos] == '\n') {
        ReadBufferPos++;
        QString Line(LineBuffer);
        LineBuffer.clear();

        logger.debug(__PRETTY_FUNCTION__, __LINE__, "isMessage = %i   Line = %s", isMessage, qPrintable(Line));

        // new message start
        if (Line[0] == '>') {
          isMessage = true;
          lstLineBuffer.clear();
          lstLineBuffer.append(Line);
          // for setting GUI indicator active
          if (!isStoredMessages)
            emit startMessage();
          continue;
        }

        // message completed
        if (Line[0] == '<') {
          // for setting GUI indicator passive
          emit endOfMessage();
          logger.debug(__PRETTY_FUNCTION__, __LINE__, "emit endOfMessage()  isMessage = %i", isMessage);
          if (isMessage) {
            PCNMessage * Msg = new PCNMessage(lstLineBuffer, isStoredMessages);
            if (Msg->isValid()) {
              pNewMessages->append(Msg);
              logger.debug(__PRETTY_FUNCTION__, __LINE__, "Message received: %s\n", qPrintable(Msg->msgCode()));
              // processed in MainWindow::on_pNewMessages() for storing and display new message
              if (!isStoredMessages)
                // in receiving all stored messages mode, the signal is emitted at finish, once for all messages, not for every single message.
                // that ist for saving cpu utilisation while data transmission
                emit receivedMessage();
            } else {
              logger.info(__PRETTY_FUNCTION__, __LINE__, "Invalid message: \n    %s\n    %s\n    %s",
                          (Msg->msgCode().isEmpty() ? "?" : qPrintable(Msg->msgCode())),
                          (Msg->msgInfo().isEmpty() ? "?" : qPrintable(Msg->msgInfo())),
                          (Msg->msgBody().isEmpty() ? "?" : qPrintable(Msg->msgBody())));
              delete Msg;
            }
          }
          isMessage = false;
          lstLineBuffer.clear();
          continue;
        }

        // end of stored messages
        if (Line.startsWith("end")) {
          // this emit informs GUI to set the indicator symbol unchecked
          // at now, all stored messages read are still in pNewMessages
          // they must stored in database and shown
          TimeOutClock.stop();
          emit endOfStoredMessages();
          isMessage = false;
          isStoredMessages = false;
          lstLineBuffer.clear();
          emit receivedMessage();
          continue;
        }
        // answere to version request
        if (Line.startsWith("Version")) {
          TimeOutClock.stop();
          isVersion = false;
          emit receivedVersion(Line);
          continue;
        }

        // answere to setting channel A
        //##  Status $A

        // answere to setting channel B
        //##  Status $B

        // answere to setting date/time
        //##  Statur Datum/Urzeit

        if (isMessage)
          lstLineBuffer.append(Line);

        continue;
      }
      // inside a line
      LineBuffer.append(ReadBuffer[ReadBufferPos]);
      ReadBufferPos++;
    }
  } // while (SumBytesRead < BytesAvailable)
  if (isMessage | isStoredMessages | isVersion)
     TimeOutClock.start();
} //=========================================== PCNSerialPort::readData()


bool PCNSerialPort::writeData(const char * data) {
bool ret;
qint64  len = (qint64)strlen(data);

  ret = (SerialPort.write(data, len) == len) &&
         SerialPort.waitForBytesWritten(1000);
  if (ret)
    // When a request is written to the device, always a answere is expected
    TimeOutClock.start();
  else
    logger.error(__PRETTY_FUNCTION__, __LINE__, "Error writing to serial port", ErrorText(SerialPort.error()));
  return ret;
} //=========================================== PCNSerialPort::writeData()


void PCNSerialPort::handleError(QSerialPort::SerialPortError error) {

  if (error == QSerialPort::NoError)
    logger.debug(__PRETTY_FUNCTION__, __LINE__, "SerialPortError = %s", ErrorText(error));
  else
    logger.error(__PRETTY_FUNCTION__, __LINE__, "SerialPortError = %s", ErrorText(error));
} //=========================================== PCNSerialPort::handleError()


const char * PCNSerialPort::ErrorText(QSerialPort::SerialPortError Error) {
const char *  ErrTxt = "?";

  switch (Error) {
  case QSerialPort::NoError :
    ErrTxt = "NoError";
    break;
  case QSerialPort::DeviceNotFoundError :
    ErrTxt = "DeviceNotFoundError";
    break;
  case QSerialPort::PermissionError :
    ErrTxt = "PermissionError";
    break;
  case QSerialPort::OpenError :
    ErrTxt = "OpenError";
    break;
  case QSerialPort::NotOpenError :
    ErrTxt = "NotOpenError";
    break;
  case QSerialPort::ParityError :
    ErrTxt = "ParityError";
    break;
  case QSerialPort::FramingError :
    ErrTxt = "FramingError";
    break;
  case QSerialPort::BreakConditionError :
    ErrTxt = "BreakConditionError";
    break;
  case QSerialPort::WriteError :
    ErrTxt = "WriteError";
    break;
  case QSerialPort::ReadError :
    ErrTxt = "ReadError";
    break;
  case QSerialPort::ResourceError :
    ErrTxt = "ResourceError";
    break;
  case QSerialPort::UnsupportedOperationError :
    ErrTxt = "UnsupportedOperationError";
    break;
  case QSerialPort::TimeoutError :
    ErrTxt = "TimeoutError";
    break;
  case QSerialPort::UnknownError :
    ErrTxt = "UnknownError";
      break;
  }
  return ErrTxt;
} //=========================================== PCNSerialPort::ErrorText()


void PCNSerialPort::TimeOut() {
const char * pErrorText = NULL;

  if (isStoredMessages) {
    emit endOfStoredMessages();
    pErrorText = "Time out while reading stored messages.";
    logger.info(__PRETTY_FUNCTION__, __LINE__, pErrorText);
  } else {
    if (isMessage) {
      emit endOfMessage();
      pErrorText = "Time out while reading stored messages.";
      logger.info(__PRETTY_FUNCTION__, __LINE__, pErrorText);
    }
  }
  if (isVersion) {
    emit receivedVersion("");
    pErrorText = "Time out while reading version info.";
    logger.info(__PRETTY_FUNCTION__, __LINE__, pErrorText);
  }
  if (pErrorText != NULL) {
    emit errorTimeOut(tr(pErrorText));
    isStoredMessages = false;
    isMessage = false;
    isVersion = false;
    LineBuffer.clear();
    lstLineBuffer.clear();
  }
} //=========================================== PCNSerialPort::TimeOut()


bool PCNSerialPort::openSerialPort(const QString & PortName) {
bool ret = false;

  if (SerialPort.isOpen())
    SerialPort.close();
  SerialPort.setPortName(PortName);
  SerialPort.setBaudRate(QSerialPort::Baud38400);
  SerialPort.setDataBits(QSerialPort::Data8);
  SerialPort.setParity(QSerialPort::NoParity);
  SerialPort.setStopBits(QSerialPort::OneStop);
  ret = SerialPort.open(QIODevice::ReadWrite);
  if (!ret)
    logger.error(__PRETTY_FUNCTION__, __LINE__, "SerialPort::open(): %i", SerialPort.error());
  return ret;
} //=========================================== PCNSerialPort::openSerialPort()


void PCNSerialPort::closeSerialPort() {

  if (SerialPort.isOpen())
    SerialPort.close();
} //=========================================== PCNSerialPort::closeSerialPort()


bool PCNSerialPort::queryVersion() {
bool ret = !(isMessage || isStoredMessages || isVersion) && writeData("$V\r\n");

  isVersion = ret;
  return ret;
} //=========================================== PCNSerialPort::queryVersion()


bool PCNSerialPort::queryStoredMessages() {
bool ret = !(isMessage || isStoredMessages || isVersion) && writeData("$S\r\n");

  isStoredMessages = ret;
  return ret;
} //=========================================== PCNSerialPort::getStoredMessages()
