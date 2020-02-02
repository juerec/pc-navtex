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

#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#ifndef NO_THREADS
#include <QMutex>
#endif

#ifdef DEBUG
#undef DEBUG
#endif

class Logger {

  public :

    enum Level {
      DEBUG,
      INFO,
      WARNING,
      ERROR,
      NONE
    };

    Logger(const char * LogFile = "stderr", Level lev = DEBUG);
    ~Logger();
    void  setLevel(Level lev) { level = lev; }
    void  setLevel(const char * cLevel);
    Level getLevel() { return level; }

    void  error(const char * Location, const char * FormatString, ... );
    void  error(const char * Location, int Line, const char * FormatString, ... );
    void  warning(const char * Location, const char * FormatString, ... );
    void  warning(const char * Location, int Line, const char * FormatString, ... );
    void  info(const char * Location, const char * FormatString, ... );
    void  info(const char * Location, int Line, const char * FormatString, ... );
    void  debug(const char * Location, const char * FormatString, ... );
    void  debug(const char * Location, int Line, const char * FormatString, ... );

  protected :
    FILE * f;
    Level  level;
#ifndef NO_THREADS
    QMutex mutex;
#endif

    void log(const char * Level, const char * Location, const char * FormatString, va_list * ap);
    void log(const char * Level, const char * Location, int Line, const char * FormatString, va_list * ap);
};

extern Logger logger;

#endif /* LOGGER_H */
