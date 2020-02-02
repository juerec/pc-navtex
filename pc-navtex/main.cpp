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

#include "PCNMainWindow.h"
#include <QApplication>
#include <QFont>
#include <QMessageBox>
#include "PCNOptions.h"
#include "PCNDatabase.h"
#include "PCNSerialPort.h"
#include "Logger.h"
#include <QStandardPaths>


PCNOptions * pOptions = NULL;
PCNDatabase * pDatabase = NULL;
PCNSerialPort * pSerialPort = NULL;
QList<PCNMessage*> * pNewMessages = NULL;


int main(int argc, char *argv[]) {
QApplication app(argc, argv);
int ret;
PCNOptions Options;
PCNDatabase Database;
PCNSerialPort SerialPort;
QList<PCNMessage*>   lstNewMessages;

  pOptions = &Options;
  pDatabase = &Database;
  pSerialPort = &SerialPort;
  pNewMessages = &lstNewMessages;

  app.setOrganizationName("JR");
  app.setApplicationName("pc-navtex");
  app.setApplicationDisplayName("PC NAVTEX");
  app.setApplicationVersion("1.0.1");

  Options.readOptions(argc, argv);
  SerialPort.openSerialPort(Options.DeviceName);

QTranslator translator;

  translator.load(QString("pc-navtex_") + Options.Locale);
  app.installTranslator(&translator);

  if (!Database.init(Options.DatabaseName())) {
    QMessageBox::critical(NULL, app.applicationDisplayName(), QObject::tr("Error opening database"));
    return 1;
  }

  QFont Font(app.font());
  Font.setPointSize(Options.MainWindow_FontSize);
  app.setFont(Font);

PCNMainWindow w;

  w.resize(Options.MainWindow_wh);
  w.move(Options.MainWindow_xy);
  if (Options.MainWindow_format == "maximized")
    w.showMaximized();
  else {
    if (Options.MainWindow_format == "fullscreen")
      w.showFullScreen();
    else
      w.show();
  }

  w.show();

  ret = app.exec();

  pSerialPort->closeSerialPort();
  for (int i = lstNewMessages.count() - 1; i >= 0; i--)
    delete lstNewMessages[i];
  Options.save();
  return ret;
}
