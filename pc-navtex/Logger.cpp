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

#include "Logger.h"

#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>

Logger logger;

Logger::Logger(const char * LogFile, Level lev) {

  level = lev;
  f = NULL;
  if (strcmp(LogFile, "stderr") == 0)
    f = stderr;
  if (strcmp(LogFile, "stdout") == 0)
    f = stdout;
  if (f == NULL) {
    f = fopen(LogFile, "a");
    if (f == NULL) {
      f = stderr;
      error("Logger::Logger()", "Cannot open LogFile \"%s\"     \"%s\"\n", LogFile, strerror(errno));
    }
  }
}

Logger::~Logger() {
  if (f != stderr && f != stdout)
    fclose(f);
}

void Logger::setLevel(const char * cLevel) {

  if (cLevel != NULL) {
    if (!strcmp(cLevel, "DEBUG"))
      level = DEBUG;
    if (!strcmp(cLevel, "INFO"))
      level = INFO;
    if (!strcmp(cLevel, "WARNING"))
      level = WARNING;
    if (!strcmp(cLevel, "ERROR"))
      level = ERROR;
    if (!strcmp(cLevel, "NONE"))
      level = NONE;
  }
}

void Logger::log(const char * Level, const char * Location, const char * fmt, va_list * ap) {
//time_t t = time(NULL);
struct tm tt; // = *localtime(&t);
struct timeval tv;

  gettimeofday(&tv, NULL);
  tt = *localtime(&tv.tv_sec);

#ifndef NO_THREADS
  mutex.lock();
#endif

  fprintf(f, "%04i-%02i-%02i %02i:%02i:%02i.%03i %s : %s ", tt.tm_year + 1900, tt.tm_mon + 1, tt.tm_mday, tt.tm_hour, tt.tm_min, tt.tm_sec, (int)(tv.tv_usec / 1000), Level == NULL ? "LOG:   " : Level, Location == NULL ? "---" : Location);

  if (fmt != NULL) {
/*
    va_list ap;
    va_start(ap, fmt);
    vfprintf(f, fmt, ap);
    va_end(ap);
*/
    vfprintf(f, fmt, *ap);
    if (strchr(fmt, '\n') == NULL)
      fprintf(f, "\n");
  } else {
    fprintf(f, "\n");
  }
#ifndef NO_THREADS
  mutex.unlock();
#endif
}

void Logger::log(const char * Level, const char * Location, int Line, const char * fmt, va_list * ap) {
//time_t t = time(NULL);
struct tm tt; // = *localtime(&t);
struct timeval tv;
const char * pSpace;

  if (Location == NULL) {
    pSpace = "---";
  } else {
    pSpace = strrchr(Location, ' ');
    if (pSpace != NULL)
      pSpace++;
    else
      pSpace = Location;
  }
  gettimeofday(&tv, NULL);
  tt = *localtime(&tv.tv_sec);

  // "2010-09-06 18:07:12.000  DEBUG : Logger::log() "
  fprintf(f, "%04i-%02i-%02i %02i:%02i:%02i.%03i %s : %s[%i] ", tt.tm_year + 1900, tt.tm_mon + 1, tt.tm_mday, tt.tm_hour, tt.tm_min, tt.tm_sec, (int)(tv.tv_usec / 1000), Level == NULL ? "LOG:   " : Level, pSpace, Line);

  if (fmt != NULL) {
/*
    va_list ap;
    va_start(ap, fmt);
    vfprintf(f, fmt, ap);
    va_end(ap);
*/
    vfprintf(f, fmt, *ap);
    if (strchr(fmt, '\n') == NULL)
      fprintf(f, "\n");
  } else {
    fprintf(f, "\n");
  }
}

void  Logger::error(const char * Location, const char * fmt, ...) {
  if (level <= ERROR) {
    va_list ap;
    va_start(ap, fmt);
    log("ERROR", Location, fmt, &ap);
    va_end(ap);
  }
}

void  Logger::error(const char * Location, int Line, const char * fmt, ...) {
  if (level <= ERROR) {
    va_list ap;
    va_start(ap, fmt);
    log("ERROR", Location, Line, fmt, &ap);
    va_end(ap);
  }
}

void  Logger::warning(const char * Location, const char * fmt, ...) {
  if (level <= WARNING) {
    va_list ap;
    va_start(ap, fmt);
    log("WARNING", Location, fmt, &ap);
    va_end(ap);
  }
}

void  Logger::warning(const char * Location, int Line, const char * fmt, ...) {
  if (level <= WARNING) {
    va_list ap;
    va_start(ap, fmt);
    log("WARNING", Location, Line, fmt, &ap);
    va_end(ap);
  }
}

void  Logger::info(const char * Location, const char * fmt, ...) {
  if (level <= INFO) {
    va_list ap;
    va_start(ap, fmt);
    log("INFO ", Location, fmt, &ap);
    va_end(ap);
  }
}

void  Logger::info(const char * Location, int Line, const char * fmt, ...) {
  if (level <= INFO) {
    va_list ap;
    va_start(ap, fmt);
    log("INFO ", Location, Line, fmt, &ap);
    va_end(ap);
  }
}

void  Logger::debug(const char * Location, const char * fmt, ...) {
  if (level <= DEBUG) {
    va_list ap;
    va_start(ap, fmt);
    log("DEBUG", Location, fmt, &ap);
    va_end(ap);
  }
}

void  Logger::debug(const char * Location, int Line,const char * fmt, ...) {
  if (level <= DEBUG) {
    va_list ap;
    va_start(ap, fmt);
    log("DEBUG", Location, Line, fmt, &ap);
    va_end(ap);
  }
}
