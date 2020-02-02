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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSortFilterProxyModel>
#include <QLabel>
#include <QTableView>
#include <QProcess>
#include "ui_PCNMainWindow.h"
#include "PCNMessagesTableModel.h"
#include "PCNMessage.h"
#include "PCNOptions.h"


namespace Ui {
class PCNMainWindow;
}

Q_DECLARE_METATYPE(PCNMessage);

class PCNMainWindow : public QMainWindow, public Ui::PCNMainWindow {

    Q_OBJECT

public:

  explicit PCNMainWindow(QWidget *parent = 0);
  ~PCNMainWindow();

protected :

  void openInputFile();
  void loadInputFile(QString FileName);
  virtual void closeEvent(QCloseEvent * event);
  void    ToolButtonsChanged();
  void    onToolButtonAction(int iToolButton, bool checked);

  bool  isDownload;

  bool  isStartup;
  PCNMessagesTableModel model;
  QSortFilterProxyModel ProxyModel;

  QLabel   permanentStatusText;

  QString  ToolButtonAction[MAX_TOOLBUTTONS];
  QToolButton * ToolButtons[MAX_TOOLBUTTONS];

public slots :

  void on_actionExit_activated();
  void on_actionStations_Filter_490_triggered();
  void on_actionStations_Filter_518_triggered();
  void on_actionStation_Names_triggered();
  void on_actionPreferences_triggered();
  void on_actionCustomizeToolbar_triggered();
  void on_actionAbout_triggered();

  // Toolbuttons
  void on_tbtA_clicked(bool checked = false);
  void on_tbtB_clicked(bool checked = false);
  void on_tbtC_clicked(bool checked = false);
  void on_tbtD_clicked(bool checked = false);
  void on_tbtE_clicked(bool checked = false);
  void on_tbt0_clicked(bool checked = false);
  void MsgTypeButton_clicked(bool checked, QChar cType);
  void on_tbtFilter490_clicked(bool = false);
  void on_tbtFilter518_clicked(bool = false);

  void on_DatabaseReadCompleted();
  void on_startStoredMessages();
  void on_endOfStoredMessages();
  void on_startMessage();
  void on_endOfMessage();
  void on_receivedMessage();

  void on_tbt00_clicked(bool checked = false);
  void on_tbt01_clicked(bool checked = false);
  void on_tbt02_clicked(bool checked = false);
  void on_tbt03_clicked(bool checked = false);
  void on_tbt04_clicked(bool checked = false);
  void on_tbt05_clicked(bool checked = false);
  void on_tbt06_clicked(bool checked = false);
  void on_tbt07_clicked(bool checked = false);
  void on_tbt08_clicked(bool checked = false);
  void on_tbt09_clicked(bool checked = false);
  void on_tbt10_clicked(bool checked = false);
  void on_tbt11_clicked(bool checked = false);

  void currentRowChanged(const QModelIndex & current, const QModelIndex & previous);
  void setTableViewColumnWidth();
};

#endif // MAINWINDOW_H
