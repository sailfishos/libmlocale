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

#ifndef UT_MLOCATIONDATABASE_H
#define UT_MLOCATIONDATABASE_H

#include <QtTest/QtTest>
#include <QObject>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <MLocale>

class Ut_MLocationDatabase : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testConstructors();

    void testNearestCity_data();
    void testNearestCity();

    void testCitiesInCountry_data();
    void testCitiesInCountry();

    void testMatchingCities_data();
    void testMatchingCities();

    void testCitiesInTimeZone_data();
    void testCitiesInTimeZone();

    void testCountryCodes_data();
    void testCountryCodes();

    void dumpCitiesInTimeZoneIds();

    void testCities_data();
    void testCities();

    void testCitiesDumpInfo();

    void testTimeZoneOffsets();
};

#endif
