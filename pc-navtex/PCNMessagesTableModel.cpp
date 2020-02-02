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

#include "PCNMessagesTableModel.h"
#include "PCNOptions.h"
#include "Logger.h"

PCNMessagesTableModel::PCNMessagesTableModel(QObject *parent)
                  : QAbstractTableModel(parent) {
}


int PCNMessagesTableModel::rowCount(const QModelIndex & /*parent*/) const {
  // logger.debug(__PRETTY_FUNCTION__, __LINE__, "lstMessages.count() = %i", lstMessages.count());
  return lstMessages.count();
}

int PCNMessagesTableModel::columnCount(const QModelIndex & /*parent*/) const {
  // logger.debug(__PRETTY_FUNCTION__, __LINE__, "COL_COUNT = %i", COL_COUNT);
  return COL_COUNT;
}

QVariant PCNMessagesTableModel::data(const QModelIndex &index, int role) const {

//  logger.debug(__PRETTY_FUNCTION__, __LINE__, "index.isValid() %s", index.isValid() ? "true" : "false");
  if (!index.isValid())
    return QVariant();

  const PCNMessage & msg = *lstMessages.at(index.row());

  switch (role) {
  case Qt::DisplayRole :
    switch (index.column()) {
    case COL_KEY :
      return msg.msgCode();
    case COL_INFO :
      return msg.msgInfo();
    default :
      return "???";
    }
    break;
  case Qt::SizeHintRole :
    switch (index.column()) {
    case COL_KEY :
      return sizeColKey;
    }
    break;
  case Qt::UserRole :
    return msg.msgCode() + "\n\n" + msg.msgBody();
  }
  return QVariant();
}

QVariant PCNMessagesTableModel::headerData(int section, Qt::Orientation /*orientation*/, int role) const {

//  if (orientation == Qt::Horizontal) {
    switch (role) {
    case Qt::DisplayRole :
      switch (section) {
      case COL_KEY :
        return tr("Msg-Id");
      case COL_INFO :
        return tr("Info");
      }
      break;
/*
    case Qt::SizeHintRole :
      // logger.debug("PCNMessagesTableModel::headerData()", __LINE__, "Qt::SizeHintRole : section = %i", section);
      switch (section) {
      case COL_KEY :
        logger.debug("PCNMessagesTableModel::headerData()", __LINE__, "Qt::SizeHintRole : section = %i   orientation = %i   options.MainWindow_Col1 = %i", section, orientation, options.MainWindow_Col1);
        return QSize(options.MainWindow_Col1, 64);
      case COL_MSG_NO:
        logger.debug("PCNMessagesTableModel::headerData()", __LINE__, "Qt::SizeHintRole : section = %i   orientation = %i   options.MainWindow_Col2 = %i", section, orientation, options.MainWindow_Col2);
        return QSize(options.MainWindow_Col2, 64);
      case COL_DATE :
        logger.debug("PCNMessagesTableModel::headerData()", __LINE__, "Qt::SizeHintRole : section = %i   orientation = %i   options.MainWindow_Col3 = %i", section, orientation, options.MainWindow_Col3);
        return QSize(options.MainWindow_Col3, 64);
      }
      break;
*/
//    }

  }
  return QVariant();
}

bool PCNMessagesTableModel::insertMessage(PCNMessage * Msg) {

  logger.debug(__PRETTY_FUNCTION__, __LINE__, " ");

  beginInsertRows(QModelIndex(), 0, 0);
  lstMessages.insert(0, Msg);
  endInsertRows();
  return true;
}

void PCNMessagesTableModel::newMessageList(QList<PCNMessage*> & lst) {

  logger.debug(__PRETTY_FUNCTION__, __LINE__, " ");

  beginResetModel();
  while (!lstMessages.isEmpty())
    delete lstMessages.takeLast();
  lstMessages.append(lst);
  lst.clear();
  endResetModel();
}

void PCNMessagesTableModel::maxTextWidth(const QFontMetrics & fm, int * pMaxCode, int * pMaxInfo) {
int  maxCode = fm.width(tr("Msg-Id")) + 16; // +16 for sort marker
int  maxInfo = fm.width(tr("Info")) + 16;
int  widthCode = 0;
int  widthInfo = 0;
int  nMessages = lstMessages.count();

  for (int y = 0; y < nMessages; y++) {
    PCNMessage * msg = lstMessages[y];
    msg->calcTextWidth(fm, &widthCode, &widthInfo);
    if (maxCode < widthCode)
      maxCode = widthCode;
    if (maxInfo < widthInfo)
      maxInfo = widthInfo;
  }
  *pMaxCode = maxCode;
  *pMaxInfo = maxInfo;
}

/*
void PCNMessagesTableModel::updateTableView() {

  beginResetModel();
  endResetModel();
}
*/
