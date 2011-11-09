/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libmeegotouch.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include <sys/stat.h>


#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <signal.h>
#include <execinfo.h>
#include <cxxabi.h>

#include <cstdarg>

#include <syslog.h>

#include "debug.h"

#define FILE_NAME_LEN 128

static char g_LogFileName[FILE_NAME_LEN] = {0};

const char* binaryName(){
    int stat_fd = 0;
    ssize_t read_result;
    char buf[2048] = {0};
    static bool haveName = false;
    static char runningBinaryFileName[128] = "unknown";

    if(haveName)
        return runningBinaryFileName;

    stat_fd = open("/proc/self/stat", O_RDONLY);
    if(stat_fd < 0){
        fprintf(stderr, "CRITICAL %s\n",
                "Cant open /proc/self/stat. Error: ");
        abort();
    }
    read_result = read(stat_fd, buf, sizeof(buf));
    close(stat_fd);
    if(read_result < 0 || sizeof(buf) < (size_t)read_result){
        fprintf(stderr, "CRITICAL %s\n",
                "Cant read /proc/self/stat. Error: ");
        abort();
    }

    sscanf(buf, "%*d (%[^)]s) %*s", runningBinaryFileName);
    if(runningBinaryFileName[sizeof(runningBinaryFileName)-1] != 0){
        if(stderr)
            fprintf(stderr, "CRITICAL %s\n",
                    "runningBinaryFileName too small for the name of the executable!");
        abort();
    }
    haveName = true;
    
    return runningBinaryFileName;
}

void msgLogger(MsgType type, const char *format, ...)
{
    static FILE *log = NULL;

    if(format[0] == 0)
        return;

    if(g_LogFileName[0] == 0)
        snprintf(g_LogFileName, FILE_NAME_LEN, "%s.%d-libmlocale.log", binaryName(), getpid());

#ifdef LOGDEBUG
    log = fopen(g_LogFileName, "a");
    if(log == NULL){
            fprintf(stderr, "CRITICAL %s\n",
                    "no log file opened");
    }
#else
    log = NULL;
#endif

    char msg[4096];
    {
        va_list args;
        va_start(args, format);
        vsnprintf(msg, sizeof(msg), format, args);
        va_end(args);
    }

    char timeStamp[64];
    {
        char timeStr[10];
        time_t t = time(NULL);
        strftime(timeStr, 10, "%H:%M:%S", localtime(&t));
        double c = (double)(clock())/(double)(CLOCKS_PER_SEC);

        snprintf(timeStamp, sizeof(timeStamp), "%8s %7.3f ", timeStr, c);
    }

    switch (type) {
        case LogMsg:
            syslog(LOG_NOTICE, "%s: %s %s", binaryName(), timeStamp, msg);
            fprintf(stderr, "LOG ");
            if(log)
                fprintf(log, "LOG ");
            break;
        case DebugMsg:
            syslog(LOG_DEBUG, "%s: %s %s", binaryName(), timeStamp, msg);
            fprintf(stderr, "DEBUG    ");
            if(log)
                fprintf(log, "DEBUG    ");
            break;
        case WarningMsg:
            syslog(LOG_WARNING, "%s: %s %s", binaryName(), timeStamp, msg);
            fprintf(stderr, "WARNING ");
            if(log)
                fprintf(log, "WARNING  ");
            break;
        case CriticalMsg:
            syslog(LOG_CRIT, "%s: %s %s", binaryName(), timeStamp, msg);
            fprintf(stderr, "CRITICAL ");
            if(log)
                fprintf(log, "CRITICAL ");
            break;
        case FatalMsg:
            syslog(LOG_EMERG, "%s: %s %s", binaryName(), timeStamp, msg);
            fprintf(stderr, "FATAL ");
            if(log)
                fprintf(log, "FATAL    ");
    }

    if(log)
        fprintf(log, "%s %s\n", timeStamp, msg);
    fprintf(stderr, "%s %s\n", timeStamp, msg);

    if(log)
        fflush(log);
    fflush(stderr);

    if(log){
        fclose(log);
        log = NULL;
    }

    if(type == FatalMsg)
        abort();
}

void msgHandler(QtMsgType type, const char *msg)
{
    MsgType msgType;
    switch (type) {
        case QtWarningMsg:
            msgType = WarningMsg;
            break;
        case QtCriticalMsg:
            msgType = CriticalMsg;
            break;
        case QtFatalMsg:
            msgType = FatalMsg;
            break;
        case QtDebugMsg:
        default:
            msgType = DebugMsg;
            break;
    }

    msgLogger(msgType, "%s", msg);
}
