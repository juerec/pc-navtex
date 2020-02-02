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

#include <QFileDialog>
#include <QString>
#include <QMessageBox>

#include "PCNMainWindow.h"
#include "ui_PCNMainWindow.h"
#include "Logger.h"
#include "PCNDatabase.h"
#include "PCNSerialPort.h"
#include "PCNStationFilterDialog.h"
#include "PCNStationNamesDialog.h"
#include "PCNPreferences.h"
#include "PCNToolbarCustomizer.h"


PCNMainWindow::PCNMainWindow(QWidget *parent) :
    QMainWindow(parent),
    Ui::PCNMainWindow() {

  setupUi(this);

  qRegisterMetaType<PCNMessage>();

  isDownload = false;
  isStartup  = true;

  QFont Font = font();
  Font.setPointSize(pOptions->MainWindow_FontSize);
  setFont(Font);

  tbtA->setChecked(pOptions->MessageFilter.isFilterElement("TA"));          // activeMessageTypes.indexOf('A') >= 0);
  tbtA->setToolTip(tr("Navigational warnings"));
  tbtB->setChecked(pOptions->MessageFilter.isFilterElement("TB"));          // activeMessageTypes.indexOf('B') >= 0);
  tbtB->setToolTip(tr("Meteorological warnings"));
  tbtC->setChecked(pOptions->MessageFilter.isFilterElement("TC"));          // activeMessageTypes.indexOf('C') >= 0);
  tbtC->setToolTip(tr("Ice reports"));
  tbtD->setChecked(pOptions->MessageFilter.isFilterElement("TD"));          // activeMessageTypes.indexOf('D') >= 0);
  tbtD->setToolTip(tr("Search & rescue information, pirate warnings"));
  tbtE->setChecked(pOptions->MessageFilter.isFilterElement("TE"));          // activeMessageTypes.indexOf('E') >= 0);
  tbtE->setToolTip(tr("Meteorological forecasts"));
  tbt0->setChecked(pOptions->MessageFilter.isFilterElement("TZ"));          // activeMessageTypes.indexOf('*') >= 0);
  tbt0->setToolTip(tr("All other") + " - F,G,H,I,J,K,L,T,V,W,X,Y,Z,*");

  tbvMessages->verticalHeader()->hide();
  // The model must be set before setting column width
  tbvMessages->setModel(&model);
  tbvMessages->setSelectionMode(QAbstractItemView::SingleSelection);
  tbvMessages->setSelectionBehavior(QAbstractItemView::SelectRows);
  tbvMessages->setColumnWidth(0, pOptions->MainWindow_Col1);
  tbvMessages->setColumnWidth(1, pOptions->MainWindow_Col2);

  statusBar()->insertPermanentWidget(0, &permanentStatusText, 0);

  connect(tbvMessages->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)),
          this, SLOT(currentRowChanged(const QModelIndex &, const QModelIndex &)));
  connect(pDatabase, SIGNAL(newMessages()), this, SLOT(on_DatabaseReadCompleted()));
  connect(pSerialPort, SIGNAL(startStoredMessages()), this, SLOT(on_startStoredMessages()));
  connect(pSerialPort, SIGNAL(endOfStoredMessages()), this, SLOT(on_endOfStoredMessages()));
  connect(pSerialPort, SIGNAL(startMessage()), this, SLOT(on_startMessage()));
  connect(pSerialPort, SIGNAL(endOfMessage()), this, SLOT(on_endOfMessage()));
  connect(pSerialPort, SIGNAL(receivedMessage()), this, SLOT(on_receivedMessage()));

  QList<PCNMessage*> lstMsg;
  pDatabase->queryMessages(lstMsg, pOptions->MessageFilter);
  model.newMessageList(lstMsg);
  setTableViewColumnWidth();
  pSerialPort->queryStoredMessages();

  ToolButtons[ 0] = tbt00;
  ToolButtons[ 1] = tbt01;
  ToolButtons[ 2] = tbt02;
  ToolButtons[ 3] = tbt03;
  ToolButtons[ 4] = tbt04;
  ToolButtons[ 5] = tbt05;
  ToolButtons[ 6] = tbt06;
  ToolButtons[ 7] = tbt07;
  ToolButtons[ 8] = tbt08;
  ToolButtons[ 9] = tbt09;
  ToolButtons[10] = tbt10;
  ToolButtons[11] = tbt11;
  ToolButtonsChanged();
} //=========================================== PCNMainWindow::PCNMainWindow()


PCNMainWindow::~PCNMainWindow() {
} //=========================================== PCNMainWindow::~PCNMainWindow()


void PCNMainWindow::closeEvent(QCloseEvent * /* event */) {

  if (!(isMinimized() || isMaximized() || isFullScreen())) {
    pOptions->MainWindow_xy = pos();
    pOptions->MainWindow_wh = size();
  }
  QHeaderView * hv = tbvMessages->horizontalHeader();
  pOptions->MainWindow_Col1 = hv->sectionSize(0);
  pOptions->MainWindow_Col2 = hv->sectionSize(1);
} //=========================================== PCNMainWindow::closeEvent()


void PCNMainWindow::ToolButtonsChanged() {
int  iToolButton = 0;
int  iOptionsToolButton = 0;
int  nToolOptionsButtons = pOptions->lstToolButtons.count();
bool ok;
bool checkable;
QString Key;

  while (iOptionsToolButton < nToolOptionsButtons && iToolButton < MAX_TOOLBUTTONS) {

    Key = pOptions->lstToolButtons[iOptionsToolButton];
    QString IconResource = ":/resources/icon32x32/";
    if (Key.length() != 2) {
      iOptionsToolButton++;
      continue;
    }
    ok = true;
    checkable = true;
    // choose the icons
    if (Key == "@N") {
      checkable = false;
    } else {
      if (Key == "@I") {
        checkable = false;
      } else {
        if (Key[0] != 'I' && Key[0] != 'N' &&
            (Key[1] < 'A' || Key[1] > 'Z' || Key[1] == 'Y')) {
          ok = false;
        }
      }
    }
    if (ok) {
      ToolButtonAction[iToolButton] = Key;
      QIcon icon;
      if (checkable) {
        icon.addFile(IconResource + Key + "p.png", QSize(), QIcon::Normal, QIcon::Off);
        icon.addFile(IconResource + Key + "a.png", QSize(), QIcon::Normal, QIcon::On);
      } else {
        icon = QIcon(IconResource + Key + ".png");
      }
      ToolButtons[iToolButton]->setIcon(icon);
      ToolButtons[iToolButton]->setIconSize(QSize(32, 32));
      ToolButtons[iToolButton]->setCheckable(checkable);
      if (checkable)
        ToolButtons[iToolButton]->setChecked(pOptions->MessageFilter.isFilterElement(Key));
      ToolButtons[iToolButton]->setAutoRaise(true);
      ToolButtons[iToolButton]->show();
      if (checkable)
        ToolButtons[iToolButton]->setToolTip(pOptions->StationName(Key));
      iToolButton++;
    }
    iOptionsToolButton++;
  }
  for (; iToolButton < MAX_TOOLBUTTONS; iToolButton++) {
    ToolButtonAction[iToolButton] = "";
    ToolButtons[iToolButton]->hide();
  }
} //=========================================== PCNMainWindow::ToolButtonsChanged()


void PCNMainWindow::onToolButtonAction(int iToolButton, bool checked) {
QString Key = ToolButtonAction[iToolButton];

  if (Key.length() != 2)
    return;

  if (Key == "@I") {
    on_actionStations_Filter_518_triggered();
    return;
  }
  if (Key == "@N") {
    on_actionStations_Filter_490_triggered();
    return;
  }
  pOptions->MessageFilter.setFilterElement(Key, checked);
  QList<PCNMessage*> lstMsg;
  pDatabase->queryMessages(lstMsg, pOptions->MessageFilter);
  model.newMessageList(lstMsg);
  setTableViewColumnWidth();
} //=========================================== PCNMainWindow::onToolButtonAction()


void PCNMainWindow::on_actionExit_activated() {
  logger.debug(__PRETTY_FUNCTION__, __LINE__, "By By");
  close();
} //=========================================== PCNMainWindow::on_actionExit_activated()


void PCNMainWindow::on_actionStations_Filter_490_triggered() {
  logger.debug(__PRETTY_FUNCTION__, __LINE__, " - ");

  PCNStationFilterDialog dlg(this);
  dlg.setData(490);
  if (dlg.exec() == QDialog::Accepted) {
    dlg.getData(490);
    ToolButtonsChanged();
    QList<PCNMessage*> lstMsg;
    pDatabase->queryMessages(lstMsg, pOptions->MessageFilter);
    model.newMessageList(lstMsg);
    setTableViewColumnWidth();
  }
} //=========================================== PCNMainWindow::on_actionStations_Filter_490_triggered()


void PCNMainWindow::on_actionStations_Filter_518_triggered() {
  logger.debug(__PRETTY_FUNCTION__, __LINE__, " - ");

  PCNStationFilterDialog dlg(this);
  dlg.setData(518);
  if (dlg.exec() == QDialog::Accepted) {
    dlg.getData(518);
    ToolButtonsChanged();
    QList<PCNMessage*> lstMsg;
    pDatabase->queryMessages(lstMsg, pOptions->MessageFilter);
    model.newMessageList(lstMsg);
    setTableViewColumnWidth();
  }
} //=========================================== PCNMainWindow::on_actionStations_Filter_518_triggered()


void PCNMainWindow::on_actionStation_Names_triggered() {
  logger.debug(__PRETTY_FUNCTION__, __LINE__, " - ");

  PCNStationNamesDialog dlg(this);
  dlg.setData(pOptions->lstStations518, pOptions->lstStations490);
  if (dlg.exec() == QDialog::Accepted) {
    dlg.getData(pOptions->lstStations518, pOptions->lstStations490);
  }
} //=========================================== PCNMainWindow::on_actionStation_Names_triggered()


void PCNMainWindow::on_actionPreferences_triggered() {

  PCNPreferences dlg(this);
  dlg.setData(pOptions);
  connect(pSerialPort, SIGNAL(receivedVersion(QString)), &dlg, SLOT(DeviceVersion(QString)));
  if (dlg.exec() == QDialog::Accepted) {
    dlg.getData(pOptions);
  }
  disconnect(pSerialPort, SIGNAL(readVersion(QString)), &dlg, SLOT(DeviceVersion(QString)));
} //=========================================== PCNMainWindow::on_actionPreferences_triggered()


void PCNMainWindow::on_actionCustomizeToolbar_triggered() {
PCNToolbarCustomizer dlg(this);

  if (dlg.exec() == QDialog::Accepted)
    ToolButtonsChanged();

} //=========================================== PCNMainWindow::on_actionCustomizeToolbar_triggered()


void PCNMainWindow::on_actionAbout_triggered() {
  QMessageBox::about(this, "PC NAVTEX", "PC NAVTEX\nVersion: 1.0.0\nⒸ Jürgen Reche\nLizenz GPLv3");
} //=========================================== PCNMainWindow::on_actionAbout_triggered()


void PCNMainWindow::on_tbtA_clicked(bool checked) {
  MsgTypeButton_clicked(checked, 'A');
} //=========================================== PCNMainWindow::on_tbtA_clicked()


void PCNMainWindow::on_tbtB_clicked(bool checked) {
  MsgTypeButton_clicked(checked, 'B');
} //=========================================== PCNMainWindow::on_tbtB_clicked()


void PCNMainWindow::on_tbtC_clicked(bool checked) {
  MsgTypeButton_clicked(checked, 'C');
} //=========================================== PCNMainWindow::on_tbtC_clicked()


void PCNMainWindow::on_tbtD_clicked(bool checked) {
  MsgTypeButton_clicked(checked, 'D');
} //=========================================== PCNMainWindow::on_tbtD_clicked()


void PCNMainWindow::on_tbtE_clicked(bool checked) {
  MsgTypeButton_clicked(checked, 'E');
} //=========================================== PCNMainWindow::on_tbtE_clicked()


void PCNMainWindow::on_tbt0_clicked(bool checked) {
  MsgTypeButton_clicked(checked, 'Z');
} //=========================================== PCNMainWindow::on_tbt0_clicked()


void PCNMainWindow::MsgTypeButton_clicked(bool checked, QChar cType) {

  pOptions->MessageFilter.setFilterElement(PCNMessageFilter::MSG_TYPES, cType, checked);

  QList<PCNMessage*> lstMsg;
  pDatabase->queryMessages(lstMsg, pOptions->MessageFilter);
  model.newMessageList(lstMsg);
  setTableViewColumnWidth();
} //=========================================== PCNMainWindow::MsgTypeButton_clicked()


void PCNMainWindow::on_tbtFilter490_clicked(bool /* checked */) {
  on_actionStations_Filter_490_triggered();
} //=========================================== PCNMainWindow::on_tbtFilter490_clicked()


void PCNMainWindow::on_tbtFilter518_clicked(bool /* checked */) {
  on_actionStations_Filter_518_triggered();
} //=========================================== PCNMainWindow::on_tbtFilter518_clicked()


void PCNMainWindow::on_DatabaseReadCompleted() {
  isStartup = false;
} //=========================================== PCNMainWindow::on_DatabaseReadCompleted()


void PCNMainWindow::on_startStoredMessages() {
  logger.debug(__PRETTY_FUNCTION__, __LINE__, "+++++++++++++++++");
  labDownload->setPixmap(QPixmap(QString::fromUtf8(":/resources/icon32x32/down-activ.png")));
  isDownload = true;
} //=========================================== PCNMainWindow::on_startStoredMessages()


void PCNMainWindow::on_endOfStoredMessages() {
  logger.debug(__PRETTY_FUNCTION__, __LINE__, "+++++++++++++++++");
  labDownload->setPixmap(QPixmap(QString::fromUtf8(":/resources/icon32x32/down-passiv.png")));
  isDownload = false;
  on_receivedMessage();
  QList<PCNMessage*> lstMsg;
  pDatabase->queryMessages(lstMsg, pOptions->MessageFilter);
  model.newMessageList(lstMsg);
  setTableViewColumnWidth();
} //=========================================== PCNMainWindow::on_endOfStoredMessages()


void PCNMainWindow::on_startMessage() {
  logger.debug(__PRETTY_FUNCTION__, __LINE__, "-----------------");
  labMessage->setPixmap(QPixmap(QString::fromUtf8(":/resources/icon32x32/message-activ.png")));
} //=========================================== PCNMainWindow::on_startMessage()


void PCNMainWindow::on_endOfMessage() {
  logger.debug(__PRETTY_FUNCTION__, __LINE__, "-----------------");
  labMessage->setPixmap(QPixmap(QString::fromUtf8(":/resources/icon32x32/message-passiv.png")));
} //=========================================== PCNMainWindow::on_endOfMessage()


void PCNMainWindow::on_receivedMessage() {
PCNMessage * Msg;
QString      Id;
QString      ReceiveDate;
bool         SqlError = false;

  logger.debug(__PRETTY_FUNCTION__, __LINE__, "new Messages: %i", pNewMessages->count());
  while(!pNewMessages->isEmpty()) {
    Msg = pNewMessages->takeFirst();
    logger.debug(__PRETTY_FUNCTION__, __LINE__, "%s", qPrintable(Msg->msgCode()));

    if (SqlError) {
      delete Msg;
      continue;
    }
// ------------------------------------

    QString msgReceiveDate = PCNDatabase::DateTimeToString(Msg->msgReceiveDate());
    switch (pDatabase->existsMessage(Msg, Id, ReceiveDate)) {
    case -1 : // SQL-error
      SqlError = true;
      break;
    case 0 : // not found
      SqlError = (pDatabase->insertMessage(Msg) <= 0);
      break;
    default : // message exits
      if (!Msg->isDownloaded()) {
        if (ReceiveDate < msgReceiveDate)
          SqlError = (pDatabase->updateReceiveDate(Id, msgReceiveDate) <= 0);
      }
    }
    if (SqlError)
      logger.error(__PRETTY_FUNCTION__, __LINE__, "Message not inserted: %s", qPrintable(Msg->msgCode()));
    else {
      // if a single fresh message comes from device, we put it into the model ( if it fits to filter )
      if (!Msg->isDownloaded() && Msg->fitsToFilter(pOptions->MessageFilter)) {
        // Msg goes into model's list, do not delete it
        model.insertMessage(Msg);
        setTableViewColumnWidth();
      } else {
        // If message comes from download all stored messages from device, we expect about 400 messages.
        // In these case, the message is stored in database only. After all messages are stored, we refresh model and view
        delete Msg;
      }
    }
  }
} //=========================================== PCNMainWindow::on_receivedMessage()


void PCNMainWindow::on_tbt00_clicked(bool checked) {
  onToolButtonAction(0, checked);
} //=========================================== PCNMainWindow::on_tbt00_clicked()
void PCNMainWindow::on_tbt01_clicked(bool checked) {
  onToolButtonAction(1, checked);
} //=========================================== PCNMainWindow::on_tbt01_clicked()
void PCNMainWindow::on_tbt02_clicked(bool checked) {
  onToolButtonAction(2, checked);
} //=========================================== PCNMainWindow::on_tbt02_clicked()
void PCNMainWindow::on_tbt03_clicked(bool checked) {
  onToolButtonAction(3, checked);
} //=========================================== PCNMainWindow::on_tbt03_clicked()
void PCNMainWindow::on_tbt04_clicked(bool checked) {
  onToolButtonAction(4, checked);
} //=========================================== PCNMainWindow::on_tbt04_clicked()
void PCNMainWindow::on_tbt05_clicked(bool checked) {
  onToolButtonAction(5, checked);
} //=========================================== PCNMainWindow::on_tbt05_clicked()
void PCNMainWindow::on_tbt06_clicked(bool checked) {
  onToolButtonAction(6, checked);
} //=========================================== PCNMainWindow::on_tbt06_clicked()
void PCNMainWindow::on_tbt07_clicked(bool checked) {
  onToolButtonAction(7, checked);
} //=========================================== PCNMainWindow::on_tbt07_clicked()
void PCNMainWindow::on_tbt08_clicked(bool checked) {
  onToolButtonAction(8, checked);
} //=========================================== PCNMainWindow::on_tbt08_clicked()
void PCNMainWindow::on_tbt09_clicked(bool checked) {
  onToolButtonAction(9, checked);
} //=========================================== PCNMainWindow::on_tbt09_clicked()
void PCNMainWindow::on_tbt10_clicked(bool checked) {
  onToolButtonAction(10, checked);
} //=========================================== PCNMainWindow::on_tbt10_clicked()
void PCNMainWindow::on_tbt11_clicked(bool checked) {
  onToolButtonAction(11, checked);
} //=========================================== PCNMainWindow::on_tbt11_clicked()



void PCNMainWindow::currentRowChanged(const QModelIndex & current, const QModelIndex & /* previous */) {

  logger.debug(__PRETTY_FUNCTION__, __LINE__, "-");

  if (current.isValid()) {
    QVariant data = current.data(Qt::UserRole);
    tedMessage->clear();
    tedMessage->insertPlainText(data.toString());
  }
} //=========================================== PCNMainWindow::currentRowChanged()


void PCNMainWindow::setTableViewColumnWidth() {
int  widthCode = 0;
int  widthInfo = 0;
QFont Font(tbvMessages->font());
QFontMetrics fm(Font);

  logger.debug(__PRETTY_FUNCTION__, __LINE__, "Font.famiy = %s   pointSize = %i   pixelSize = %i", qPrintable(Font.family()), Font.pointSize(), Font.pixelSize());

  model.maxTextWidth(fm, &widthCode, &widthInfo);
  if (widthCode > 0)
    tbvMessages->setColumnWidth(0, widthCode + pOptions->MainWindow_FontSize);
  if (widthInfo > 0)
    tbvMessages->setColumnWidth(1, widthInfo + pOptions->MainWindow_FontSize);
  logger.debug(__PRETTY_FUNCTION__, __LINE__, "widthCode = %i    widthInfo = %i", widthCode, widthInfo);
} //=========================================== PCNMainWindow::setTableViewColumnWidth()
