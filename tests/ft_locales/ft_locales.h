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

#ifndef FT_LOCALES_H
#define FT_LOCALES_H

#include <QtTest/QtTest>
#include <QObject>
#include <Qt>

#include <MLocale>
#include <MCollator>
#include <MCalendar>

#include <QCoreApplication>

#ifdef HAVE_ICU
#include <unicode/unistr.h>
#include <unicode/ucal.h>
#include <unicode/coll.h>
#include <unicode/fieldpos.h>
#include <unicode/datefmt.h>
#include <unicode/calendar.h>
#include <unicode/smpdtfmt.h> // SimpleDateFormat
#include <unicode/numfmt.h>
#include <unicode/uloc.h>
#include <unicode/dtfmtsym.h> // date format symbols
#include <unicode/putil.h> // u_setDataDirectory
#endif

using ML10N::MLocale;

Q_DECLARE_METATYPE(MLocale);
Q_DECLARE_METATYPE(MLocale::CalendarType);
Q_DECLARE_METATYPE(QList<MLocale::Comparison>);

class Ft_Locales : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testBug210881();
    void testBug169305();
    void testMLocaleConstructor();
    void testCreateCLocale();

    void testLocaleNameParsingFunctions_data();
    void testLocaleNameParsingFunctions();

    void testMLocaleConstructorWithParams_data();
    void testMLocaleConstructorWithParams();

    void testMLocaleConstructorAndCategoryWithParams_data();
    void testMLocaleConstructorAndCategoryWithParams();

    void testMLocaleLanguage_data();
    void testMLocaleLanguage();
    void testMLocaleCountry_data();
    void testMLocaleCountry();
    void testMLocaleScript_data();
    void testMLocaleScript();

    void testMLocaleVariant_data();
    void testMLocaleVariant();

    void testMLocaleTextDirection_data();
    void testMLocaleTextDirection();

    void testMLocaleLanguageEndonym_data();
    void testMLocaleLanguageEndonym();
    void testMLocaleCountryEndonym_data();
    void testMLocaleCountryEndonym();
    void testMLocaleLocaleScripts_data();
    void testMLocaleLocaleScripts();

    void testMLocaleToLower_data();
    void testMLocaleToLower();
    void testMLocaleToUpper_data();
    void testMLocaleToUpper();

    void testMLocaleJoinStringList_data();
    void testMLocaleJoinStringList();

    void testMLocaleIndexBucket_data();
    void testMLocaleIndexBucket();

    void testDifferentStrengthComparison_data();
    void testDifferentStrengthComparison();

    void checkAvailableLocales();
};


#endif
