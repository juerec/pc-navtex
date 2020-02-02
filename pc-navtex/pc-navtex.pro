#****************************************************************************
#
#  This file is a part of the program "PC NAVTEX".
#
#  Copyright © 2020 Jürgen Reche
#
# PC NAVTEX is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# PC NAVTEX is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <https://www.gnu.org/licenses/>.
#
#**************************************************************************

#-------------------------------------------------
#
# Project created by QtCreator 2015-03-20T16:46:26
#
#-------------------------------------------------

QT       += core gui widgets serialport sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pc-navtex
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
           PCNMainWindow.cpp \
           Logger.cpp \
           PCNDatabase.cpp \
           PCNMessage.cpp \
           PCNSerialPort.cpp \
           PCNStation.cpp \
           PCNStationFilterDialog.cpp \
           PCNStationNamesDialog.cpp \
           PCNPreferences.cpp \
           PCNOptions.cpp \
           PCNMessageFilter.cpp \
           PCNMessagesTableModel.cpp \
    PCNToolbarCustomizer.cpp

HEADERS += PCNMainWindow.h \
           Logger.h \
           PCNDatabase.h \
           PCNMessage.h \
           PCNSerialPort.h \
           PCNStation.h \
           PCNStationFilterDialog.h \
           PCNStationNamesDialog.h \
           PCNPreferences.h \
           PCNOptions.h \
           PCNMessageFilter.h \
           PCNMessagesTableModel.h \
    PCNToolbarCustomizer.h

FORMS   += PCNMainWindow.ui \
    PCNStationNamesDialog.ui \
    PCNPreferences.ui \
    PCNStationFilterDialog.ui \
    PCNToolbarCustomizer.ui

LIBS    += -lz

RESOURCES += pc-navtex.qrc


TRANSLATIONS = pc-navtex_de.ts \
               pc-navtex_en.ts

isEmpty(QMAKE_LRELEASE) {
win32|os2:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\lrelease.exe
else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
unix {
!exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease-qt4 }
} else {
!exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease }
}
}

updateqm.input = TRANSLATIONS
updateqm.output = ${QMAKE_FILE_BASE}.qm
updateqm.commands = $$QMAKE_LRELEASE -silent ${QMAKE_FILE_IN} -qm ${QMAKE_FILE_BASE}.qm
updateqm.CONFIG += no_link target_predeps
QMAKE_EXTRA_COMPILERS += updateqm

OTHER_FILES +=
