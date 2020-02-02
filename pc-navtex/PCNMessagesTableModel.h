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

#ifndef MESSAGESTABLEMODEL_H
#define MESSAGESTABLEMODEL_H

#include <QAbstractTableModel>
#include <QSize>

#include "PCNMessage.h"

#define  COL_KEY         0
#define  COL_INFO        1

#define  COL_COUNT       2


class PCNMessagesTableModel : public QAbstractTableModel {

  Q_OBJECT

public:

  explicit PCNMessagesTableModel(QObject *parent = 0);

  virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
  virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
  virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

  bool    insertMessage(PCNMessage * Msg);
  void    newMessageList(QList<PCNMessage*> & lst);
  void    maxTextWidth(const QFontMetrics & fm, int * pMaxCode, int * pMaxInfo);
//  void    updateTableView();

  QList<PCNMessage*> lstMessages;
  QSize   sizeColKey;

signals:

public slots:

};

#endif // MESSAGESTABLEMODEL_H
