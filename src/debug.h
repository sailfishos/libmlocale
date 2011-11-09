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

#ifndef DEBUG_H
#define DEBUG_H

#include <QtDebug>

typedef enum MsgType {
    LogMsg,
    DebugMsg,
    WarningMsg,
    CriticalMsg,
    FatalMsg
} MsgType;

const char* binaryName() __attribute__ ((no_instrument_function));
void msgLogger(MsgType type, const char *format, ...) __attribute__ ((no_instrument_function));
void msgHandler(QtMsgType type, const char *msg) __attribute__ ((no_instrument_function));

#ifdef LOGDEBUG
#define lmlDebug(...) msgLogger(DebugMsg, __VA_ARGS__)
#else
#define lmlDebug(...) {;}
#endif

#define lmlLog(...) msgLogger(LogMsg, __VA_ARGS__)
#define lmlWarning(...) msgLogger(WarningMsg, __VA_ARGS__)
#define lmlCritical(...) msgLogger(CriticalMsg, __VA_ARGS__)
#define lmlFatal(...) msgLogger(FatalMsg, __VA_ARGS__)

#endif
