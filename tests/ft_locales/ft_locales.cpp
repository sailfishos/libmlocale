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

#include "ft_locales.h"

#define VERBOSE_OUTPUT

using ML10N::MLocale;
using ML10N::MCalendar;
using ML10N::MCollator;

class TestLocale : public MLocale
{
};

void Ft_Locales::initTestCase()
{
}

void Ft_Locales::cleanupTestCase()
{
}

void Ft_Locales::init()
{
}

void Ft_Locales::cleanup()
{
}

void Ft_Locales::testBug210881()
{
    // see
    // https://projects.maemo.org/bugzilla/show_bug.cgi?id=210881
    MLocale locale;
    MLocale locale1;
    locale1.formatDateTime(QDateTime(),
                           MLocale::DateFull, MLocale::TimeFull,
                           MLocale::GregorianCalendar);
    locale1 = locale;
    locale1 = locale;
}

void Ft_Locales::testBug169305()
{
    // see
    // https://projects.maemo.org/bugzilla/show_bug.cgi?id=169305
    // and also:
    // https://projects.maemo.org/bugzilla/show_bug.cgi?id=184502
    MLocale locale0("en_US@layout-direction=auto");
    locale0.installTrCatalog("foo");
    MLocale::setDefault(locale0);
    QCOMPARE(QCoreApplication::translate("QApplication", "QT_LAYOUT_DIRECTION"), QString("LTR"));
    QCOMPARE(qApp->layoutDirection(), Qt::LeftToRight);
    MLocale locale1("ar_SA@layout-direction=auto");
    locale1.installTrCatalog("foo");
    MLocale::setDefault(locale1);
    QCOMPARE(QCoreApplication::translate("QApplication", "QT_LAYOUT_DIRECTION"), QString("RTL"));
    QCOMPARE(qApp->layoutDirection(), Qt::RightToLeft);
    MLocale locale2;
    QCOMPARE(locale2.name(), QString("ar_SA@layout-direction=auto"));
    locale2.installTrCatalog("foo");
    MLocale::setDefault(locale2);
    QCOMPARE(QCoreApplication::translate("QApplication", "QT_LAYOUT_DIRECTION"), QString("RTL"));
    QCoreApplication::processEvents();
    QCOMPARE(QCoreApplication::translate("QApplication", "QT_LAYOUT_DIRECTION"), QString("RTL"));
    QCOMPARE(qApp->layoutDirection(), Qt::RightToLeft);
    MLocale::setDefault(locale0);
    QCOMPARE(QCoreApplication::translate("QApplication", "QT_LAYOUT_DIRECTION"), QString("LTR"));
    QCoreApplication::processEvents();
    QCOMPARE(QCoreApplication::translate("QApplication", "QT_LAYOUT_DIRECTION"), QString("LTR"));
    QCOMPARE(qApp->layoutDirection(), Qt::LeftToRight);
}

void Ft_Locales::testMLocaleConstructor()
{
    TestLocale *testLocale = new TestLocale();
    QVERIFY2(testLocale->isValid(), "testLocale is not valid");
    testLocale->setCategoryLocale(MLocale::MLcNumeric, "de_CH");

    // copy constructor
    MLocale *mLocale = new MLocale(*testLocale);
    QVERIFY2(mLocale->isValid(), "mLocale is not valid");
    QCOMPARE(mLocale->name(), testLocale->name());
    QCOMPARE(mLocale->categoryName(MLocale::MLcNumeric),
             testLocale->categoryName(MLocale::MLcNumeric));
    mLocale->setCategoryLocale(MLocale::MLcNumeric, "zh_CN");
    QVERIFY2(mLocale->categoryName(MLocale::MLcNumeric)
             != testLocale->categoryName(MLocale::MLcNumeric),
             "mLocale->categoryName(MLocale::MLcNumeric) and testLocale->categoryName(MLocale::MLcNumeric) should be different");
    // assignment operator
    *mLocale = * testLocale;
    QVERIFY2(mLocale->isValid(), "mLocale is not valid");
    QCOMPARE(mLocale->name(), testLocale->name());
    QCOMPARE(mLocale->categoryName(MLocale::MLcNumeric),
             testLocale->categoryName(MLocale::MLcNumeric));

    // copy constructor
    TestLocale *testLocale2 = new TestLocale(*testLocale);
    QVERIFY2(testLocale2->isValid(), "testLocale2 is not valid");
    QCOMPARE(testLocale2->name(), testLocale->name());
    QCOMPARE(testLocale2->categoryName(MLocale::MLcNumeric),
             testLocale->categoryName(MLocale::MLcNumeric));
    testLocale2->setCategoryLocale(MLocale::MLcNumeric, "zh_CN");
    QVERIFY2(testLocale2->categoryName(MLocale::MLcNumeric)
             != testLocale->categoryName(MLocale::MLcNumeric),
             "testLocale2->categoryName(MLocale::MLcNumeric) and testLocale->categoryName(MLocale::MLcNumeric) should be different");
    // assignment operator
    *testLocale2 = * testLocale;
    QVERIFY2(testLocale2->isValid(), "testLocale2 is not valid");
    QCOMPARE(testLocale2->name(), testLocale->name());
    QCOMPARE(testLocale2->categoryName(MLocale::MLcNumeric),
             testLocale->categoryName(MLocale::MLcNumeric));

    delete mLocale;
    delete testLocale;
    delete testLocale2;
}

void Ft_Locales::testCreateCLocale()
{
    MLocale cLocale = MLocale::createCLocale();
    MLocale en_US_POSIX_locale("en_US_POSIX");
    QCOMPARE(cLocale.name(), QString("en_US_POSIX"));
    QCOMPARE(en_US_POSIX_locale.name(), QString("en_US_POSIX"));
}

void Ft_Locales::testLocaleNameParsingFunctions_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<QString>("localeLanguage");
    QTest::addColumn<QString>("localeScript");
    QTest::addColumn<QString>("localeCountry");
    QTest::addColumn<QString>("localeVariant");

    QTest::newRow("en")
        << "en"
        << "en"
        << ""
        << ""
        << "";
    QTest::newRow("en_US")
        << "en_US"
        << "en"
        << ""
        << "US"
        << "";
    QTest::newRow("ru_RU")
        << "ru_RU"
        << "ru"
        << ""
        << "RU"
        << "";
    QTest::newRow("ru_Cyrl_RU")
        << "ru_Cyrl_RU"
        << "ru"
        << "Cyrl"
        << "RU"
        << "";
    QTest::newRow("en_IE_PREEURO")
        << "en_IE_PREEURO"
        << "en"
        << ""
        << "IE"
        << "PREEURO";
    QTest::newRow("eo")
        << "eo"
        << "eo"
        << ""
        << ""
        << "";
    QTest::newRow("fr_FR@collation=phonebook;calendar=islamic-civil")
        << "fr_FR@collation=phonebook;calendar=islamic-civil"
        << "fr"
        << ""
        << "FR"
        << "";
    QTest::newRow("fr@collation=phonebook;calendar=islamic-civil")
        << "fr@collation=phonebook;calendar=islamic-civil"
        << "fr"
        << ""
        << ""
        << "";
    QTest::newRow("sr_RS")
        << "sr_RS"
        << "sr"
        << ""
        << "RS"
        << "";
    QTest::newRow("sr_Latn_RS_REVISED")
        << "sr_Latn_RS_REVISED"
        << "sr"
        << "Latn"
        << "RS"
        << "REVISED";
    QTest::newRow("sr_Cyrl_RS_REVISED@currency=USD")
        << "sr_Cyrl_RS_REVISED@currency=USD"
        << "sr"
        << "Cyrl"
        << "RS"
        << "REVISED";
    QTest::newRow("es__TRADITIONAL")
        << "es__TRADITIONAL"
        << "es"
        << ""
        << ""
        << "TRADITIONAL";
}

void Ft_Locales::testLocaleNameParsingFunctions()
{
    QFETCH(QString, localeName);
    QFETCH(QString, localeLanguage);
    QFETCH(QString, localeScript);
    QFETCH(QString, localeCountry);
    QFETCH(QString, localeVariant);

    MLocale::Category category = MLocale::MLcNumeric;
    MLocale locale(localeName);
    QCOMPARE(locale.name(), localeName);
    QCOMPARE(locale.language(), localeLanguage);
    QCOMPARE(locale.script(), localeScript);
    QCOMPARE(locale.country(), localeCountry);
    QCOMPARE(locale.variant(), localeVariant);
    MLocale englishLocale("en_US");
    locale = englishLocale;
    locale.setCategoryLocale(category, localeName);
    QCOMPARE(locale.categoryName(category), localeName);
    QCOMPARE(locale.categoryLanguage(category), localeLanguage);
    QCOMPARE(locale.categoryScript(category), localeScript);
    QCOMPARE(locale.categoryCountry(category), localeCountry);
    QCOMPARE(locale.categoryVariant(category), localeVariant);
}

void Ft_Locales::testMLocaleLanguage_data()
{
    QTest::addColumn<QString>("locale_name");
    QTest::addColumn<QString>("language");

    QTest::newRow("fi") << QString("fi_FI") << QString("fi");
    QTest::newRow("snd") << QString("snd_Arab_AF") << QString("snd");
    QTest::newRow("en") << QString("en") << QString("en");
}

void Ft_Locales::testMLocaleLanguage()
{
    QFETCH(QString, locale_name);
    QFETCH(QString, language);

    MLocale *z = new MLocale(locale_name);
    QCOMPARE(z->language(), language);
    delete z;
}

void Ft_Locales::testMLocaleCountry_data()
{
    QTest::addColumn<QString>("locale_name");
    QTest::addColumn<QString>("country");

    QTest::newRow("fi") << QString("fi_FI") << QString("FI");
    QTest::newRow("snd") << QString("snd_Arab_AF") << QString("AF");
    QTest::newRow("snd") << QString("snd_Arab") << QString("");
    QTest::newRow("snd") << QString("snd_Arab_EG") << QString("EG");
    QTest::newRow("en") << QString("en") << QString("");
}

void Ft_Locales::testMLocaleCountry()
{
    QFETCH(QString, locale_name);
    QFETCH(QString, country);

    MLocale *z = new MLocale(locale_name);

    QCOMPARE(z->country(), country);
    delete z;
}

void Ft_Locales::testMLocaleScript_data()
{
    QTest::addColumn<QString>("locale");
    QTest::addColumn<QString>("script");

    QTest::newRow("fi") << QString("fi_FI") << QString("");
    QTest::newRow("snd") << QString("snd_Arab_AF") << QString("Arab");
    QTest::newRow("snd") << QString("snd_Deva") << QString("Deva");
    QTest::newRow("en") << QString("en") << QString("");
    QTest::newRow("en_Arab") << QString("en_Arab") << QString("Arab");
    QTest::newRow("es__TRADITIONAL") << QString("es__TRADITIONAL") << QString("");
}

void Ft_Locales::testMLocaleScript()
{
    QFETCH(QString, locale);
    QFETCH(QString, script);

    MLocale *z = new MLocale(locale);
    QCOMPARE(z->script(), script);
    delete z;
}

void Ft_Locales::testMLocaleVariant_data()
{
    QTest::addColumn<QString>("locale");
    QTest::addColumn<QString>("variant");

    QTest::newRow("fi_FI") << QString("fi_FI") << QString("");
    QTest::newRow("sr_Cyrl_RS_REVISED@currency=USD") << QString("sr_Cyrl_RS_REVISED@currency=USD") << QString("REVISED");
    QTest::newRow("snd_AF_XXX") << QString("snd_AF_XXX") << QString("XXX");
    QTest::newRow("snd_Arab_AF_XXX") << QString("snd_Arab_AF_XXX") << QString("XXX");
    QTest::newRow("en") << QString("en") << QString("");
}

void Ft_Locales::testMLocaleVariant()
{
    QFETCH(QString, locale);
    QFETCH(QString, variant);

    MLocale *z = new MLocale(locale);
    QCOMPARE(z->variant(), variant);
    delete z;
}

void Ft_Locales::testMLocaleTextDirection_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<int>("direction");

    QTest::newRow("fi")
        << QString("fi")
        << (int) Qt::LeftToRight;
    QTest::newRow("fi@layout-direction=auto")
        << QString("fi@layout-direction=auto")
        << (int) Qt::LeftToRight;
    QTest::newRow("fi@layout-direction=ltr")
        << QString("fi@layout-direction=ltr")
        << (int) Qt::LeftToRight;
    QTest::newRow("fi@layout-direction=rtl")
        << QString("fi@layout-direction=rtl")
        << (int) Qt::RightToLeft;
    QTest::newRow("am")
        << QString("am")
        << (int) Qt::LeftToRight;
    QTest::newRow("am@layout-direction=auto")
        << QString("am@layout-direction=auto")
        << (int) Qt::LeftToRight;
    QTest::newRow("am@layout-direction=ltr")
        << QString("am@layout-direction=ltr")
        << (int) Qt::LeftToRight;
    QTest::newRow("am@layout-direction=rtl")
        << QString("am@layout-direction=rtl")
        << (int) Qt::RightToLeft;
    QTest::newRow("ar")
        << QString("ar")
        << (int) Qt::LeftToRight;
    QTest::newRow("ar@layout-direction=auto")
        << QString("ar@layout-direction=auto")
        << (int) Qt::RightToLeft;
    QTest::newRow("ar@layout-direction=ltr")
        << QString("ar@layout-direction=ltr")
        << (int) Qt::LeftToRight;
    QTest::newRow("ar@layout-direction=rtl")
        << QString("ar@layout-direction=rtl")
        << (int) Qt::RightToLeft;
    QTest::newRow("he")
        << QString("he")
        << (int) Qt::LeftToRight;
    QTest::newRow("he@layout-direction=auto")
        << QString("he@layout-direction=auto")
        << (int) Qt::RightToLeft;
    QTest::newRow("he@layout-direction=ltr")
        << QString("he@layout-direction=ltr")
        << (int) Qt::LeftToRight;
    QTest::newRow("he@layout-direction=rtl")
        << QString("he@layout-direction=rtl")
        << (int) Qt::RightToLeft;
    QTest::newRow("ur")
        << QString("ur")
        << (int) Qt::LeftToRight;
    QTest::newRow("ur@layout-direction=auto")
        << QString("ur@layout-direction=auto")
        << (int) Qt::RightToLeft;
    QTest::newRow("ur@layout-direction=ltr")
        << QString("ur@layout-direction=ltr")
        << (int) Qt::LeftToRight;
    QTest::newRow("ur@layout-direction=rtl")
        << QString("ur@layout-direction=rtl")
        << (int) Qt::RightToLeft;
    QTest::newRow("fa")
        << QString("fa")
        << (int) Qt::LeftToRight;
    QTest::newRow("fa@layout-direction=auto")
        << QString("fa@layout-direction=auto")
        << (int) Qt::RightToLeft;
    QTest::newRow("fa@layout-direction=ltr")
        << QString("fa@layout-direction=ltr")
        << (int) Qt::LeftToRight;
    QTest::newRow("fa@layout-direction=rtl")
        << QString("fa@layout-direction=rtl")
        << (int) Qt::RightToLeft;
    QTest::newRow("ps")
        << QString("ps")
        << (int) Qt::LeftToRight;
    QTest::newRow("ps@layout-direction=auto")
        << QString("ps@layout-direction=auto")
        << (int) Qt::RightToLeft;
    QTest::newRow("ps@layout-direction=ltr")
        << QString("ps@layout-direction=ltr")
        << (int) Qt::LeftToRight;
    QTest::newRow("ps@layout-direction=rtl")
        << QString("ps@layout-direction=rtl")
        << (int) Qt::RightToLeft;
    QTest::newRow("snd_Arab_AF")
        << QString("snd_Arab_AF")
        << (int) Qt::LeftToRight;
    QTest::newRow("snd_Arab_AF@layout-direction=auto")
        << QString("snd_Arab_AF@layout-direction=auto")
        << (int) Qt::RightToLeft;
    QTest::newRow("snd_Arab_AF@layout-direction=ltr")
        << QString("snd_Arab_AF@layout-direction=ltr")
        << (int) Qt::LeftToRight;
    QTest::newRow("snd_Arab_AF@layout-direction=rtl")
        << QString("snd_Arab_AF@layout-direction=rtl")
        << (int) Qt::RightToLeft;
    QTest::newRow("snd_Arab_AF_XXX")
        << QString("snd_Arab_AF_XXX")
        << (int) Qt::LeftToRight;
    QTest::newRow("snd_Arab_AF_XXX@layout-direction=auto")
        << QString("snd_Arab_AF_XXX@layout-direction=auto")
        << (int) Qt::RightToLeft;
    QTest::newRow("snd_Arab_AF_XXX@layout-direction=ltr")
        << QString("snd_Arab_AF_XXX@layout-direction=ltr")
        << (int) Qt::LeftToRight;
    QTest::newRow("snd_Arab_AF_XXX@layout-direction=rtl")
        << QString("snd_Arab_AF_XXX@layout-direction=rtl")
        << (int) Qt::RightToLeft;
    QTest::newRow("snd")
        << QString("snd_Deva_AF_XXX")
        << (int) Qt::LeftToRight;
    QTest::newRow("snd_Deva_AF_XXX@layout-direction=auto")
        << QString("snd_Deva_AF_XXX@layout-direction=auto")
        << (int) Qt::LeftToRight;
    QTest::newRow("snd_Deva_AF_XXX@layout-direction=ltr")
        << QString("snd_Deva_AF_XXX@layout-direction=ltr")
        << (int) Qt::LeftToRight;
    QTest::newRow("snd_Deva_AF_XXX@layout-direction=rtl")
        << QString("snd_Deva_AF_XXX@layout-direction=rtl")
        << (int) Qt::RightToLeft;
}

void Ft_Locales::testMLocaleTextDirection()
{
    QFETCH(QString, localeName);
    QFETCH(int, direction);

    MLocale defaultLocale("ar");
    MLocale::setDefault(defaultLocale);
    QCOMPARE(MLocale::defaultLayoutDirection(), Qt::LeftToRight);

    MLocale locale(localeName);
    QCOMPARE((int)locale.textDirection(), direction);
    MLocale::setDefault(locale);
    if(localeName.contains(QRegExp("@.*layout-direction=auto")))
        QCOMPARE(MLocale::defaultLayoutDirection(), Qt::LayoutDirectionAuto);
    else if(localeName.contains(QRegExp("@.*layout-direction=ltr")))
        QCOMPARE(MLocale::defaultLayoutDirection(), Qt::LeftToRight);
    else if(localeName.contains(QRegExp("@.*layout-direction=rtl")))
        QCOMPARE(MLocale::defaultLayoutDirection(), Qt::RightToLeft);
    else
        QCOMPARE(MLocale::defaultLayoutDirection(), Qt::LeftToRight);
}

void Ft_Locales::testMLocaleConstructorWithParams_data()
{
    QTest::addColumn<QString>("language");
    QTest::addColumn<QString>("country");
    QTest::addColumn<QString>("name");

    QTest::newRow("fi_FI") << QString("fi") << QString("FI") << QString("fi_FI");
    QTest::newRow("ar_SA") << QString("ar") << QString("SA") << QString("ar_SA");
    QTest::newRow("xx_YY") << QString("xx") << QString("YY") << QString("xx_YY");
}

void Ft_Locales::testMLocaleConstructorWithParams()
{
    QFETCH(QString, language);
    QFETCH(QString, country);
    QFETCH(QString, name);

    MLocale *z = new MLocale(language + '_' +  country);
    QVERIFY2(z->isValid(),
             "new MLocale(language + '_' + country)");
    QVERIFY2(z->name() == name, "z->name() differs from name");
    delete z;

    z = new MLocale(name);
    QVERIFY2(z->isValid(), "new MLocale(name)");
    QVERIFY2(z->name() == name, "z->name() differs from name");
    delete z;
}

void Ft_Locales::testMLocaleConstructorAndCategoryWithParams_data()
{
    QTest::addColumn<QString>("default_language");
    QTest::addColumn<QString>("default_country");
    QTest::addColumn<QString>("default_name");

    QTest::addColumn<QString>("category_language");
    QTest::addColumn<QString>("category_country");
    QTest::addColumn<QString>("category_name");

    QTest::newRow("fi_FI")
            << QString("fi")  << QString("FI") << QString("fi_FI")
            << QString("en")  << QString("GB") << QString("en_GB");
    QTest::newRow("ar_SA")
            << QString("ar")  << QString("SA") << QString("ar_SA")
            << QString("en")  << QString("GB") << QString("en_GB");
    QTest::newRow("xx_YY")
            << QString("xx")  << QString("YY") << QString("xx_YY")
            << QString("aa")  << QString("BB") << QString("aa_BB");
}

void Ft_Locales::testMLocaleConstructorAndCategoryWithParams()
{
    QFETCH(QString, default_language);
    QFETCH(QString, default_country);
    QFETCH(QString, default_name);

    QFETCH(QString, category_language);
    QFETCH(QString, category_country);
    QFETCH(QString, category_name);

    MLocale *z = 0;

    z = new MLocale(default_language + '_' + default_country);

    QVERIFY2(z->isValid(),
             "new MLocale(default_language + '_' + default_country)");
    QVERIFY2(z->name() == default_name, "z->name() differs from default_name");

    QVERIFY2(z->categoryName(MLocale::MLcTime) == default_name,
             "TIME: category name does not point to default category");
    z->setCategoryLocale(MLocale::MLcTime, category_language + '_' + category_country);
    QVERIFY2(z->categoryName(MLocale::MLcTime) == category_name,
             "TIME: category name does not point to category_name");

    QVERIFY2(z->categoryName(MLocale::MLcCollate) == default_name,
             "COLLATE: category name does not point to default category");

    z->setCategoryLocale(MLocale::MLcCollate, category_language + '_' + category_country);
    QVERIFY2(z->categoryName(MLocale::MLcCollate) == category_name,
             "COLLATE: category name does not point to category_name");

    QVERIFY2(z->categoryName(MLocale::MLcNumeric) == default_name,
             "NUMERIC: category name does not point to default category");

    z->setCategoryLocale(MLocale::MLcNumeric, category_language + '_' +  category_country);
    QVERIFY2(z->categoryName(MLocale::MLcNumeric) == category_name,
             "NUMERIC: category name does not point to category_name");
    delete z;
}

void Ft_Locales::testMLocaleLanguageEndonym_data()
{
    QTest::addColumn<QString>("locale_name");
    QTest::addColumn<QString>("endonym_result");

#if !defined(ALSO_VERIFY_ICU_DOES_ITS_JOB_AS_WE_EXPECT)
    QSKIP("Locale endonyms really testing only ICU data, skipping");
    return;
#endif

    QTest::newRow("ar")
            << QString("ar")
            << QString("??????????????");
    QTest::newRow("zh")
            << QString("zh")
            << QString("?????? (????????????)");
    QTest::newRow("zh_CN")
            << QString("zh_CN")
            << QString("?????? (????????????)");
    QTest::newRow("zh_HK")
            << QString("zh_HK")
            << QString("?????? (??????)");
    QTest::newRow("zh_TW")
            << QString("zh_TW")
            << QString("?????? (??????)");
    QTest::newRow("cs")
            << QString("cs")
            << QString("??e??tina");
    QTest::newRow("da")
            << QString("da")
            << QString("dansk");
    QTest::newRow("nl")
            << QString("nl")
            << QString("Nederlands");
    QTest::newRow("en")
            << QString("en")
            << QString("English (United Kingdom)");
    QTest::newRow("en_GB")
            << QString("en_GB")
            << QString("English (United Kingdom)");
    QTest::newRow("en_US")
            << QString("en_US")
            << QString("English (United States)");
    QTest::newRow("en_AU")
            << QString("en_AU")
            << QString("English (Australia)");
    QTest::newRow("fi")
            << QString("fi")
            << QString("suomi");
    QTest::newRow("fi_FI")
            << QString("fi_FI")
            << QString("suomi");
    QTest::newRow("fr")
            << QString("fr")
            << QString("fran??ais (France)");
    QTest::newRow("fr_FR")
            << QString("fr_FR")
            << QString("fran??ais (France)");
    QTest::newRow("fr_CA")
            << QString("fr_CA")
            << QString("fran??ais (Canada)");
    QTest::newRow("de")
            << QString("de")
            << QString("Deutsch");
    QTest::newRow("de_DE")
            << QString("de_DE")
            << QString("Deutsch");
    // de_CH needs a special fix because of
    // see http://site.icu-project.org/design/resbund/issues
    // make sure that the fix works:
    QTest::newRow("de_CH")
            << QString("de_CH")
            << QString("Schweizer Hochdeutsch");
    QTest::newRow("de_AT")
            << QString("de_AT")
            << QString("??sterreichisches Deutsch");
    QTest::newRow("de_DE")
            << QString("de_DE")
            << QString("Deutsch");
    QTest::newRow("hu")
            << QString("hu")
            << QString("magyar");
    QTest::newRow("hu_HU")
            << QString("hu_HU")
            << QString("magyar");
    QTest::newRow("id")
            << QString("id")
            << QString("Indonesia");
    QTest::newRow("id_ID")
            << QString("id_ID")
            << QString("Indonesia");
    QTest::newRow("it")
            << QString("it")
            << QString("italiano");
    QTest::newRow("it_IT")
            << QString("it_IT")
            << QString("italiano");
    QTest::newRow("ms")
            << QString("ms")
            << QString("Melayu");
    QTest::newRow("ms_BN")
            << QString("ms_BN")
            << QString("Melayu");
    QTest::newRow("ms_MY")
            << QString("ms_MY")
            << QString("Melayu");
    QTest::newRow("no")
            << QString("no")
            << QString("norsk");
    QTest::newRow("no_NO")
            << QString("no_NO")
            << QString("norsk");
    QTest::newRow("nb")
            << QString("nb")
            << QString("norsk bokm??l");
    QTest::newRow("nb_NO")
            << QString("nb_NO")
            << QString("norsk bokm??l");
    QTest::newRow("pl")
            << QString("pl")
            << QString("polski");
    QTest::newRow("pl_PL")
            << QString("pl_PL")
            << QString("polski");
    QTest::newRow("pt")
            << QString("pt")
            << QString("portugu??s (Portugal)");
    QTest::newRow("pt_PT")
            << QString("pt_PT")
            << QString("portugu??s (Portugal)");
    QTest::newRow("pt_BR")
            << QString("pt_BR")
            << QString("portugu??s (Brasil)");
    QTest::newRow("ro")
            << QString("ro")
            << QString("rom??n??");
    QTest::newRow("ro_MD")
            << QString("ro_MD")
            << QString("rom??n??");
    QTest::newRow("ro_RO")
            << QString("ro_RO")
            << QString("rom??n??");
    QTest::newRow("ru")
            << QString("ru")
            << QString("??????????????");
    QTest::newRow("ru_RU")
            << QString("ru_RU")
            << QString("??????????????");
    QTest::newRow("ru_UA")
            << QString("ru_UA")
            << QString("??????????????");
    QTest::newRow("sk")
            << QString("sk")
            << QString("sloven??ina");
    QTest::newRow("sk_SK")
            << QString("sk_SK")
            << QString("sloven??ina");
    QTest::newRow("es")
            << QString("es")
            << QString("espa??ol (Espa??a)");
    QTest::newRow("es_419")
            << QString("es_419")
            << QString("espa??ol (Am??rica Latina)");
    QTest::newRow("sv")
            << QString("sv")
            << QString("svenska");
    QTest::newRow("sv_FI")
            << QString("sv_FI")
            << QString("svenska");
    QTest::newRow("sv_SE")
            << QString("sv_SE")
            << QString("svenska");
    QTest::newRow("tr")
            << QString("tr")
            << QString("T??rk??e");
    QTest::newRow("tr_TR")
            << QString("tr_TR")
            << QString("T??rk??e");
    QTest::newRow("uk")
            << QString("uk")
            << QString("????????????????????");
    QTest::newRow("uk_UA")
            << QString("uk_UA")
            << QString("????????????????????");
    QTest::newRow("ja_JP")
            << QString("ja_JP")
            << QString("?????????");
}

void Ft_Locales::testMLocaleLanguageEndonym()
{
    QFETCH(QString, locale_name);
    QFETCH(QString, endonym_result);
    MLocale locale(locale_name);
#if defined(VERBOSE_OUTPUT)
    QTextStream debugStream(stdout);
    debugStream.setCodec("UTF-8");
    debugStream
        << "locale name: " << locale_name
        << " result: " << locale.languageEndonym()
        << " expected: " << endonym_result
        << "\n";
    debugStream.flush();
#endif
    QCOMPARE(locale.languageEndonym(), endonym_result);
}

void Ft_Locales::testMLocaleCountryEndonym_data()
{
    QTest::addColumn<QString>("locale_name");
    QTest::addColumn<QString>("endonym_result");

#if !defined(ALSO_VERIFY_ICU_DOES_ITS_JOB_AS_WE_EXPECT)
    QTest::newRow("en_GB")
            << QString("en_GB")
            << QString("United Kingdom");
    QTest::newRow("en_US")
            << QString("en_US")
            << QString("United States");
    return;
#endif

    QTest::newRow("fi_FI")
            << QString("fi_FI")
            << QString("Suomi");
    // de_CH needs a special fix because of
    // see http://site.icu-project.org/design/resbund/issues
    // make sure that the fix works:
    QTest::newRow("de_CH")
            << QString("de_CH")
            << QString("Schweiz");
    QTest::newRow("de_AT")
            << QString("de_AT")
            << QString("??sterreich");
    QTest::newRow("de_DE")
            << QString("de_DE")
            << QString("Deutschland");
    QTest::newRow("ja_JP")
            << QString("ja_JP")
            << QString("??????");
    QTest::newRow("zh_CN")
            << QString("zh_CN")
            << QString("??????");
}

void Ft_Locales::testMLocaleCountryEndonym()
{
    QFETCH(QString, locale_name);
    QFETCH(QString, endonym_result);
    MLocale locale(locale_name);
    QCOMPARE(locale.countryEndonym(), endonym_result);
}

void Ft_Locales::testMLocaleLocaleScripts_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<QStringList>("localeScripts");

#if !defined(ALSO_VERIFY_ICU_DOES_ITS_JOB_AS_WE_EXPECT)
    QTest::newRow("en_GB")
            << QString("en_GB")
            << (QStringList () << "Latn");
    QTest::newRow("ru_RU")
            << QString("ru_RU")
            << (QStringList () << "Cyrl");
    return;
#endif

    QTest::newRow("fi_FI")
            << QString("fi_FI")
            << (QStringList () << "Latn");
    QTest::newRow("sr_BA")
            << QString("sr_BA")
            << (QStringList () << "Cyrl");
    QTest::newRow("sr_CS")
            << QString("sr_CS")
            << (QStringList () << "Cyrl");
    QTest::newRow("sr_ME")
            << QString("sr_ME")
            << (QStringList () << "Latn");
    QTest::newRow("sr_ME")
            << QString("sr_Cyrl_ME")
            << (QStringList () << "Cyrl");
    QTest::newRow("sr_Latn_ME")
            << QString("sr_Latn_ME")
            << (QStringList () << "Latn");
    QTest::newRow("sr_RS")
            << QString("sr_RS")
            << (QStringList () << "Cyrl");
    QTest::newRow("sr_RS")
            << QString("sr_Cyrl_RS")
            << (QStringList () << "Cyrl");
    QTest::newRow("sr_Latn_RS")
            << QString("sr_Latn_RS")
            << (QStringList () << "Latn");
    QTest::newRow("sr_YU")
            << QString("sr_YU")
            << (QStringList () << "Cyrl");
    QTest::newRow("zh_TW")
            << QString("zh_TW")
            << (QStringList () << "Hani" << "Bopo");
    QTest::newRow("zh_Hant_TW")
            << QString("zh_Hant_TW")
            << (QStringList () << "Hani" << "Bopo");
    QTest::newRow("ja_JP")
            << QString("ja_JP")
            << (QStringList () << "Kana" << "Hira" << "Hani");
}

void Ft_Locales::testMLocaleLocaleScripts()
{
#if 0
    QFETCH(QString, localeName);
    QFETCH(QStringList, localeScripts);
    MLocale locale(localeName);
    QCOMPARE(locale.localeScripts(), localeScripts);
#else
    QSKIP("LocaleScripts no longer in ICU data, thus MLocale::localeScripts() is non-functional. Skipping");
#endif
}

void Ft_Locales::testMLocaleToLower_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<QString>("testString");
    QTest::addColumn<QString>("expectedMLocale");
    QTest::addColumn<QString>("expectedQt");

#if !defined(ALSO_VERIFY_ICU_DOES_ITS_JOB_AS_WE_EXPECT)
    QTest::newRow("en_GB")
        << QString("en_GB")
        << QString("Big")
        << QString("big")
        << QString("big")
        ;

    QTest::newRow("de_DE")
        << QString("de_DE")
        << QString("Gr????e")
        << QString("gr????e")
        << QString("gr????e")
        ;

    return;
#endif

    QTest::newRow("de_DE")
        << QString("de_DE")
        << QString("Gr????e")
        << QString("gr????e")
        << QString("gr????e")
        ;

    QTest::newRow("de_CH")
        << QString("de_CH") // no difference in behaviour to de_CH here
        << QString("Gr????e")
        << QString("gr????e")
        << QString("gr????e")
        ;

    QTest::newRow("de_DE")
        << QString("de_DE")
        << QString("GR?????E")
        << QString("gr????e")
        << QString("gr????e")
        ;

    QTest::newRow("de_CH") // no difference in behaviour to de_CH here
        << QString("de_CH")
        << QString("GR?????E")
        << QString("gr????e")
        << QString("gr????e")
        ;

    QTest::newRow("en_GB")
        << QString("en_GB")
        << QString("ITEM NA??VE ??")
        // last two chars are U+0069 (LATIN SMALL LETTER I) U+307 (COMBINING DOT ABOVE)
        // both for Qt and MLocale here
        << QString("item na??ve i??")
        << QString("item na??ve i??")
        ;

    QTest::newRow("tr_TR")
            << QString("tr_TR")
            << QString("ITEM NA??VE ??")
               // I lowercases to ?? in Turkish locale and ?? to i
            << QString("??tem na??ve i")
               // Qt???s toLower is *not* locale aware
               // last two chars are U+0069 (LATIN SMALL LETTER I) U+307 (COMBINING DOT ABOVE)
            << QString("item na??ve i??")
               ;

    QTest::newRow("en_GB")
        << QString("de_GB")
        << QString("????")
        << QString("????")
        << QString("????")
        ;

    QTest::newRow("el_GR")
        << QString("el_GR")
        << QString("????")
        << QString("????")
        << QString("????")
        ;

    QTest::newRow("en_GB")
            << QString("en_GB")
            << QString("????")
            << QString("????")
               // Qt???s toLower is *not* context aware and lowercases the final ??
               // the same way as a non-final one:
            << QString("????")
               ;

    QTest::newRow("el_GR")
            << QString("el_GR")
            << QString("????")
            << QString("????")
               // Qt???s toLower is *not* context aware and lowercases the final ??
               // the same way as a non-final one:
            << QString("????")
               ;

    QTest::newRow("en_GB")
        << QString("en_GB")
        << QString("??") // U+00CC (LATIN CAPITAL LETTER I WITH GRAVE)
        << QString("??") // U+00EC (LATIN SMALL LETTER I WITH GRAVE)
        << QString("??") // U+00EC (LATIN SMALL LETTER I WITH GRAVE)
        ;

    QTest::newRow("lt_LT")
        << QString("lt_LT")
        << QString("??") // U+00CC (LATIN CAPITAL LETTER I WITH GRAVE)
        << QString("i????") // U+0069 U+0307 U+0300
        << QString("??") // U+00EC (LATIN SMALL LETTER I WITH GRAVE)
        ;

    QTest::newRow("en_GB")
        << QString("en_GB")
        << QString("??") // U+012E (LATIN CAPITAL LETTER I WITH OGONEK)
        << QString("??") // U+012F (LATIN SMALL LETTER I WITH OGONEK)
        << QString("??") // U+012F (LATIN SMALL LETTER I WITH OGONEK)
        ;

    QTest::newRow("lt_LT")
        << QString("lt_LT")
        << QString("??") // U+012E (LATIN CAPITAL LETTER I WITH OGONEK)
        << QString("??") // U+012F (LATIN SMALL LETTER I WITH OGONEK)
        << QString("??") // U+012F (LATIN SMALL LETTER I WITH OGONEK)
        ;

    QTest::newRow("en_GB")
        << QString("en_GB")
        << QString("????") // U+012E (LATIN CAPITAL LETTER I WITH OGONEK) U+0303 (COMBINING TILDE)
        << QString("????") // U+012F U+0303
        << QString("????") // U+012F U+0303
        ;

    QTest::newRow("lt_LT")
        << QString("lt_LT")
        << QString("????") // U+012E (LATIN CAPITAL LETTER I WITH OGONEK) U+0303 (COMBINING TILDE)
        << QString("??????") // U+012F U+0307 U+0303
        << QString("????") // U+012F U+0303
        ;
}

void Ft_Locales::testMLocaleToLower()
{
    QFETCH(QString, localeName);
    QFETCH(QString, testString);
    QFETCH(QString, expectedMLocale);
    QFETCH(QString, expectedQt);

    MLocale locale(localeName);
    QString resultMLocale = locale.toLower(testString);
    // MLocale::setDefault() calls QLocale::setDefault() (using McLcNumeric!)
    MLocale::setDefault(locale);
    QString resultQt = testString.toLower();

    QVector<uint> testStringUcs4 = testString.toUcs4();
    QString testStringCodePoints;
    for (int i = 0; i < testStringUcs4.size(); ++i) {
        testStringCodePoints += " U+" + QString::number(testStringUcs4[i],16);
    }
    QVector<uint> expectedMLocaleUcs4 = expectedMLocale.toUcs4();
    QString expectedMLocaleCodePoints;
    for (int i = 0; i < expectedMLocaleUcs4.size(); ++i) {
        expectedMLocaleCodePoints += " U+" + QString::number(expectedMLocaleUcs4[i],16);
    }
    QVector<uint> expectedQtUcs4 = expectedQt.toUcs4();
    QString expectedQtCodePoints;
    for (int i = 0; i < expectedQtUcs4.size(); ++i) {
        expectedQtCodePoints += " U+" + QString::number(expectedQtUcs4[i],16);
    }
    QVector<uint> resultMLocaleUcs4 = resultMLocale.toUcs4();
    QString resultMLocaleCodePoints;
    for (int i = 0; i < resultMLocaleUcs4.size(); ++i) {
        resultMLocaleCodePoints += " U+" + QString::number(resultMLocaleUcs4[i],16);
    }
    QVector<uint> resultQtUcs4 = resultQt.toUcs4();
    QString resultQtCodePoints;
    for (int i = 0; i < resultQtUcs4.size(); ++i) {
        resultQtCodePoints += " U+" + QString::number(resultQtUcs4[i],16);
    }
#if 1
    QTextStream debugStream(stdout);
    debugStream.setCodec("UTF-8");
    debugStream
        << "localeName      [" << localeName      << "]\n"
        << "testString      [" << testString      << "]" << testStringCodePoints << "\n"
        << "expectedMLocale [" << expectedMLocale << "]" << expectedMLocaleCodePoints <<"\n"
        << "resultMLocale   [" << resultMLocale   << "]" << resultMLocaleCodePoints << "\n"
        << "expectedQt      [" << expectedQt      << "]" << expectedQtCodePoints << "\n"
        << "resultQt        [" << resultQt        << "]" << resultQtCodePoints << "\n"
        ;
#endif
    QCOMPARE(resultMLocale, expectedMLocale);
    QCOMPARE(resultQt, expectedQt);
}

void Ft_Locales::testMLocaleToUpper_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<QString>("testString");
    QTest::addColumn<QString>("expectedMLocale");
    QTest::addColumn<QString>("expectedQt");

#if !defined(ALSO_VERIFY_ICU_DOES_ITS_JOB_AS_WE_EXPECT)
    QTest::newRow("en_GB")
        << QString("en_GB")
        << QString("Big")
        << QString("BIG")
        << QString("BIG")
        ;

    QTest::newRow("de_DE")
        << QString("de_DE")
        << QString("Gr????e")
        << QString("GR??SSE")
        << QString("GR??SSE")
        ;

    return;
#endif

    QTest::newRow("de_DE")
        << QString("de_DE")
        << QString("Gr????e")
        << QString("GR??SSE")
        << QString("GR??SSE")
        ;

    QTest::newRow("de_CH")
        << QString("de_CH") // no difference in behaviour to de_CH here
        << QString("Gr????e")
        << QString("GR??SSE")
        << QString("GR??SSE")
        ;

    QTest::newRow("de_DE")
        << QString("de_DE")
        << QString("GR?????E")
        << QString("GR?????E")
        << QString("GR?????E")
        ;

    QTest::newRow("en_GB")
        << QString("en_GB")
        << QString("item na??ve ??")
        << QString("ITEM NA??VE I")
        << QString("ITEM NA??VE I")
        ;

    QTest::newRow("tr_TR")
            << QString("tr_TR")
            << QString("item na??ve ??")
            << QString("??TEM NA??VE I")
            << QString("ITEM NA??VE I")
               ;

    QTest::newRow("en_GB")
        << QString("en_GB")
        << QString("i??") // U+0069 (LATIN SMALL LETTER I) U+0307  (COMBINING DOT ABOVE)
        << QString("I??") // U+0049 (LATIN CAPITAL LETTER I) U+0307  (COMBINING DOT ABOVE)
        << QString("I??") // U+0049 (LATIN CAPITAL LETTER I) U+0307  (COMBINING DOT ABOVE)
        ;

    QTest::newRow("tr_TR")
            << QString("tr_TR")
            << QString("i??") // U+0069 (LATIN SMALL LETTER I) U+0307  (COMBINING DOT ABOVE)
            << QString("????") // U+0130 (LATIN CAPITAL LETTER I WITH DOT ABOVE) U+0307  (COMBINING DOT ABOVE)
            << QString("I??") // U+0049 (LATIN CAPITAL LETTER I) U+0307  (COMBINING DOT ABOVE)
               ;

    QTest::newRow("en_GB")
        << QString("en_GB")
        << QString("????") // U+0131 (LATIN SMALL LETTER DOTLESS I) U+0307  (COMBINING DOT ABOVE)
        << QString("I??") // U+0049 (LATIN CAPITAL LETTER I) U+0307  (COMBINING DOT ABOVE)
        << QString("I??") // U+0049 (LATIN CAPITAL LETTER I) U+0307  (COMBINING DOT ABOVE)
        ;

    QTest::newRow("tr_TR")
        << QString("tr_TR")
        << QString("????") // U+0131 (LATIN SMALL LETTER DOTLESS I) U+0307  (COMBINING DOT ABOVE)
        << QString("I??") // U+0049 (LATIN CAPITAL LETTER I) U+0307  (COMBINING DOT ABOVE)
        << QString("I??") // U+0049 (LATIN CAPITAL LETTER I) U+0307  (COMBINING DOT ABOVE)
        ;

    QTest::newRow("en_GB")
        << QString("en_GB")
        << QString("???")
        << QString("FFI")
        << QString("FFI")
        ;
}

void Ft_Locales::testMLocaleToUpper()
{
    QFETCH(QString, localeName);
    QFETCH(QString, testString);
    QFETCH(QString, expectedMLocale);
    QFETCH(QString, expectedQt);

    MLocale locale(localeName);
    QString resultMLocale = locale.toUpper(testString);
    // MLocale::setDefault() calls QLocale::setDefault() (using McLcNumeric!)
    MLocale::setDefault(locale);
    QString resultQt = testString.toUpper();

    QVector<uint> testStringUcs4 = testString.toUcs4();
    QString testStringCodePoints;
    for (int i = 0; i < testStringUcs4.size(); ++i) {
        testStringCodePoints += " U+" + QString::number(testStringUcs4[i],16);
    }
    QVector<uint> expectedMLocaleUcs4 = expectedMLocale.toUcs4();
    QString expectedMLocaleCodePoints;
    for (int i = 0; i < expectedMLocaleUcs4.size(); ++i) {
        expectedMLocaleCodePoints += " U+" + QString::number(expectedMLocaleUcs4[i],16);
    }
    QVector<uint> expectedQtUcs4 = expectedQt.toUcs4();
    QString expectedQtCodePoints;
    for (int i = 0; i < expectedQtUcs4.size(); ++i) {
        expectedQtCodePoints += " U+" + QString::number(expectedQtUcs4[i],16);
    }
    QVector<uint> resultMLocaleUcs4 = resultMLocale.toUcs4();
    QString resultMLocaleCodePoints;
    for (int i = 0; i < resultMLocaleUcs4.size(); ++i) {
        resultMLocaleCodePoints += " U+" + QString::number(resultMLocaleUcs4[i],16);
    }
    QVector<uint> resultQtUcs4 = resultQt.toUcs4();
    QString resultQtCodePoints;
    for (int i = 0; i < resultQtUcs4.size(); ++i) {
        resultQtCodePoints += " U+" + QString::number(resultQtUcs4[i],16);
    }
#if 1
    QTextStream debugStream(stdout);
    debugStream.setCodec("UTF-8");
    debugStream
        << "localeName      [" << localeName      << "]\n"
        << "testString      [" << testString      << "]" << testStringCodePoints << "\n"
        << "expectedMLocale [" << expectedMLocale << "]" << expectedMLocaleCodePoints <<"\n"
        << "resultMLocale   [" << resultMLocale   << "]" << resultMLocaleCodePoints << "\n"
        << "expectedQt      [" << expectedQt      << "]" << expectedQtCodePoints << "\n"
        << "resultQt        [" << resultQt        << "]" << resultQtCodePoints << "\n"
        ;
#endif
    QCOMPARE(resultMLocale, expectedMLocale);
    QCOMPARE(resultQt, expectedQt);
}

void Ft_Locales::testMLocaleJoinStringList_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<QStringList>("texts");
    QTest::addColumn<QString>("expectedResult");

    QTest::newRow("en_GB")
        << QString("en_GB")
        << (QStringList() << "a (b)" << "?? (??)" << "c (d)" << "???")
        << QString("???a (b)???, ????? (??)???, ???c (d)???, ?????????")
        ;

    // MLocale::localeScript() needs to be reimplemented. Skipping locales that rely on that data.
    return;
    QTest::newRow("ar_EG")
        << QString("ar_EG")
        << (QStringList() << "a (b)" << "?? (??)" << "c (d)" << "???")
        << QString("???a (b)????? ????? (??)????? ???c (d)????? ?????????")
        ;
    QTest::newRow("fa_IR")
        << QString("fa_IR")
        << (QStringList() << "a (b)" << "?? (??)" << "c (d)" << "???")
        << QString("???a (b)????? ????? (??)????? ???c (d)????? ?????????")
        ;
    QTest::newRow("zh_CN")
        << QString("zh_CN")
        << (QStringList() << "a (b)" << "?? (??)" << "c (d)" << "???")
        << QString("???a (b)??????????? (??)?????????c (d)???????????????")
        ;
    QTest::newRow("zh_TW")
        << QString("zh_TW")
        << (QStringList() << "a (b)" << "?? (??)" << "c (d)" << "???")
        << QString("???a (b)??????????? (??)?????????c (d)???????????????")
        ;
    QTest::newRow("ja_JP")
        << QString("ja_JP")
        << (QStringList() << "a (b)" << "?? (??)" << "c (d)" << "???")
        << QString("???a (b)??????????? (??)?????????c (d)???????????????")
        ;
}

void Ft_Locales::testMLocaleJoinStringList()
{
    QFETCH(QString, localeName);
    QFETCH(QStringList, texts);
    QFETCH(QString, expectedResult);

    MLocale locale(localeName);
    QString result = locale.joinStringList(texts);
#if defined(VERBOSE_OUTPUT)
    QTextStream debugStream(stdout);
    debugStream.setCodec("UTF-8");
    debugStream
        << "localeName= " << localeName << "\n"
        << " expectedResult= [" << expectedResult << "]\n"
        << " result=         [" << result << "]\n";
    debugStream.flush();
#endif
    QCOMPARE(result, expectedResult);
}

void Ft_Locales::testMLocaleIndexBucket_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<QString>("lcCollate");
    QTest::addColumn<QStringList>("stringsSorted");
    QTest::addColumn<QStringList>("expectedBuckets");

#if !defined(ALSO_VERIFY_ICU_DOES_ITS_JOB_AS_WE_EXPECT)
    QStringList basicEnglishStringsSorted =
        (QStringList()
         <<"??"<<"??"<<"cote"<<"cot??"<<"c??te"<<"c??t??"
         <<"f"<<"F"<<"ff"<<"???"<<"Ff"<<"ffi"<<"???"<<"Ffi"<<"???"<<"???"<<"???"
         <<"i"<<"I"<<"??"<<"??"<<"??"<<"??"
         <<"??"<<"??"<<"??e"<<"??e"<<"??E"
         <<"s"<<"S"<<"??"<<"??"<<"???"
         <<"test"<<"Test"
         <<"z"<<"Z"<<"zx"<<"Zx"<<"ZX"
            );
    QStringList basicEnglishExpectedBuckets =
        (QStringList()
         <<"C"<<"C"<<"C"<<"C"<<"C"<<"C"
         <<"F"<<"F"<<"F"<<"F"<<"F"<<"F"<<"F"<<"F"<<"F"<<"F"<<"F"
         <<"I"<<"I"<<"I"<<"I"<<"I"<<"I"
         <<"O"<<"O"<<"O" <<"O" <<"O"
         <<"S"<<"S"<<"S"<<"S"<<"S"
         <<"T"<<"T"
         <<"Z"<<"Z"<<"Z"<<"Z"<<"Z"
            );
    QTest::newRow("en_US")
        <<"ja_JP"
        <<"en_US"
        << basicEnglishStringsSorted
        << basicEnglishExpectedBuckets;
    return;
#endif

    QStringList englishStringsSorted =
        (QStringList()
         <<"??"<<"??"<<"cote"<<"cot??"<<"c??te"<<"c??t??"
         <<"f"<<"F"<<"ff"<<"???"<<"Ff"<<"ffi"<<"???"<<"Ffi"<<"???"<<"???"<<"???"
         <<"i"<<"I"<<"??"<<"??"<<"??"<<"??"
         <<"??"<<"??"<<"??e"<<"??e"<<"??E"
         <<"s"<<"S"<<"??"<<"??"<<"???"
         <<"test"<<"Test"
         <<"z"<<"Z"<<"zx"<<"Zx"<<"ZX"
         <<"??????"
            );
    QStringList englishExpectedBuckets =
        (QStringList()
         <<"C"<<"C"<<"C"<<"C"<<"C"<<"C"
         <<"F"<<"F"<<"F"<<"F"<<"F"<<"F"<<"F"<<"F"<<"F"<<"F"<<"F"
         <<"I"<<"I"<<"I"<<"I"<<"I"<<"I"
         <<"O"<<"O"<<"O" <<"O" <<"O"
         <<"S"<<"S"<<"S"<<"S"<<"S"
         <<"T"<<"T"
         <<"Z"<<"Z"<<"Z"<<"Z"<<"Z"
         <<"???"
            );
    QTest::newRow("en_US")
        <<"ja_JP"
        <<"en_US"
        << englishStringsSorted
        << englishExpectedBuckets;
    QStringList turkishStringsSorted =
        (QStringList()
         <<"cote"<<"cot??"<<"c??te"<<"c??t??"
         <<"??"<<"??"
         <<"??"<<"I"<<"i"<<"??"
         <<"??"<<"??"<<"??e"<<"??e"<<"??E"
         <<"s"<<"S"<<"??"<<"???"
         <<"test"<<"Test"
         <<"z"<<"Z"<<"zx"<<"Zx"<<"ZX"
         <<"??????");
    QStringList turkishExpectedBuckets =
        (QStringList()
         <<"C"<<"C"<<"C"<<"C"
         <<"??"<<"??"
         <<"I"<<"I"<<"??"<<"??"
         <<"??"<<"??"<<"??"<<"??"<<"??"
         <<"S"<<"S"<<"S"<<"S"
         <<"T"<<"T"
         <<"Z"<<"Z"<<"Z"<<"Z"<<"Z"
         <<"???");
    QTest::newRow("tr_TR")
        <<"ja_JP"
        <<"tr_TR"
        << turkishStringsSorted
        << turkishExpectedBuckets;
    QStringList germanStringsSorted =
        (QStringList()
         <<"Afghanistan"
         <<"??gypten"
         <<"??"
         <<"??"
         <<"cote"
         <<"cot??"
         <<"c??te"
         <<"c??t??"
         <<"f"
         <<"F"
         <<"ff"
         <<"???"
         <<"Ff"
         <<"ffi"
         <<"???"
         <<"Ffi"
         <<"???"
         <<"???"
         <<"???"
         <<"i"
         <<"I"
         <<"??"
         <<"??"
         <<"??"
         <<"Italien"
         <<"??"
         <<"j"
         <<"??"
         <<"??"
         <<"??e"
         <<"??e"
         <<"??E"
         <<"of"
         <<"??f"
         <<"s"
         <<"S"
         <<"??"
         <<"??"
         <<"???"
         <<"test"
         <<"Test"
         <<"z"
         <<"Z"
         <<"zx"
         <<"Zx"
         <<"ZX"
         <<"??????"
            );
    QStringList germanExpectedBuckets =
        (QStringList()
         <<"A"
         <<"A"
         <<"C"
         <<"C"
         <<"C"
         <<"C"
         <<"C"
         <<"C"
         <<"F"
         <<"F"
         <<"F"
         <<"F"
         <<"F"
         <<"F"
         <<"F"
         <<"F"
         <<"F"
         <<"F"
         <<"F"
         <<"I"
         <<"I"
         <<"I"
         <<"I"
         <<"I"
         <<"I"
         <<"I"
         <<"J"
         <<"O"
         <<"O"
         <<"O"
         <<"O"
         <<"O"
         <<"O"
         <<"O"
         <<"S"
         <<"S"
         <<"S"
         <<"S"
         <<"S"
         <<"T"
         <<"T"
         <<"Z"
         <<"Z"
         <<"Z"
         <<"Z"
         <<"Z"
         <<"???"
            );
    QTest::newRow("de_DE")
        <<"ja_JP"
        <<"de_DE"
        << germanStringsSorted
        << germanExpectedBuckets;
    QTest::newRow("de_DE")
        <<"ja_JP"
        <<"de_DE@collation=standard"
        << germanStringsSorted
        << germanExpectedBuckets;
    QStringList germanPhonebookStringsSorted =
        (QStringList()
         <<"??gypten"
         <<"Afghanistan"
         <<"??"
         <<"??"
         <<"cote"
         <<"cot??"
         <<"c??te"
         <<"c??t??"
         <<"f"
         <<"F"
         <<"ff"
         <<"???"
         <<"Ff"
         <<"ffi"
         <<"???"
         <<"Ffi"
         <<"???"
         <<"???"
         <<"???"
         <<"i"
         <<"I"
         <<"??"
         <<"??"
         <<"??"
         <<"Italien"
         <<"??"
         <<"j"
         <<"??"
         <<"??"
         <<"??e"
         <<"??e"
         <<"??E"
         <<"??f"
         <<"of"
         <<"s"
         <<"S"
         <<"??"
         <<"??"
         <<"???"
         <<"test"
         <<"Test"
         <<"z"
         <<"Z"
         <<"zx"
         <<"Zx"
         <<"ZX"
         <<"??????"
            );
    QStringList germanPhonebookExpectedBuckets =
        (QStringList()
         <<"A"
         <<"A"
         <<"C"
         <<"C"
         <<"C"
         <<"C"
         <<"C"
         <<"C"
         <<"F"
         <<"F"
         <<"F"
         <<"F"
         <<"F"
         <<"F"
         <<"F"
         <<"F"
         <<"F"
         <<"F"
         <<"F"
         <<"I"
         <<"I"
         <<"I"
         <<"I"
         <<"I"
         <<"I"
         <<"I"
         <<"J"
         <<"O"
         <<"O"
         <<"O"
         <<"O"
         <<"O"
         <<"O"
         <<"O"
         <<"S"
         <<"S"
         <<"S"
         <<"S"
         <<"S"
         <<"T"
         <<"T"
         <<"Z"
         <<"Z"
         <<"Z"
         <<"Z"
         <<"Z"
         <<"???"
            );
    QTest::newRow("de_DE")
        <<"ja_JP"
        <<"de_DE@collation=phonebook"
        << germanPhonebookStringsSorted
        << germanPhonebookExpectedBuckets;
    QStringList frenchStringsSorted =
        (QStringList()
           <<"a"
           <<"A"
           <<"??"
           <<"ae"
           <<"??"
           <<"??"
           <<"aez"
           <<"??z"
           <<"??z"
           <<"az"
           <<"??z"
           <<"??z"
           <<"??z"
           <<"cote"
           <<"c??te"
           <<"cot??"
           <<"c??t??"
           <<"cz"
           <<"??z"
           <<"ez"
           <<"??z"
           <<"??z"
           <<"??z"
           <<"??z"
           <<"iz"
           <<"??z"
           <<"??z"
           <<"??"
           <<"??"
           <<"??e"
           <<"??e"
           <<"??E"
           <<"oz"
           <<"??z"
           <<"??z"
           <<"uz"
           <<"??z"
           <<"??z"
           <<"yz"
           <<"??z"
           <<"z"
           <<"Z"
           <<"zx"
           <<"Zx"
           <<"ZX"
           <<"??????"
            );
    QStringList frenchExpectedBuckets =
        (QStringList()
           <<"A"
           <<"A"
           <<"A"
           <<"A"
           <<"A"
           <<"A"
           <<"A"
           <<"A"
           <<"A"
           <<"A"
           <<"A"
           <<"A"
           <<"A"
           <<"C"
           <<"C"
           <<"C"
           <<"C"
           <<"C"
           <<"C"
           <<"E"
           <<"E"
           <<"E"
           <<"E"
           <<"E"
           <<"I"
           <<"I"
           <<"I"
           <<"O"
           <<"O"
           <<"O"
           <<"O"
           <<"O"
           <<"O"
           <<"O"
           <<"O"
           <<"U"
           <<"U"
           <<"U"
           <<"Y"
           <<"Y"
           <<"Z"
           <<"Z"
           <<"Z"
           <<"Z"
           <<"Z"
           <<"???"
            );
    QTest::newRow("fr_CA")
        <<"ja_JP"
        <<"fr_CA"
        << frenchStringsSorted
        << frenchExpectedBuckets;
    QStringList spanishStringsSorted =
        (QStringList()
         <<"aaa"
         <<"a??a"
         <<"aaaa"
         <<"a??aa"
            );
    QStringList spanishExpectedBuckets =
        (QStringList()
         <<"A"
         <<"A"
         <<"A"
         <<"A"
            );
    QTest::newRow("es_ES")
        <<"ja_JP"
        <<"es_ES"
        << spanishStringsSorted
        << spanishExpectedBuckets;
    QTest::newRow("es_419")
        <<"ja_JP"
        <<"es_419"
        << spanishStringsSorted
        << spanishExpectedBuckets;
    QTest::newRow("es_MX")
        <<"ja_JP"
        <<"es_MX"
        << spanishStringsSorted
        << spanishExpectedBuckets;
    QStringList finnishStringsSorted =
        (QStringList()
         <<"a"
         <<"aa"
         <<"aaaa"
         <<"aaao"
         <<"aabae"
         <<"z"
         <<"??ao"
            );
    QStringList finnishExpectedBuckets =
        (QStringList()
         <<"A"
         <<"A"
         <<"A"
         <<"A"
         <<"A"
         <<"Z"
         <<"??"
            );
    QTest::newRow("fi_FI")
        <<"ja_JP"
        <<"fi_FI"
        << finnishStringsSorted
        << finnishExpectedBuckets;
    QStringList danishStringsSorted =
        (QStringList()
         <<"a"
         <<"z"
         <<"aa"
         <<"??ao"
         <<"aaao"
         <<"aabae"
         <<"aaaa"
            );
    QStringList danishExpectedBuckets =
        (QStringList()
         <<"A"
         <<"Z"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
            );
    QTest::newRow("da_DK")
        <<"ja_JP"
        <<"da_DK"
        << danishStringsSorted
        << danishExpectedBuckets;
    QStringList hungarianStringsSorted =
        (QStringList()
         <<"a"
         <<"z"
         <<"zs"
            );
    QStringList hungarianExpectedBuckets =
        (QStringList()
         <<"A"
         <<"Z"
         <<"ZS"
            );
    QTest::newRow("hu_HU")
        <<"ja_JP"
        <<"hu_HU"
        << hungarianStringsSorted
        << hungarianExpectedBuckets;
    QStringList czechStringsSorted =
        (QStringList()
         <<"c"
         <<"Cc"
         <<"CC"
         <<"c??"
         <<"c??"
         <<"??"
         <<"??r??k"
         <<"Etsk"
         <<"??zfd"
         <<"h"
         <<"H"
         <<"ch"
         <<"cH"
         <<"Ch"
         <<"CH"
         <<"i"
         <<"I"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"?????????"
         <<"??????");
    QStringList czechExpectedBuckets =
        (QStringList()
         <<"C"
         <<"C"
         <<"C"
         <<"??"
         <<"??"
         <<"??"
         <<"E"
         <<"E"
         <<"E"
         <<"H"
         <<"H"
         <<"CH"
         <<"CH"
         <<"CH"
         <<"CH"
         <<"I"
         <<"I"
         <<"U"
         <<"U"
         <<"??"
         <<"??"
         <<"??"
         <<"???"
         <<"???");
    QTest::newRow("cs_CZ")
        <<"ja_JP"
        <<"cs_CZ"
        << czechStringsSorted
        << czechExpectedBuckets;
    QStringList russianStringsSorted =
        (QStringList()
         <<"????????????"
         <<"????????????"
         <<"??"
         <<"??"
         <<"????"
         <<"????"
         <<"??????"
         <<"??????"
         <<"????????"
         <<"????????"
         <<"??????"
         <<"??????"
         <<"??"
         <<"??"
         <<"????"
         <<"????"
         <<"????????"
         <<"????????"
         <<"??????????"
         <<"??????????"
         <<"????????"
         <<"????????"
         <<"??"
         <<"??"
         <<"??????????"
         <<"??????????"
         <<"??"
         <<"??"
         <<"??"
         <<"c"
         <<"c??"
         <<"c??"
         <<"??"
         <<"Cc"
         <<"CC"
         <<"ch"
         <<"cH"
         <<"Ch"
         <<"CH"
         <<"h"
         <<"H"
         <<"i"
         <<"I"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"?????????"
         <<"??????");
    QStringList russianExpectedBuckets =
        (QStringList()
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"C"
         <<"C"
         <<"C"
         <<"C"
         <<"C"
         <<"C"
         <<"C"
         <<"C"
         <<"C"
         <<"C"
         <<"H"
         <<"H"
         <<"I"
         <<"I"
         <<"U"
         <<"U"
         <<"??"
         <<"??"
         <<"???"
         <<"???");
    QStringList macedonianStringsSorted =
        (QStringList()
         <<"??"
         <<"??"
         <<"??"
            );
    QStringList macedonianExpectedBuckets =
        (QStringList()
         <<"??"
         <<"??"
         <<"??"
            );
    QTest::newRow("ar_SA")
        <<"ja_JP"
        <<"ar_SA"
        <<(QStringList()
           <<"arabia"
           <<"zero"
           <<"??")
        <<(QStringList()
           <<"A"
           <<"Z"
           <<"??");
    QTest::newRow("fa_IR")
        <<"ja_JP"
        <<"fa_IR"
        <<(QStringList()
           <<"??")
        <<(QStringList()
           <<"??");
    QStringList japaneseStringsSorted =
        (QStringList()
         <<"???"
         <<"???"
         <<"("
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"??"
         <<"???"
         <<"john"
         <<"John"
         <<"saki"
         <<"Saki"
         <<"??"
         <<"???"
         <<"u"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"??????"
         <<"???"
         <<"???"
         <<"???"
         <<"??????"
         <<"??????"
         <<"?????????"
         <<"?????????"
         <<"???"
         <<"???"
         <<"???1"
         <<"???1"
         <<"???2"
         <<"???2"
         <<"?????????"
         <<"?????????"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"?????????"
         <<"?????????"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"??????"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
            );
    QStringList japaneseExpectedBuckets =
        (QStringList()
         <<"???"
         <<"???"
         <<"("
         <<"("
         <<"("
         <<"("
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"1"
         <<"1"
         <<"A"
         <<"??"
         <<"F"
         <<"J"
         <<"J"
         <<"S"
         <<"S"
         <<"S"
         <<"T"
         <<"U"
         <<"V"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
            );
   QTest::newRow("ko_KR@collation=standard")
        <<"ja_JP"
        <<"ko_KR@collation=standard"
        <<(QStringList()
           <<"???"<<"???"
           <<"A"
           <<"???"
           <<"f"<<"F"<<"ff"<<"???"<<"Ff"<<"ffi"<<"???"<<"Ffi"<<"???"<<"???"<<"???"
           <<"john"
           <<"????????????"
           <<"John"
           <<"????????????"
           <<"saki"
           <<"????????????"
           <<"Saki"
           <<"????????????"
           <<"??"<<"??"
           <<"????????????"<<"????????????"
           <<"??"
           <<"??"
           <<"??"
           <<"??"
           <<"???"
           <<"???"
           <<"???"
           <<"???"

           <<"???" // U+1100 HANGUL CHOSEONG KIYEOK
           <<"???" // U+AD50 HANGUL SYLLABLE GYO
           <<"???" // Korean Pronunciation: KYO HYO
           <<"???" // Korean Pronunciation: KWUN

           <<"???" // U+1102 HANGUL CHOSEONG NIEUN

           <<"???" // U+1103 HANGUL CHOSEONG TIKEUT
           <<"???"
           <<"???"

           <<"???" // U+1105 HANGUL CHOSEONG RIEUL
           <<"???" // Korean Pronunciation: LYU
           <<"???" // Korean Pronunciation: LI

           <<"???" // U+1106 HANGUL CHOSEONG MIEUM

           <<"???" // U+1107 HANGUL CHOSEONG PIEUP
           <<"???"
           <<"???"

           <<"???" // U+1109 HANGUL CHOSEONG SIOS
           <<"???" // U+C0B0 HANGUL SYLLABLE SAN
           <<"???" // Korean Pronunciation: SAN
           <<"???" // Korean Pronunciation: SANG
           <<"???" // U+C2DD HANGUL SYLLABLE SIG
           <<"???" // U+C2E0 HANGUL SYLLABLE SIN

           <<"???" // U+110B HANGUL CHOSEONG IEUNG
           <<"???" // Korean Pronunciation: A OK
           <<"???" // Korean Pronunciation: WI

           <<"???" // U+110C HANGUL CHOSEONG CIEUC
           <<"???" // Korean Pronunciation: CAP
           <<"???" // Korean Pronunciation: CANG
           <<"???" // Korean Pronunciation: CE
           <<"???" // U+C815 HANGUL SYLLABLE JEONG

           <<"???" // U+110E HANGUL CHOSEONG CHIEUCH
           <<"???" // Korean Pronunciation: CHI

           <<"???" // U+110F HANGUL CHOSEONG KHIEUKH

           <<"???" // U+1110 HANGUL CHOSEONG THIEUTH

           <<"???" // U+1111 HANGUL CHOSEONG PHIEUPH

           <<"???" // U+1112 HANGUL CHOSEONG HIEUH
           <<"???" // U+D559 HANGUL SYLLABLE HAG
           <<"???" // Korean Pronunciation: HAK

           <<"???"
           <<"???"
           <<"???"
           <<"???"
           <<"???"
           <<"???" // No Korean Pronunciation
           <<"???" // No Korean Pronunciation
           <<"???" // No Korean Pronunciation
            )
        <<(QStringList()
           <<"???"<<"???"
           <<"A"
           <<"A"
           <<"F"<<"F"<<"F"<<"F"<<"F"<<"F"<<"F"<<"F"<<"F"<<"F"<<"F"
           <<"J"
           <<"J"
           <<"J"
           <<"J"
           <<"S"
           <<"S"
           <<"S"
           <<"S"
           <<"??"<<"??"
           <<"??"<<"??"
           <<"??"
           <<"??"
           <<"??"
           <<"??"
           <<"???"
           <<"???"
           <<"???"
           <<"???"

           <<"???" // U+1100 HANGUL CHOSEONG KIYEOK
           <<"???" // U+1100 HANGUL CHOSEONG KIYEOK
           <<"???" // U+1100 HANGUL CHOSEONG KIYEOK
           <<"???" // U+1100 HANGUL CHOSEONG KIYEOK

           <<"???" // U+1102 HANGUL CHOSEONG NIEUN

           <<"???" // U+1103 HANGUL CHOSEONG TIKEUT
           <<"???" // U+1103 HANGUL CHOSEONG TIKEUT
           <<"???" // U+1103 HANGUL CHOSEONG TIKEUT

           <<"???" // U+1105 HANGUL CHOSEONG RIEUL
           <<"???" // U+1105 HANGUL CHOSEONG RIEUL
           <<"???" // U+1105 HANGUL CHOSEONG RIEUL

           <<"???" // U+1106 HANGUL CHOSEONG MIEUM

           <<"???" // U+1107 HANGUL CHOSEONG PIEUP
           <<"???" // U+1107 HANGUL CHOSEONG PIEUP
           <<"???" // U+1107 HANGUL CHOSEONG PIEUP

           <<"???" // U+1109 HANGUL CHOSEONG SIOS
           <<"???" // U+1109 HANGUL CHOSEONG SIOS
           <<"???" // U+1109 HANGUL CHOSEONG SIOS
           <<"???" // U+1109 HANGUL CHOSEONG SIOS
           <<"???" // U+1109 HANGUL CHOSEONG SIOS
           <<"???" // U+1109 HANGUL CHOSEONG SIOS

           <<"???" // U+110B HANGUL CHOSEONG IEUNG
           <<"???" // U+110B HANGUL CHOSEONG IEUNG
           <<"???" // U+110B HANGUL CHOSEONG IEUNG

           <<"???" // U+110C HANGUL CHOSEONG CIEUC
           <<"???" // U+110C HANGUL CHOSEONG CIEUC
           <<"???" // U+110C HANGUL CHOSEONG CIEUC
           <<"???" // U+110C HANGUL CHOSEONG CIEUC
           <<"???" // U+110C HANGUL CHOSEONG CIEUC

           <<"???" // U+110E HANGUL CHOSEONG CHIEUCH
           <<"???" // U+110E HANGUL CHOSEONG CHIEUCH

           <<"???" // U+110F HANGUL CHOSEONG KHIEUKH

           <<"???" // U+1110 HANGUL CHOSEONG THIEUTH

           <<"???" // U+1111 HANGUL CHOSEONG PHIEUPH

           <<"???" // U+1112 HANGUL CHOSEONG HIEUH
           <<"???" // U+1112 HANGUL CHOSEONG HIEUH
           <<"???" // U+1112 HANGUL CHOSEONG HIEUH

           <<"???"
           <<"???"
           <<"???"
           <<"???"
           <<"???"
           <<"???"
           <<"???"
           <<"???"
            );
    QStringList strokeStringsSorted =
        (QStringList()
         <<"???"<<"???"
         <<"01"
         <<"02"
         <<"1"
         <<"15"
         <<"2"
         <<"25"
         <<"???"
         <<"???"
         <<"???" // 1 stroke
         <<"???" // 1 stroke
         <<"???" // 2 strokes
         <<"???" // 2 strokes
         <<"???" // 3 strokes U+4E09
         <<"???" // 3 strokes U+5DDD
         <<"???" // 4 strokes U+4E11
         <<"???" // 4 strokes U+6728
         <<"???" // 5 strokes, 1st stroke horizontal U+4E19
         <<"???" // dong1, 5 strokes, 1st stroke horizontal, simplified of ??? U+4E1C
         <<"???" // dong1, 5 strokes, 1st stroke down to left U+51AC
         <<"???" // 5 strokes, 1st stroke vertical, kHanyuPinlu ka3 U+5361 kBigFive A564 kGB0 3108 kRSUnicode 25.3
         <<"???" // 5 strokes, 1st stroke vertical, kHanyuPinlu jiao4   U+53EB kBigFive A573 kGB0 2948 kRSUnicode 30.2
         <<"????" // 5 strokes, 1st stroke vertical, kHanyuPinlu - U+20BA9 kBigFive - kGB0- kRSUnicode 30.2"
         <<"??????" // 5 strokes, 1st stroke vertical, 1st character U+5B81
         <<"???" // 6 strokes U+4E1E
         <<"???" // 6 strokes, 1st stroke vertical, kMandarin liu2 U+5218 kBigFive - kGB0 3385 kRSUnicode 18.4
         <<"??? Lucy" // liu2
         <<"??????" // liu2 li4
         <<"?????????" // liu2 li4 jun1
         <<"???Lucy" // liu2
         <<"???" // dong4, 6 strokes, 1st stroke horizontal, simplified of ??? U+52A8
         <<"???" // 6 strokes, NB#279399
         <<"???" // 6 strokes U+7F8A
         <<"???" // chi2, 6 strokes, 1st stroke bending, simplified of ???, kHanyuPinlu chi2 U+9A70 kBigFive - kGB0 1959 kRSUnicode 187'.3
         <<"???" // 7 strokes, 1st stroke vertical U+4E32
         <<"???" // dong4, 7 strokes, 1st stroke down to right, simplified of ??? U+51BB
         <<"???" // 7 strokes, 1st stroke bending, kHanyuPinlu jun1 U+541B kBigFive A767 kGB0 3093 kRSUnicode 30.4
         <<"?????? ???" // 7 strokes, 1st stroke bending
         <<"???" // 7 strokes, 1st stroke bending, kHanyuPinlu zhang1  U+5F20 kBigFive    - kGB0 5337 kRSUnicode 57.4"
         <<"???" // 7 strokes, 1st stroke horizontal, kHanyuPinlu li5 U+674E kBigFive A7F5 kGB0 3278 kRSUnicode 75.3
         <<"??????" // 7 strokes, 1st stroke down to right, 1st character U+6C99
         <<"???" // 7 strokes, NB#279399 (a1, 8 strokes, 1st stroke bending, kHanyuPinlu a1 U+963F kBigFive AAFC kGB0 1602 kRSUnicode 170.5)
         <<"???" // 7 strokes, NB#279399
         <<"???" // 8 strokes, 1st stroke down to right U+4E26
         <<"???" // 8 strokes! (libicu 44 counts 9 strokes, which looks like an ERROR), 1st stroke horizontal U+4E9F
         <<"???" // dong4, 8 strokes, 1st stroke down to left U+4F97
         <<"???" // dong1, 8 strokes, 1st stroke vertical U+549A
         <<"???" // dong1, 8 strokes, 1st stroke vertical, simplified of ??? U+5CBD
         <<"???" // dong1, 8 strokes, 1st stroke horizontal, traditional of ??? U+6771
         <<"???" // 8 strokes, NB#279399
         <<"???" // dong1, 8 strokes, 1st stroke horizontal, (CLDR trunk 20110909: 9 strokes (old, 2 part kusakanmuri gives 9 strokes)) U+82F3
         <<"???" // 9 strokes, 1st stroke horizontal U+5A01
         <<"???" // quia3??? ??? strokes, 1st stroke vertical, (Japanese only! variant of ???) U+5CE0 kBigFive - kGB0 - kRSUnicode 46.6
         <<"???" // dong4, 9 strokes, 1st stroke vertical U+606B
         <<"???" // 9 strokes, 1st stroke horizontal, kHanyuPinlu liu3 U+67F3 kBigFive AC68 kGB0 3388 kRSUnicode 75.5"
         <<"??????" // liu3 dong1; 2nd character 5 strokes, initial stroke horizontal, 2nd character U+4E1C
         <<"??????" // liu3 dong1; 2nd character 5 strokes, initial stroke down to left, 2nd character U+51AC
         <<"?????????" // liu3 li4 jun1; 3rd character 5 strokes, initial stroke vertical, 3rd character U+519B
         <<"?????????" // liu3 li4 jun1; 3rd character traditional, 9 strokes, initial stroke vertical, 3rd character U+8ECD
         <<"??????" // liu3 dong1; 2nd character 8 strokes, initial stroke vertical
         <<"??????" // liu3 dong1; 2nd character traditional, 8 strokes, initial stroke horizontal
         <<"???" // dong4, 9 strokes, 1st stroke horizontal, simplified of ??? U+680B
         <<"???" // dong1, 9 strokes, 1st stroke down to left U+6C21
         <<"???" // dong4, 9 strokes, 1st stroke down to right U+6D1E
         <<"???" // 9 strokes, NB#279399
         <<"???" // 10 strokes, 1st stroke down to left U+4E58
         <<"???" // dong4, 10 strokes, 1st stroke down to right, traditional of ??? U+51CD
         <<"???" // dong4, 10 strokes, 1st stroke down to left U+80F4
         <<"???" // 10 strokes, 1st stroke vertical U+9ADF
         <<"???" // dong1, 10 strokes, 1st stroke horizontal, simplified of ??? U+9E2B
         <<"???" // 11 strokes, 1st stroke horizontal U+4E7E
         <<"???" // dong4, 11 strokes, 1st stroke down to left, traditional of ??? U+52D5
         <<"???" // dong1, dong4, 11 strokes, 1st stroke vertical, traditional of ??? U+5D20
         <<"???" // 11 strokes, 1st stroke bending, kHanyuPinlu zhang1  U+5F35 kBigFive B169 kGB0    - kRSUnicode 57.8"
         <<"???" // dong1, 11 strokes, 1st stroke down to right U+6DB7
         <<"???" // dong4, 11 strokes, 1st stroke horizontal U+7850
         <<"???" // 11 strokes, 1st stroke horizontal U+96EA
         <<"???" // 11 strokes, NB#279399
         <<"???" // 12 strokes, 1st stroke down to left U+50A2
         <<"???" // dong4, 12 strokes, 1st stroke horizontal, traditional of ??? U+68DF
         <<"???" // U+3E83
         <<"???" // dong3, 12 strokes (CLDR trunk 20110909: 13 strokes, seems to count kusakanmuri as 4) U+8463
         <<"???" // 13 strokes, 1st stroke down to left U+4E82
         <<"???" // dong4, 13 strokes (Japanese only!) U+50CD
         <<"???" // 13 strokes, 1st stroke vertical, 2nd stroke bending U+7F6A
         <<"???" // 13 strokes, NB#279399
         <<"???" // chi2, 13 strokes, 1st stroke vertical, 2nd stroke horizontal, kHanyuPinlu chi2    U+99B3 kBigFive B9A3 kGB0    - kRSUnicode 187.3
         <<"???" // 14 strokes, 1st stroke down to left U+50E7
         <<"???" // 14 strokes, 1st stroke vertical U+873B
         <<"???" // dong1, 14 strokes, 1st stroke vertical U+8740
         <<"???" // 14 strokes, NB#279399
         <<"???" // 15 strokes, 1st stroke down to left U+5104
         <<"???" // liu2; 15 strokes, 1st stroke bending, traditional of ??? U+5289
         <<"??? Lucy" // liu2; traditional
         <<"??????" // liu2 li4; traditional
         <<"?????????" // liu2 li4 jun1; traditional
         <<"???Lucy" // liu2; traditional
         <<"???" // dong3, 15 strokes (CLDR trunk 20110909: 17 strokes. If kusakanmuri counts as 4, then 16, but why 17?) U+61C2
         <<"???" // 15 strokes, 1st stroke vertical U+8759
         <<"???" // 16 U+5112
         <<"???" // 16 U+FA6A
         <<"???" // 17 U+512A
         <<"???" // 17 U+66D9
         <<"???" // 18 strokes, 1st stroke U+53E2
         <<"???" // 18 strokes, 1st stroke down to left U+9BB9
         <<"???" // dong1, 18 strokes, 1st stroke horizontal U+9F15
         <<"???" // 19 strokes, 1st stroke horizontal U+52F8
         <<"???" // 19 strokes, 1st stroke vertical U+56A5
         <<"???" // 19 strokes (when kusakanmuri counts as 4, else 18), 1st stroke horizontal, kHanyuPinlu heng2 U+8605 kBigFive F4C1 kGB0 6231 kRSUnicode 140.16
         <<"???" // dong1, 19 strokes, 1st stroke horizontal, traditional of ??? U+9D87
         <<"???" // U+4D77
         <<"???" // 20 strokes, 1st stroke vertical
         <<"???" // 21
         <<"???" // 22
         <<"???" // 23
         <<"???" // 24
         <<"???" // 25 last official bucket
         <<"???" // 25
         <<"???" // 27
         <<"???" // 29
         <<"???" // 30
         <<"A"<<"john"<<"John"<<"saki"<<"Saki"
         <<"???"
            );
        QStringList strokeExpectedBuckets =
        (QStringList()
         <<"???"<<"???"
         <<"0"
         <<"0"
         <<"1"
         <<"1"
         <<"2"
         <<"2"
         <<"???"
         <<"???"
         <<"1???"
         <<"1???"
         <<"2???"
         <<"2???"
         <<"3???"
         <<"3???"
         <<"4???"
         <<"4???"
         <<"5???"
         <<"5???"
         <<"5???"
         <<"5???"
         <<"5???"
         <<"5???"
         <<"5???"
         <<"6???"
         <<"6???"
         <<"6???"
         <<"6???"
         <<"6???"
         <<"6???"
         <<"6???"
         <<"6???"
         <<"6???"
         <<"6???"
         <<"7???"
         <<"7???"
         <<"7???"
         <<"7???"
         <<"7???"
         <<"7???"
         <<"7???"
         <<"7???"
         <<"7???"
         <<"8???"
         <<"8???"
         <<"8???"
         <<"8???"
         <<"8???"
         <<"8???"
         <<"8???"
         <<"8???"
         <<"9???"
         <<"9???"
         <<"9???"
         <<"9???"
         <<"9???"
         <<"9???"
         <<"9???"
         <<"9???"
         <<"9???"
         <<"9???"
         <<"9???"
         <<"9???"
         <<"9???"
         <<"9???"
         <<"10???"
         <<"10???"
         <<"10???"
         <<"10???"
         <<"10???"
         <<"11???"
         <<"11???"
         <<"11???"
         <<"11???"
         <<"11???"
         <<"11???"
         <<"11???"
         <<"11???"
         <<"12???"
         <<"12???"
         <<"12???"
         <<"12???"
         <<"13???"
         <<"13???"
         <<"13???"
         <<"13???"
         <<"13???"
         <<"14???"
         <<"14???"
         <<"14???"
         <<"14???"
         <<"15???"
         <<"15???"
         <<"15???"
         <<"15???"
         <<"15???"
         <<"15???"
         <<"15???"
         <<"15???"
         <<"16???"
         <<"16???"
         <<"17???"
         <<"17???"
         <<"18???"
         <<"18???"
         <<"18???"
         <<"19???"
         <<"19???"
         <<"19???"
         <<"19???"
         <<"19???"
         <<"20???"
         <<"21???"
         <<"21???"
         <<"22???"
         <<"24???"
         <<"25???"
         <<"25???"
         <<"27???"
         <<"29???"
         <<"30???"
         <<"A"<<"J"<<"J"<<"S"<<"S"
         <<"???"
            );
    QStringList pinyinStringsSorted =
        (QStringList()
         <<"???"<<"???"
         <<"???"
         <<"???" // a1, 8 strokes, 1st stroke bending, kHanyuPinlu a1 U+963F kBigFive AAFC kGB0 1602 kRSUnicode 170.5
         <<"???" // b??
         <<"???" // b??
         <<"???" // b??
         <<"???" // b??
         <<"???" // ba (b?? ba p?? according to http://www.zdic.net/zd/zi/ZdicE7ZdicBDZdicB7.htm !)
         <<"???" // b?? (not "bo")
         <<"???" // ch??ng, http://unicode.org/cldr/trac/browser/trunk/common/transforms/Han-Latin-Names.xml
         <<"???" // chi2, 6 strokes, 1st stroke bending, kHanyuPinlu chi2 U+9A70 kBigFive - kGB0 1959 kRSUnicode 187'.3
         <<"???" // chi2, 13 strokes, 1st stroke vertical, kHanyuPinlu chi2    U+99B3 kBigFive B9A3 kGB0    - kRSUnicode 187.3
         <<"???" // dong1, 5 strokes, 1st stroke horizontal, simplified of ??? U+4E1C
         <<"???" // dong1, 5 strokes, 1st stroke down to left U+51AC
         <<"???" // dong1, 8 strokes, 1st stroke vertical U+549A
         <<"???" // dong1, 8 strokes, 1st stroke vertical, simplified of ??? U+5CBD kRSUnicode 46.5
         <<"???" // dong1, 8 strokes, 1st stroke horizontal, traditional of ??? U+6771 kRSUnicode 75.4
         <<"???" // dong1, 8 strokes, 1st stroke horizontal U+82F3 kRSUnicode 140.5
         <<"???" // dong1, 9 strokes, 1st stroke down to left kRSUnicode 84.5
         <<"???" // dong1, 10 strokes, 1st stroke horizontal, simplified of ??? U+9E2B kRSUnicode 196'.5
         <<"???" // dong1, dong4 (libicu44 sorts as dong4, libicu46 as dong1), 11 strokes, 1st stroke vertical, traditional of ??? U+5D20 kRSUnicode 46.8
         <<"???" // dong1, 11 strokes U+6DB7 kRSUnicode 85.8
         <<"???" // dong1, 14 strokes kRSUnicode 142.8
         <<"???" // dong1, 18 strokes kRSUnicode 207.5
         <<"???" // dong1, 19 strokes, 1st stroke horizontal, traditional of ??? kRSUnicode 196.8
         <<"???" // dong3, 12 strokes kRSUnicode 140.9
         <<"???" // dong3, 15 strokes kRSUnicode 61.13
         <<"???" // dong4, 6 strokes, 1st stroke horizontal, simplified of ??? kRSUnicode 19.4
         <<"???" // dong4, 7 strokes, 1st stroke down to right, simplified of ??? kRSUnicode 15.5
         <<"???" // dong4, 8 strokes kRSUnicode 9.6
         <<"???" // dong4, 9 strokes, 1st stroke vertical U+606B kRSUnicode 61.6
         <<"???" // dong4, 9 strokes, 1st stroke horizontal, simplified of ??? U+680B kRSUnicode 75.5
         <<"???" // dong4, 9 strokes, 1st stroke down to right U+6D1E kRSUnicode 85.6
         <<"???" // dong4, 10 strokes, 1st stroke down to right, traditional of ??? U+51CD kRSUnicode 15.8
         <<"???" // dong4, 10 strokes, 1st stroke down to left U+80F4 kRSUnicode 130.6
         <<"???" // dong4, 11 strokes, 1st stroke down to left, traditional of ??? U+52D5 kRSUnicode 19.9
         <<"???" // dong4, 11 strokes, 1st stroke horizontal U+7850 kRSUnicode 112.6
         <<"???" // dong4, 12 strokes, 1st stroke horizontal, traditional of ??? kRSUnicode 75.8
         <<"???" // dong4, 13 strokes (Japanese only!) kRSUnicode 9.11
         <<"???" // g??, http://unicode.org/cldr/trac/browser/trunk/common/transforms/Han-Latin-Names.xml
         <<"???" // quia3??? ??? strokes, 1st stroke vertical, (Japanese only! variant of ???) U+5CE0 kBigFive - kGB0 - kRSUnicode 46.6
         <<"???" // 20 strokes, 1st stroke horizontal, kHanyuPinlu heng2 U+8605 kBigFive F4C1 kGB0 6231 kRSUnicode 140.16
         <<"????????????"
         <<"????????????"
         <<"???" // j??a, http://unicode.org/cldr/trac/browser/trunk/common/transforms/Han-Latin-Names.xml
         <<"???" // j??a, http://unicode.org/cldr/trac/browser/trunk/common/transforms/Han-Latin-Names.xml
         <<"???" // ji??o 5 strokes, 1st stroke vertical, kHanyuPinlu jiao4 U+53EB kBigFive A573 kGB0 2948 kRSUnicode 30.2
         <<"???" // j??n, http://unicode.org/cldr/trac/browser/trunk/common/transforms/Han-Latin-Names.xml
         <<"???" // j??n 7 strokes, 1st stroke bending, kHanyuPinlu jun1 U+541B kBigFive A767 kGB0 3093 kRSUnicode 30.4
         <<"???" // 5 strokes, 1st stroke vertical, kHanyuPinlu ka3 U+5361 kBigFive A564 kGB0 3108 kRSUnicode 25.3
         <<"???" // 7 strokes, 1st stroke horizontal, kHanyuPinlu li5 U+674E kBigFive A7F5 kGB0 3278 kRSUnicode 75.3
         <<"???" // li?? 6 strokes, 1st stroke vertical, kMandarin liu2 U+5218 kBigFive - kGB0 3385 kRSUnicode 18.4
         <<"??? Lucy" //li??
         <<"??????" //li?? l??
         <<"?????????" //li?? l?? j??n
         <<"???Lucy" //li??
         <<"???" //li??; 15 strokes, 1st stroke bending, traditional of ??? kRSUnicode 18.13
         <<"??? Lucy" //li?? ; traditional
         <<"??????" // li?? l??; traditional
         <<"?????????" // li?? l?? j??n; traditional
         <<"???Lucy" //li?? ; traditional
         <<"???" // li??; 9 strokes, 1st stroke horizontal, kHanyuPinlu liu3 U+67F3 kBigFive AC68 kGB0 3388 kRSUnicode 75.5"
         <<"??????" // li?? d??ng; 2nd character 5 strokes, initial stroke horizontal, 2nd character U+4E1C
         <<"??????" // li?? d??ng; 2nd character 5 strokes, initial stroke down to left, 2nd character U+51AC
         <<"??????" // li?? d??ng; 2nd character 8 strokes, initial stroke vertical, 2nd character U+549A
         <<"??????" // li?? d??ng; 2nd character traditional, 8 strokes, initial stroke horizontal, U+6771
         <<"?????????" // li?? l?? j??n; 3rd character 5 strokes, initial stroke vertical, 3rd character U+519B
         <<"?????????" // li?? l?? j??n; 3rd character traditional, 9 strokes, initial stroke vertical, 3rd character U+8ECD
         <<"???" // kHanyuPinlu ning2   U+5B81 kBigFive C972 kGB0 3694 kRSUnicode 40.2"
         <<"????????????"
         <<"????????????"
         <<"???" // sh??n, http://unicode.org/cldr/trac/browser/trunk/common/transforms/Han-Latin-Names.xml
         <<"???" // sh??, http://unicode.org/cldr/trac/browser/trunk/common/transforms/Han-Latin-Names.xml
         <<"???" // 9 strokes, 1st stroke horizontal, kHanyuPinlu wei1 U+5A01 kBigFive ABC2 kGB0 4594 kRSUnicode 38.6"
         <<"???" // x??n, http://unicode.org/cldr/trac/browser/trunk/common/transforms/Han-Latin-Names.xml
         <<"???" // z??ng, http://unicode.org/cldr/trac/browser/trunk/common/transforms/Han-Latin-Names.xml
         <<"???" // 7 strokes, 1st stroke bending, kHanyuPinlu zhang1  U+5F20 kBigFive    - kGB0 5337 kRSUnicode 57.4"
         <<"???" // 11 strokes, 1st stroke bending, kHanyuPinlu zhang1  U+5F35 kBigFive B169 kGB0    - kRSUnicode 57.8"
         <<"??"
         <<"A"
         <<"b??"
         <<"b??"
         <<"b??"
         <<"b??"
         <<"ba"
         <<"ji??o"
         <<"john"
         <<"John"
         <<"j??n"
         <<"saki"
         <<"Saki"
         <<"??" // Greek
         <<"??" // Greek
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"??" // Cyrillic
         <<"??" // Cyrillic
         <<"???"
         <<"???"
         <<"???"
         <<"????" // 5 strokes, 1st stroke vertical, kHanyuPinlu - U+20BA9 kBigFive - kGB0- kRSUnicode 30.2"
            );
    QStringList pinyinExpectedBuckets =
        (QStringList()
         <<"???"<<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"A"
         <<"A"
         <<"B"
         <<"B"
         <<"B"
         <<"B"
         <<"B"
         <<"J"
         <<"J"
         <<"J"
         <<"J"
         <<"S"
         <<"S"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"???"
         <<"???"
         <<"???"
         <<"????"
            );
    QStringList pinyinSearchStringsSorted =
        (QStringList()
         <<"???"<<"???"
         <<"??"
         <<"A"
         <<"???"
         <<"???" // a1, 8 strokes, 1st stroke bending, kHanyuPinlu a1 U+963F kBigFive AAFC kGB0 1602 kRSUnicode 170.5
         <<"b??"
         <<"b??"
         <<"b??"
         <<"b??"
         <<"ba"
         <<"???" // b??
         <<"???" // b??
         <<"???" // b??
         <<"???" // b??
         <<"???" // ba (b?? ba p?? according to http://www.zdic.net/zd/zi/ZdicE7ZdicBDZdicB7.htm !)
         <<"???" // chi2, 6 strokes, 1st stroke bending, kHanyuPinlu chi2 U+9A70 kBigFive - kGB0 1959 kRSUnicode 187'.3
         <<"???" // chi2, 13 strokes, 1st stroke vertical, kHanyuPinlu chi2    U+99B3 kBigFive B9A3 kGB0    - kRSUnicode 187.3
         <<"???" // dong1, 5 strokes, 1st stroke horizontal, simplified of ??? U+4E1C
         <<"???" // dong1, 5 strokes, 1st stroke down to left U+51AC
         <<"???" // dong1, 8 strokes, 1st stroke vertical U+549A
         <<"???" // dong1, 8 strokes, 1st stroke vertical, simplified of ??? U+5CBD kRSUnicode 46.5
         <<"???" // dong1, 8 strokes, 1st stroke horizontal, traditional of ??? U+6771 kRSUnicode 75.4
         <<"???" // dong1, 8 strokes, 1st stroke horizontal U+82F3 kRSUnicode 140.5
         <<"???" // dong1, 9 strokes, 1st stroke down to left kRSUnicode 84.5
         <<"???" // dong1, 10 strokes, 1st stroke horizontal, simplified of ??? U+9E2B kRSUnicode 196'.5
         <<"???" // dong1, dong4 (libicu44 sorts as dong4, libicu46 as dong1), 11 strokes, 1st stroke vertical, traditional of ??? U+5D20 kRSUnicode 46.8
         <<"???" // dong1, 11 strokes U+6DB7 kRSUnicode 85.8
         <<"???" // dong1, 14 strokes kRSUnicode 142.8
         <<"???" // dong1, 18 strokes kRSUnicode 207.5
         <<"???" // dong1, 19 strokes, 1st stroke horizontal, traditional of ??? kRSUnicode 196.8
         <<"???" // dong3, 12 strokes kRSUnicode 140.9
         <<"???" // dong3, 15 strokes kRSUnicode 61.13
         <<"???" // dong4, 6 strokes, 1st stroke horizontal, simplified of ??? kRSUnicode 19.4
         <<"???" // dong4, 7 strokes, 1st stroke down to right, simplified of ??? kRSUnicode 15.5
         <<"???" // dong4, 8 strokes kRSUnicode 9.6
         <<"???" // dong4, 9 strokes, 1st stroke vertical U+606B kRSUnicode 61.6
         <<"???" // dong4, 9 strokes, 1st stroke horizontal, simplified of ??? U+680B kRSUnicode 75.5
         <<"???" // dong4, 9 strokes, 1st stroke down to right U+6D1E kRSUnicode 85.6
         <<"???" // dong4, 10 strokes, 1st stroke down to right, traditional of ??? U+51CD kRSUnicode 15.8
         <<"???" // dong4, 10 strokes, 1st stroke down to left U+80F4 kRSUnicode 130.6
         <<"???" // dong4, 11 strokes, 1st stroke down to left, traditional of ??? U+52D5 kRSUnicode 19.9
         <<"???" // dong4, 11 strokes, 1st stroke horizontal U+7850 kRSUnicode 112.6
         <<"???" // dong4, 12 strokes, 1st stroke horizontal, traditional of ??? kRSUnicode 75.8
         <<"???" // dong4, 13 strokes (Japanese only!) kRSUnicode 9.11
         <<"???" // quia3??? ??? strokes, 1st stroke vertical, (Japanese only! variant of ???) U+5CE0 kBigFive - kGB0 - kRSUnicode 46.6
         <<"???" // 20 strokes, 1st stroke horizontal, kHanyuPinlu heng2 U+8605 kBigFive F4C1 kGB0 6231 kRSUnicode 140.16
         <<"ji??o"
         <<"???" // ji??o 5 strokes, 1st stroke vertical, kHanyuPinlu jiao4 U+53EB kBigFive A573 kGB0 2948 kRSUnicode 30.2
         <<"john"
         <<"????????????"
         <<"John"
         <<"????????????"
         <<"j??n"
         <<"???" // j??n 7 strokes, 1st stroke bending, kHanyuPinlu jun1 U+541B kBigFive A767 kGB0 3093 kRSUnicode 30.4
         <<"???" // 5 strokes, 1st stroke vertical, kHanyuPinlu ka3 U+5361 kBigFive A564 kGB0 3108 kRSUnicode 25.3
         <<"???" // 7 strokes, 1st stroke horizontal, kHanyuPinlu li5 U+674E kBigFive A7F5 kGB0 3278 kRSUnicode 75.3
         <<"???" // li?? 6 strokes, 1st stroke vertical, kMandarin liu2 U+5218 kBigFive - kGB0 3385 kRSUnicode 18.4
         <<"???" //li??; 15 strokes, 1st stroke bending, traditional of ??? kRSUnicode 18.13
         <<"???" // li??; 9 strokes, 1st stroke horizontal, kHanyuPinlu liu3 U+67F3 kBigFive AC68 kGB0 3388 kRSUnicode 75.5"
         <<"??? Lucy" //li??
         <<"??? Lucy" //li?? ; traditional
         <<"??????" // li?? d??ng; 2nd character 5 strokes, initial stroke horizontal, 2nd character U+4E1C
         <<"??????" // li?? d??ng; 2nd character 5 strokes, initial stroke down to left, 2nd character U+51AC
         <<"??????" // li?? d??ng; 2nd character 8 strokes, initial stroke vertical, 2nd character U+549A
         <<"??????" // li?? d??ng; 2nd character traditional, 8 strokes, initial stroke horizontal, U+6771
         <<"??????" //li?? l??
         <<"??????" // li?? l??; traditional
         <<"?????????" //li?? l?? j??n
         <<"?????????" // li?? l?? j??n; traditional
         <<"?????????" // li?? l?? j??n; 3rd character 5 strokes, initial stroke vertical, 3rd character U+519B
         <<"?????????" // li?? l?? j??n; 3rd character traditional, 9 strokes, initial stroke vertical, 3rd character U+8ECD
         <<"???Lucy" //li??
         <<"???Lucy" //li?? ; traditional
         <<"???" // kHanyuPinlu ning2   U+5B81 kBigFive C972 kGB0 3694 kRSUnicode 40.2"
         <<"saki"
         <<"????????????"
         <<"Saki"
         <<"????????????"
         <<"???" // 9 strokes, 1st stroke horizontal, kHanyuPinlu wei1 U+5A01 kBigFive ABC2 kGB0 4594 kRSUnicode 38.6"
         <<"???" // 7 strokes, 1st stroke bending, kHanyuPinlu zhang1  U+5F20 kBigFive    - kGB0 5337 kRSUnicode 57.4"
         <<"???" // 11 strokes, 1st stroke bending, kHanyuPinlu zhang1  U+5F35 kBigFive B169 kGB0    - kRSUnicode 57.8"
         <<"??" // Greek
         <<"??" // Greek
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"??" // Cyrillic
         <<"??" // Cyrillic
         <<"???"
         <<"???"
         <<"???"
         <<"????" // 5 strokes, 1st stroke vertical, kHanyuPinlu - U+20BA9 kBigFive - kGB0- kRSUnicode 30.2"
            );
    QStringList pinyinSearchExpectedBuckets =
        (QStringList()
         <<"???"<<"???"
         <<"A"
         <<"A"
         <<"A"
         <<"A"
         <<"B"
         <<"B"
         <<"B"
         <<"B"
         <<"B"
         <<"B"
         <<"B"
         <<"B"
         <<"B"
         <<"B"
         <<"C"
         <<"C"
         <<"D"
         <<"D"
         <<"D"
         <<"D"
         <<"D"
         <<"D"
         <<"D"
         <<"D"
         <<"D"
         <<"D"
         <<"D"
         <<"D"
         <<"D"
         <<"D"
         <<"D"
         <<"D"
         <<"D"
         <<"D"
         <<"D"
         <<"D"
         <<"D"
         <<"D"
         <<"D"
         <<"D"
         <<"D"
         <<"D"
         <<"D"
         <<"G"
         <<"H"
         <<"J"
         <<"J"
         <<"J"
         <<"J"
         <<"J"
         <<"J"
         <<"J"
         <<"J"
         <<"K"
         <<"L"
         <<"L"
         <<"L"
         <<"L"
         <<"L"
         <<"L"
         <<"L"
         <<"L"
         <<"L"
         <<"L"
         <<"L"
         <<"L"
         <<"L"
         <<"L"
         <<"L"
         <<"L"
         <<"L"
         <<"L"
         <<"N"
         <<"S"
         <<"S"
         <<"S"
         <<"S"
         <<"W"
         <<"Z"
         <<"Z"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"??"
         <<"???"
         <<"???"
         <<"???"
         <<"????"
            );
    QStringList zhuyinStringsSorted =
        (QStringList()
         <<"???"<<"???"
         <<"???" // U+3105 BOPOMOFO LETTER B
         <<"???" // U+3106 BOPOMOFO LETTER P
         <<"???" // U+3107 BOPOMOFO LETTER M
         <<"???" // U+3108 BOPOMOFO LETTER F
         <<"???" // U+3109 BOPOMOFO LETTER D
         <<"???" // ????????? dong1, 5 strokes, 1st stroke horizontal, simplified of ??? U+4E1C
         <<"???" // ????????? dong1, 5 strokes, 1st stroke down to left U+51AC
         <<"???" // ????????? dong1, 8 strokes, 1st stroke vertical U+549A
         <<"???" // ????????? dong1, 8 strokes, 1st stroke vertical, simplified of ??? U+5CBD
         <<"???" // ????????? dong1, 8 strokes, 1st stroke horizontal, traditional of ??? U+6771
         <<"???" // ????????? dong1, 8 strokes, 1st stroke horizontal U+82F3
         <<"???" // ????????? dong1, 9 strokes, 1st stroke down to left
         <<"???" // ????????? dong1, 10 strokes, 1st stroke horizontal, simplified of ??? U+9E2B
         <<"???" // ????????? dong1, dong4 (libicu44 sorts as dong4, libicu46 as dong1), 11 strokes, 1st stroke vertical, traditional of ??? U+5D20
         <<"???" // ????????? dong1, 11 strokes U+6DB7
         <<"???" // ????????? dong1, 14 strokes
         <<"???" // ????????? dong1, 18 strokes
         <<"???" // ????????? dong1, 19 strokes, 1st stroke horizontal, traditional of ???
         <<"???" // ??????????? dong3, 12 strokes
         <<"???" // ??????????? dong3, 15 strokes
         <<"???" // ??????????? dong4, 6 strokes, 1st stroke horizontal, simplified of ???
         <<"???" // ??????????? dong4, 7 strokes, 1st stroke down to right, simplified of ???
         <<"???" // ??????????? dong4, 8 strokes
         <<"???" // ??????????? dong4, 9 strokes, 1st stroke vertical U+606B
         <<"???" // ??????????? dong4, 9 strokes, 1st stroke horizontal, simplified of ??? U+680B
         <<"???" // ??????????? dong4, 9 strokes, 1st stroke down to right U+6D1E
         <<"???" // ??????????? dong4, 10 strokes, 1st stroke down to right, traditional of ??? U+51CD
         <<"???" // ??????????? dong4, 10 strokes, 1st stroke down to left U+80F4
         <<"???" // ??????????? dong4, 11 strokes, 1st stroke down to left, traditional of ??? U+52D5
         <<"???" // ??????????? dong4, 11 strokes, 1st stroke horizontal U+7850
         <<"???" // ??????????? dong4, 12 strokes, 1st stroke horizontal, traditional of ???
         <<"???" // ??????????? dong4, 13 strokes (Japanese only!)
         <<"???" // U+310A BOPOMOFO LETTER T
         <<"???" // U+310B BOPOMOFO LETTER N
         <<"???" // ??????????? kHanyuPinlu ning2   U+5B81 kBigFive C972 kGB0 3694 kRSUnicode 40.2"
         <<"???" // U+310C BOPOMOFO LETTER L
         <<"???" // ?????? 7 strokes, 1st stroke horizontal, kHanyuPinlu li5 U+674E kBigFive A7F5 kGB0 3278 kRSUnicode 75.3
         <<"???" // ??????????? 6 strokes, 1st stroke vertical, kMandarin liu2 U+5218 kBigFive - kGB0 3385 kRSUnicode 18.4
         <<"??? Lucy" // ??????????? Lucy liu2
         <<"??????" // ??????????????????? liu2 li4
         <<"?????????" // ???????????????????????????? liu2 li4 jun1
         <<"???Lucy" // ???????????Lucy liu2
         <<"???" // ??????????? liu2; 15 strokes, 1st stroke bending, traditional of ???
         <<"??? Lucy" // ??????????? Lucy liu2; traditional
         <<"??????" // ??????????????????? liu2 li4; traditional
         <<"?????????" // ???????????????????????????? liu2 li4 jun1; traditional
         <<"???Lucy" // ???????????Lucy liu2; traditional
         <<"???" // ??????????? 9 strokes, 1st stroke horizontal, kHanyuPinlu liu3 U+67F3 kBigFive AC68 kGB0 3388 kRSUnicode 75.5"
         <<"??????" //  ???????????????????? liu3 dong1; 2nd character 5 strokes, initial stroke horizontal, 2nd character U+4E1C
         <<"??????" //  ???????????????????? liu3 dong1; 2nd character 5 strokes, initial stroke down to left, 2nd character U+51AC
         <<"??????" //  ???????????????????? liu3 dong1; 2nd character 8 strokes, initial stroke vertical, 2nd character U+549A
         <<"??????" //  ???????????????????? liu3 dong1; 2nd character traditional, 8 strokes, initial stroke horizontal, U+6771
         <<"?????????" // ???????????????????????????? liu3 li4 jun1; 3rd character 5 strokes, initial stroke vertical, 3rd character U+519B
         <<"?????????" // ???????????????????????????? liu3 li4 jun1; 3rd character traditional, 9 strokes, initial stroke vertical, 3rd character U+8ECD
         <<"???" // U+310D BOPOMOFO LETTER G
         <<"???" // ??????????? quia3??? ??? strokes, 1st stroke vertical, (Japanese only! variant of ???) U+5CE0 kBigFive - kGB0 - kRSUnicode 46.6
         <<"???" // U+310E BOPOMOFO LETTER K
         <<"???" // ???????? 5 strokes, 1st stroke vertical, kHanyuPinlu ka3 U+5361 kBigFive A564 kGB0 3108 kRSUnicode 25.3
         <<"???" // U+310F BOPOMOFO LETTER H
         <<"???" // ???????? 20 strokes, 1st stroke horizontal, kHanyuPinlu heng2 U+8605 kBigFive F4C1 kGB0 6231 kRSUnicode 140.16
         <<"???" // U+3110 BOPOMOFO LETTER J
         <<"???" // ??????????? 5 strokes, 1st stroke vertical, kHanyuPinlu jiao4 U+53EB kBigFive A573 kGB0 2948 kRSUnicode 30.2
         <<"???" // ????????? 7 strokes, 1st stroke bending, kHanyuPinlu jun1 U+541B kBigFive A767 kGB0 3093 kRSUnicode 30.4
         <<"???" // U+3111 BOPOMOFO LETTER Q
         <<"???" // U+3112 BOPOMOFO LETTER X
         <<"???" // U+3113 BOPOMOFO LETTER ZH
         <<"???" // ?????? 7 strokes, 1st stroke bending, kHanyuPinlu zhang1  U+5F20 kBigFive    - kGB0 5337 kRSUnicode 57.4"
         <<"???" // ?????? 11 strokes, 1st stroke bending, kHanyuPinlu zhang1  U+5F35 kBigFive B169 kGB0    - kRSUnicode 57.8"
         <<"???" // U+3114 BOPOMOFO LETTER CH
         <<"???" // ????? chi2, 6 strokes, 1st stroke bending, kHanyuPinlu chi2 U+9A70 kBigFive - kGB0 1959 kRSUnicode 187'.3
         <<"???" // ????? chi2, 13 strokes, 1st stroke vertical, kHanyuPinlu chi2    U+99B3 kBigFive B9A3 kGB0    - kRSUnicode 187.3
         <<"???" // U+3115 BOPOMOFO LETTER SH
         <<"???" // U+3116 BOPOMOFO LETTER R
         <<"???" // U+3117 BOPOMOFO LETTER Z
         <<"???" // U+3118 BOPOMOFO LETTER C
         <<"???" // U+3119 BOPOMOFO LETTER S
         <<"???" // U+3127 BOPOMOFO LETTER I
         <<"???" // U+3128 BOPOMOFO LETTER U
         <<"???" // ?????? 9 strokes, 1st stroke horizontal, kHanyuPinlu wei1 U+5A01 kBigFive ABC2 kGB0 4594 kRSUnicode 38.6"
         <<"???" // U+3129 BOPOMOFO LETTER IU
         <<"???" // U+311A BOPOMOFO LETTER A
         <<"???" // ??? a1, 8 strokes, 1st stroke bending, kHanyuPinlu a1 U+963F kBigFive AAFC kGB0 1602 kRSUnicode 170.5
         <<"???" // U+311B BOPOMOFO LETTER O
         <<"???" // U+311C BOPOMOFO LETTER E
         <<"???" // U+311D BOPOMOFO LETTER EH
         <<"???" // U+311E BOPOMOFO LETTER AI
         <<"???" // U+311F BOPOMOFO LETTER EI
         <<"???" // U+3120 BOPOMOFO LETTER AU
         <<"???" // U+3121 BOPOMOFO LETTER OU
         <<"???" // U+3122 BOPOMOFO LETTER AN
         <<"???" // U+3123 BOPOMOFO LETTER EN
         <<"???" // U+3124 BOPOMOFO LETTER ANG
         <<"???" // U+3125 BOPOMOFO LETTER ENG
         <<"???" // U+3126 BOPOMOFO LETTER ER
         <<"???" // U+312A BOPOMOFO LETTER V
         <<"???" // U+312B BOPOMOFO LETTER NG
         <<"???" // U+312C BOPOMOFO LETTER GN
         <<"???" // U+312D BOPOMOFO LETTER IH
         <<"A"
         <<"???"
         <<"john"
         <<"????????????"
         <<"John"
         <<"????????????"
         <<"saki"
         <<"????????????"
         <<"Saki"
         <<"????????????"
         <<"???"
         <<"???"
         <<"????" // 5 strokes, 1st stroke vertical, kHanyuPinlu - U+20BA9 kBigFive - kGB0- kRSUnicode 30.2"
            );
    QStringList zhuyinExpectedBuckets =
        (QStringList()
         <<"???"<<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"???"
         <<"A"
         <<"A"
         <<"J"
         <<"J"
         <<"J"
         <<"J"
         <<"S"
         <<"S"
         <<"S"
         <<"S"
         <<"???"
         <<"???"
         <<"????"
            );
    QStringList unihanStringsSorted =
        (QStringList()
         <<"???"<<"???"
         <<"A"
         <<"???"
         <<"john"
         <<"????????????"
         <<"John"
         <<"????????????"
         <<"saki"
         <<"????????????"
         <<"Saki"
         <<"????????????"
         <<"???"
         <<"???" // 1
         <<"???" // kRSUnicode 1.4
         <<"???" // 2
         <<"???" // 3
         <<"???" // 4
         <<"???" // 5
         <<"???" // 6
         <<"???" // 7
         <<"???" // 8
         <<"???" // 9
         <<"???" // kRSUnicode 9.6
         <<"???" // kRSUnicode 9.11
         <<"???" // 10
         <<"???" // 11
         <<"???" // 12
         <<"???" // 13
         <<"???" // 14
         <<"???" // kRSUnicode???14.4
         <<"???" // 15
         <<"???" // kRSUnicode 15.3
         <<"???" // kRSUnicode 15.5
         <<"???" // kRSUnicode 15.8
         <<"???" // 16
         <<"???" // 17
         <<"???" // 18
         <<"???" // kRSUnicode 18.4
         <<"???" // kRSUnicode 18.5
         <<"???" // kRSUnicode 18.13
         <<"???" // 19
         <<"???" // kRSUnicode 19.4
         <<"???" // kRSUnicode 19.9
         <<"???" // 20
         <<"???" // 21
         <<"???" // 22
         <<"???" // 23
         <<"???" // 24
         <<"???" // 25
         <<"???" // kRSUnicode 25.3
         <<"???" // 26
         <<"???" // 27
         <<"???" // 28
         <<"???" // 29
         <<"???" // 30
         <<"???" // kRSUnicode 30.2
         <<"???" // kRSUnicode 30.4
         <<"???" // kRSUnicode 30.5
         <<"???" // 31
         <<"???" // 32
         <<"???" // 33
         <<"???" // 34
         <<"???" // 35
         <<"???" // 36
         <<"???" // 37
         <<"???" // 38
         <<"???" // kRSUnicode 38.6
         <<"???" // 39
         <<"???" // 40
         <<"???" // kRSUnicode 40.2"
         <<"???" // 41
         <<"???" // 42
         <<"???" // 43
         <<"???" // 44
         <<"???" // 45
         <<"???" // 46
         <<"???" // kRSUnicode 46.5
         <<"???" // kRSUnicode 46.6
         <<"???" // kRSUnicode 46.8
         <<"???" // 47
         <<"???" // 48
         <<"???" // 49
         <<"???" // 50
         <<"???" // 51
         <<"???" // 52
         <<"???" // 53
         <<"???" // 54
         <<"???" // 55
         <<"???" // 56
         <<"???" // 57
         <<"???" // kRSUnicode 57.4
         <<"???" // kRSUnicode 57.8"
         <<"???" // 58
         <<"???" // 59
         <<"???" // 60
         <<"???" // 61
         <<"???" // kRSUnicode 61.6
         <<"???" // kRSUnicode 61.13
         <<"???" // 62
         <<"???" // 63
         <<"???" // 64
         <<"???" // 65
         <<"???" // 66
         <<"???" // 67
         <<"???" // 68
         <<"???" // 69
         <<"???" // 70
         <<"???" // 71
         <<"???" // 72
         <<"???" // 73
         <<"???" // 74
         <<"???" // 75
         <<"???" // kRSUnicode 75.3
         <<"???" // kRSUnicode 75.4
         <<"???" // kRSUnicode 75.5"
         <<"???" // kRSUnicode 75.5
         <<"???" // 76
         <<"???" // 77
         <<"???" // 78
         <<"???" // 79
         <<"???" // 80
         <<"???" // 81
         <<"???" // 82
         <<"???" // 83
         <<"???" // 84
         <<"???" // kRSUnicode 84.5
         <<"???" // 85
         <<"???" // kRSUnicode 85.6
         <<"???" // kRSUnicode 85.8
         <<"???" // 86
         <<"???" // 87
         <<"???" // 88
         <<"???" // 89
         <<"???" // 90
         <<"???" // 91
         <<"???" // 92
         <<"???" // 93
         <<"???" // 94
         <<"???" // 95
         <<"???" // 96
         <<"???" // 97
         <<"???" // 98
         <<"???" // 99
         <<"???" // 100
         <<"???" // 101
         <<"???" // 102
         <<"???" // 103
         <<"???" // 104
         <<"???" // 105
         <<"???" // 106
         <<"???" // 107
         <<"???" // 108
         <<"???" // 109
         <<"???" // 110
         <<"???" // 111
         <<"???" // 112
         <<"???" // 112.6
         <<"???" // 113
         <<"???" // 114
         <<"???" // 115
         <<"???" // 116
         <<"???" // 117
         <<"???" // 118
         <<"???" // 119
         <<"???" // 120
         <<"???" // 121
         <<"???" // 122
         <<"???" // 123
         <<"???" // 124
         <<"???" // 125
         <<"???" // 126
         <<"???" // 127
         <<"???" // 128
         <<"???" // 129
         <<"???" // 130
         <<"???" // kRSUnicode 130.6
         <<"???" // 131
         <<"???" // 132
         <<"???" // 133
         <<"???" // 134
         <<"???" // 135
         <<"???" // 136
         <<"???" // 137
         <<"???" // 138
         <<"???" // 139
         <<"???" // 140
         <<"???" // kRSUnicode 140.5
         <<"???" // kRSUnicode 140.9
         <<"???" // kRSUnicode 140.16
         <<"???" // 141
         <<"???" // 142
         <<"???" // kRSUnicode 142.8
         <<"???" // 143
         <<"???" // 144
         <<"???" // 145
         <<"???" // 146
         <<"???" // 147
         <<"???" // 148
         <<"???" // 149
         <<"???" // 150
         <<"???" // 151
         <<"???" // 152
         <<"???" // 153
         <<"???" // 154
         <<"???" // 155
         <<"???" // 156
         <<"???" // 157
         <<"???" // 158
         <<"???" // 159
         <<"???" // kRSUnicode 159.2
         <<"???" // 160
         <<"???" // 161
         <<"???" // 162
         <<"???" // 163
         <<"???" // 164
         <<"???" // 165
         <<"???" // 166
         <<"???" // 167
         <<"???" // 168
         <<"???" // 169
         <<"???" // 170
         <<"???" // kRSUnicode 170.5
         <<"???" // 171
         <<"???" // 172
         <<"???" // 173
         <<"???" // 174
         <<"???" // 175
         <<"???" // 176
         <<"???" // 177
         <<"???" // 178
         <<"???" // 179
         <<"???" // 180
         <<"???" // 181
         <<"???" // 182
         <<"???" // 183
         <<"???" // 184
         <<"???" // 185
         <<"???" // 186
         <<"???" // 187
         <<"???" // kRSUnicode 187.3
         <<"???" // kRSUnicode 187'.3
         <<"???" // 188
         <<"???" // 189
         <<"???" // 190
         <<"???" // 191
         <<"???" // 192
         <<"???" // 193
         <<"???" // 194
         <<"???" // 195
         <<"???" // 196
         <<"???" // kRSUnicode 196.8
         <<"???" // kRSUnicode 196'.5
         <<"???" // 197
         <<"???" // 198
         <<"???" // 199
         <<"???" // 200
         <<"???" // 201
         <<"???" // 202
         <<"???" // 203
         <<"???" // 204
         <<"???" // 205
         <<"???" // 206
         <<"???" // 207
         <<"???" // kRSUnicode 207.5
         <<"???" // 208
         <<"???" // 209
         <<"???" // 210
         <<"???" // 211
         <<"???" // 212
         <<"???" // 213
         <<"???" // 214
         <<"???" // kRSUnicode 214.5
         <<"????" // kRSUnicode 30.2
         <<"????" // kRSUnicode 214.20
            );
    QStringList unihanExpectedBuckets =
        (QStringList()
         <<"???"<<"???"
         <<"A"
         <<"A"
         <<"J"
         <<"J"
         <<"J"
         <<"J"
         <<"S"
         <<"S"
         <<"S"
         <<"S"
         <<"???"
         <<"???" // 1
         <<"???" // 1
         <<"???" // 2
         <<"???" // 3
         <<"???" // 4
         <<"???" // 5
         <<"???" // 6
         <<"???" // 7
         <<"???" // 8
         <<"???" // 9
         <<"???" // 9
         <<"???" // 9
         <<"???" // 10
         <<"???" // 11
         <<"???" // 12
         <<"???" // 13
         <<"???" // 14
         <<"???" // 14
         <<"???" // 15
         <<"???" // 15
         <<"???" // 15
         <<"???" // 15
         <<"???" // 16
         <<"???" // 17
         <<"???" // 18
         <<"???" // 18
         <<"???" // 18
         <<"???" // 18
         <<"???" // 19
         <<"???" // 19
         <<"???" // 19
         <<"???" // 20
         <<"???" // 21
         <<"???" // 22
         <<"???" // 23
         <<"???" // 24
         <<"???" // 25
         <<"???" // 25
         <<"???" // 26
         <<"???" // 27
         <<"???" // 28
         <<"???" // 29
         <<"???" // 30
         <<"???" // 30
         <<"???" // 30
         <<"???" // 30
         <<"???" // 31
         <<"???" // 32
         <<"???" // 33
         <<"???" // 34
         <<"???" // 35
         <<"???" // 36
         <<"???" // 37
         <<"???" // 38
         <<"???" // 38
         <<"???" // 39
         <<"???" // 40
         <<"???" // 40
         <<"???" // 41
         <<"???" // 42
         <<"???" // 43
         <<"???" // 44
         <<"???" // 45
         <<"???" // 46
         <<"???" // 46
         <<"???" // 46
         <<"???" // 46
         <<"???" // 47
         <<"???" // 48
         <<"???" // 49
         <<"???" // 50
         <<"???" // 51
         <<"???" // 52
         <<"???" // 53
         <<"???" // 54
         <<"???" // 55
         <<"???" // 56
         <<"???" // 57
         <<"???" // 57
         <<"???" // 57
         <<"???" // 58
         <<"???" // 59
         <<"???" // 60
         <<"???" // 61
         <<"???" // 61
         <<"???" // 61
         <<"???" // 62
         <<"???" // 63
         <<"???" // 64
         <<"???" // 65
         <<"???" // 66
         <<"???" // 67
         <<"???" // 68
         <<"???" // 69
         <<"???" // 70
         <<"???" // 71
         <<"???" // 72
         <<"???" // 73
         <<"???" // 74
         <<"???" // 75
         <<"???" // 75
         <<"???" // 75
         <<"???" // 75
         <<"???" // 75
         <<"???" // 76
         <<"???" // 77
         <<"???" // 78
         <<"???" // 79
         <<"???" // 80
         <<"???" // 81
         <<"???" // 82
         <<"???" // 83
         <<"???" // 84
         <<"???" // 84
         <<"???" // 85
         <<"???" // 85
         <<"???" // 85
         <<"???" // 86
         <<"???" // 87
         <<"???" // 88
         <<"???" // 89
         <<"???" // 90
         <<"???" // 91
         <<"???" // 92
         <<"???" // 93
         <<"???" // 94
         <<"???" // 95
         <<"???" // 96
         <<"???" // 97
         <<"???" // 98
         <<"???" // 99
         <<"???" // 100
         <<"???" // 101
         <<"???" // 102
         <<"???" // 103
         <<"???" // 104
         <<"???" // 105
         <<"???" // 106
         <<"???" // 107
         <<"???" // 108
         <<"???" // 109
         <<"???" // 110
         <<"???" // 111
         <<"???" // 112
         <<"???" // 112
         <<"???" // 113
         <<"???" // 114
         <<"???" // 115
         <<"???" // 116
         <<"???" // 117
         <<"???" // 118
         <<"???" // 119
         <<"???" // 120
         <<"???" // 121
         <<"???" // 122
         <<"???" // 123
         <<"???" // 124
         <<"???" // 125
         <<"???" // 126
         <<"???" // 127
         <<"???" // 128
         <<"???" // 129
         <<"???" // 130
         <<"???" // 130
         <<"???" // 131
         <<"???" // 132
         <<"???" // 133
         <<"???" // 134
         <<"???" // 135
         <<"???" // 136
         <<"???" // 137
         <<"???" // 138
         <<"???" // 139
         <<"???" // 140
         <<"???" // 140
         <<"???" // 140
         <<"???" // 140
         <<"???" // 141
         <<"???" // 142
         <<"???" // 142
         <<"???" // 143
         <<"???" // 144
         <<"???" // 145
         <<"???" // 146
         <<"???" // 147
         <<"???" // 148
         <<"???" // 149
         <<"???" // 150
         <<"???" // 151
         <<"???" // 152
         <<"???" // 153
         <<"???" // 154
         <<"???" // 155
         <<"???" // 156
         <<"???" // 157
         <<"???" // 158
         <<"???" // 159
         <<"???" // 159
         <<"???" // 160
         <<"???" // 161
         <<"???" // 162
         <<"???" // 163
         <<"???" // 164
         <<"???" // 165
         <<"???" // 166
         <<"???" // 167
         <<"???" // 168
         <<"???" // 169
         <<"???" // 170
         <<"???" // 170
         <<"???" // 171
         <<"???" // 172
         <<"???" // 173
         <<"???" // 174
         <<"???" // 175
         <<"???" // 176
         <<"???" // 177
         <<"???" // 178
         <<"???" // 179
         <<"???" // 180
         <<"???" // 181
         <<"???" // 182
         <<"???" // 183
         <<"???" // 184
         <<"???" // 185
         <<"???" // 186
         <<"???" // 187
         <<"???" // 187
         <<"???" // 187
         <<"???" // 188
         <<"???" // 189
         <<"???" // 190
         <<"???" // 191
         <<"???" // 192
         <<"???" // 193
         <<"???" // 194
         <<"???" // 195
         <<"???" // 196
         <<"???" // 196
         <<"???" // 196
         <<"???" // 197
         <<"???" // 198
         <<"???" // 199
         <<"???" // 200
         <<"???" // 201
         <<"???" // 202
         <<"???" // 203
         <<"???" // 204
         <<"???" // 205
         <<"???" // 206
         <<"???" // 207
         <<"???" // 207
         <<"???" // 208
         <<"???" // 209
         <<"???" // 210
         <<"???" // 211
         <<"???" // 212
         <<"???" // 213
         <<"???" // 214
         <<"???" // 214
         <<"???" // 214
         <<"???" // 214
            );
    QTest::newRow("zh_TW@collation=stroke")
        <<"ja_JP"
        <<"zh_TW@collation=stroke"
        << strokeStringsSorted
        << strokeExpectedBuckets;
    QTest::newRow("zh_CN@collation=stroke")
        <<"ja_JP"
        <<"zh_CN@collation=stroke"
        << strokeStringsSorted
        << strokeExpectedBuckets;
    QTest::newRow("zh_TW")
        <<"ja_JP"
        <<"zh_TW" // collation=stroke is default for zh_TW
        << strokeStringsSorted
        << strokeExpectedBuckets;
    QTest::newRow("zh_MO")
        <<"ja_JP"
        <<"zh_MO" // collation=stroke is default for zh_MO
        << strokeStringsSorted
        << strokeExpectedBuckets;
    QTest::newRow("zh_HK")
        <<"ja_JP"
        <<"zh_HK" // collation=stroke is default for zh_HK
        << strokeStringsSorted
        << strokeExpectedBuckets;
    QTest::newRow("zh_CN@collation=pinyin")
        <<"ja_JP"
        <<"zh_CN@collation=pinyin" // collation=pinyin is default for zh_CN
        << pinyinStringsSorted
        << pinyinExpectedBuckets;
    QTest::newRow("zh_CN")
        <<"ja_JP"
        <<"zh_CN" // collation=pinyin is default for zh_CN
        << pinyinStringsSorted
        << pinyinExpectedBuckets;
    QTest::newRow("zh_CN@collation=pinyinsearch")
        <<"ja_JP"
        <<"zh_CN@collation=pinyinsearch"
        << pinyinSearchStringsSorted
        << pinyinSearchExpectedBuckets;
    QTest::newRow("zh_TW@collation=zhuyin")
        <<"ja_JP"
        <<"zh_TW@collation=zhuyin"
        << zhuyinStringsSorted
        << zhuyinExpectedBuckets;
    QTest::newRow("zh_TW@collation=zhuyin")
        <<"ja_JP"
        <<"zh_CN@collation=zhuyin"
        << zhuyinStringsSorted
        << zhuyinExpectedBuckets;
    QTest::newRow("zh_TW@collation=unihan")
        <<"de_DE"
        <<"zh_TW@collation=unihan"
        << unihanStringsSorted
        << unihanExpectedBuckets;
    QTest::newRow("zh_TW@collation=unihan")
        <<"de_DE"
        <<"zh_CN@collation=unihan"
        << unihanStringsSorted
        << unihanExpectedBuckets;
    QTest::newRow("ja_JP@collation=standard")
        <<"de_DE"
        <<"ja_JP@collation=standard" // collation=standard is default for ja_JP
        << japaneseStringsSorted
        << japaneseExpectedBuckets;
    QTest::newRow("ru_RU")
        <<"ja_JP"
        <<"ru_RU"
        << russianStringsSorted
        << russianExpectedBuckets;
    QTest::newRow("mk_MK")
        <<"ja_JP"
        <<"mk_MK"
        << macedonianStringsSorted
        << macedonianExpectedBuckets;
}

void Ft_Locales::testMLocaleIndexBucket()
{
    QFETCH(QString, localeName);
    QFETCH(QString, lcCollate);
    QFETCH(QStringList, stringsSorted);
    QFETCH(QStringList, expectedBuckets);

    // we need to instantiate a MLocale before dataPaths returns
    // a valid result.
    MLocale locale(localeName);
    QCOMPARE(MLocale::dataPaths(), (QStringList() << "/usr/share/mlocale5/icu"));

    MLocale localeEn("en_US");
    locale.setCategoryLocale(MLocale::MLcCollate, lcCollate);
    QStringList stringsSortedCopy = stringsSorted;
    std::sort (stringsSortedCopy.begin(), stringsSortedCopy.end(),
           localeEn.collator());
    std::sort (stringsSortedCopy.begin(), stringsSortedCopy.end(),
           locale.collator());
#if defined(VERBOSE_OUTPUT)
    QTextStream debugStream(stdout);
    debugStream.setCodec("UTF-8");
    debugStream << " stringsSorted.size():" << stringsSorted.size()
                << " expectedBuckets.size():" << expectedBuckets.size()
                << "\n";
    debugStream.flush();
    debugStream << "\tlcCollate: " << lcCollate
                << "\n\tbuckets\tbuckets\tstrings\tstrings"
                << "\n\texpect \tfound  \torig   \tsorted\n";
    for (int i = 0; i < stringsSorted.size(); ++i)
        debugStream
            << i
            << "\t" << expectedBuckets[i]
            << "\t" << locale.indexBucket(stringsSorted[i])
            << "\t" << stringsSorted[i]
            << "\t" << stringsSortedCopy[i]
            << "\n";
    debugStream.flush();
#endif
    QCOMPARE(stringsSortedCopy, stringsSorted);
    for (int i = 0; i < stringsSorted.size(); ++i) {
#if defined(VERBOSE_OUTPUT)
        if (locale.indexBucket(stringsSorted[i]) != expectedBuckets[i]) {
            debugStream << "i=" << i
                        << " locale.indexBucket(stringsSorted[i])="
                        << locale.indexBucket(stringsSorted[i])
                        << " expectedBuckets[i]="
                        << expectedBuckets[i] << "\n";
            debugStream.flush();
        }
#endif
        QCOMPARE(locale.indexBucket(stringsSorted[i]),
                 expectedBuckets[i]);
    }
}

void Ft_Locales::testDifferentStrengthComparison_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<QString>("lcCollate");
    QTest::addColumn<QString>("string1");
    QTest::addColumn<QString>("string2");
    QTest::addColumn<QList<MLocale::Comparison> >("comparisonExpectedResults");

    // not spotting anything for pinyinsearch in icu. Quite a special cased collation.
#ifdef ALSO_VERIFY_ICU_DOES_ITS_JOB_AS_WE_EXPECT
    QTest::newRow("zh_CN@collation=pinyinsearch")
        <<"ja_JP"
        <<"zh_CN@collation=pinyinsearch"
        <<"???" // li??
        <<"???" // li??
        << (QList<MLocale::Comparison>()
            << MLocale::Equal
            << MLocale::LessThan
            << MLocale::LessThan
            << MLocale::LessThan
            );
    QTest::newRow("zh_CN@collation=pinyinsearch")
        <<"ja_JP"
        <<"zh_CN@collation=pinyinsearch"
        <<"???" // li??
        <<"???" // li??
        << (QList<MLocale::Comparison>()
            << MLocale::Equal
            << MLocale::GreaterThan
            << MLocale::GreaterThan
            << MLocale::GreaterThan
            );
    QTest::newRow("zh_CN@collation=pinyinsearch")
        <<"ja_JP"
        <<"zh_CN@collation=pinyinsearch"
        <<"???" // li??
        <<"???" // li??
        << (QList<MLocale::Comparison>()
            << MLocale::Equal
            << MLocale::LessThan
            << MLocale::LessThan
            << MLocale::LessThan
            );
    QTest::newRow("zh_CN@collation=pinyinsearch")
        <<"ja_JP"
        <<"zh_CN@collation=pinyinsearch"
        <<"???" // li??
        <<"liu"
        << (QList<MLocale::Comparison>()
            << MLocale::Equal
            << MLocale::GreaterThan
            << MLocale::GreaterThan
            << MLocale::GreaterThan
            );
    QTest::newRow("zh_CN@collation=pinyinsearch")
        <<"ja_JP"
        <<"zh_CN@collation=pinyinsearch"
        <<"??????" // li??l??
        <<"liuli"
        << (QList<MLocale::Comparison>()
            << MLocale::Equal
            << MLocale::GreaterThan
            << MLocale::GreaterThan
            << MLocale::GreaterThan
            );
    QTest::newRow("zh_CN@collation=pinyinsearch")
        <<"ja_JP"
        <<"zh_CN@collation=pinyinsearch"
        <<"??????" // li??l??
        <<"liu li"
        << (QList<MLocale::Comparison>()
            << MLocale::GreaterThan
            << MLocale::GreaterThan
            << MLocale::GreaterThan
            << MLocale::GreaterThan
            );
    QTest::newRow("zh_CN@collation=pinyinsearch")
        <<"ja_JP"
        <<"zh_CN@collation=pinyinsearch"
        <<"??? ???" // li??l??
        <<"liuli"
        << (QList<MLocale::Comparison>()
            << MLocale::LessThan
            << MLocale::LessThan
            << MLocale::LessThan
            << MLocale::LessThan
            );
#endif

    QTest::newRow("en_US")
        <<"ja_JP"
        <<"en_US"
        <<"a%20b"
        <<"a20b"
        << (QList<MLocale::Comparison>()
            << MLocale::LessThan
            << MLocale::LessThan
            << MLocale::LessThan
            << MLocale::LessThan
            );
    QTest::newRow("zh_CN")
        <<"ja_JP"
        <<"zh_CN"
        <<"a%20b"
        <<"a20b"
        << (QList<MLocale::Comparison>()
            << MLocale::LessThan
            << MLocale::LessThan
            << MLocale::LessThan
            << MLocale::LessThan
            );
}

void Ft_Locales::testDifferentStrengthComparison()
{
    QFETCH(QString, localeName);
    QFETCH(QString, lcCollate);
    QFETCH(QString, string1);
    QFETCH(QString, string2);
    QFETCH(QList<MLocale::Comparison>, comparisonExpectedResults);

    QCOMPARE(MLocale::dataPaths(), (QStringList() << "/usr/share/mlocale5/icu"));
    MLocale locale(localeName);
    locale.setCategoryLocale(MLocale::MLcCollate, lcCollate);
    MCollator collator = locale.collator();

    QList<MLocale::Comparison> comparisonResults;
#if defined(VERBOSE_OUTPUT)
    QTextStream debugStream(stdout);
    debugStream.setCodec("UTF-8");
    debugStream
        << " lc_collate=" << lcCollate
        << " " << string1 << " " << string2
        << " (strength: result expectedResult) ";
#endif
    for (unsigned i = 0; i <= 3; ++i) {
        switch(i+1) {
        case 1:
            collator.setStrength(MLocale::CollatorStrengthPrimary);
            break;
        case 2:
            collator.setStrength(MLocale::CollatorStrengthSecondary);
            break;
        case 3:
            collator.setStrength(MLocale::CollatorStrengthTertiary);
            break;
        case 4:
            collator.setStrength(MLocale::CollatorStrengthQuaternary);
            break;
        }
        if (collator(string1, string2))
            comparisonResults << MLocale::LessThan;
        else if (collator(string2, string1))
            comparisonResults << MLocale::GreaterThan;
        else
            comparisonResults << MLocale::Equal;
        
#if defined(VERBOSE_OUTPUT)
        debugStream
            << " (" << i+1
            << ": " << comparisonResults[i] << " " << comparisonExpectedResults[i] <<
            ")";
#endif
    }
#if defined(VERBOSE_OUTPUT)
    debugStream << "\n";
    debugStream.flush();
#endif
    QCOMPARE(comparisonResults, comparisonExpectedResults);
    
}

/*
 * To reduce the size of libicu, we customize the locale data included in
 * our package of libicu and include only what needs to be there.
 *
 * This test checks whether our package of libicu still has all the
 * locales which need to be supported to make sure we did not omit too
 * much accidentally.
 *
 */
void Ft_Locales::checkAvailableLocales()
{
    // For the list of locales which need to be supported see, the list
    // in i18n.dox
    QList<QString> requiredLocaleNames;
    requiredLocaleNames << "ar";          // "Arabic"
    requiredLocaleNames << "ar_AE";       // "Arabic (United Arab Emirates)"
    requiredLocaleNames << "ar_BH";       // "Arabic (Bahrain)"
    requiredLocaleNames << "ar_DZ";       // "Arabic (Algeria)"
    requiredLocaleNames << "ar_EG";       // "Arabic (Egypt)"
    requiredLocaleNames << "ar_IQ";       // "Arabic (Iraq)"
    requiredLocaleNames << "ar_JO";       // "Arabic (Jordan)"
    requiredLocaleNames << "ar_KW";       // "Arabic (Kuwait)"
    requiredLocaleNames << "ar_LB";       // "Arabic (Lebanon)"
    requiredLocaleNames << "ar_LY";       // "Arabic (Libya)"
    requiredLocaleNames << "ar_MA";       // "Arabic (Morocco)"
    requiredLocaleNames << "ar_OM";       // "Arabic (Oman)"
    requiredLocaleNames << "ar_QA";       // "Arabic (Qatar)"
    requiredLocaleNames << "ar_SA";       // "Arabic (Saudi Arabia)"
    requiredLocaleNames << "ar_SD";       // "Arabic (Sudan)"
    requiredLocaleNames << "ar_SY";       // "Arabic (Syria)"
    requiredLocaleNames << "ar_TN";       // "Arabic (Tunisia)"
    requiredLocaleNames << "ar_YE";       // "Arabic (Yemen)"
    requiredLocaleNames << "ca";          // "Catalan"
    requiredLocaleNames << "ca_ES";       // "Catalan (Spain)"
    requiredLocaleNames << "cs";          // "Czech
    requiredLocaleNames << "cs_CZ";       // "Czech (Czech Republic)"
    requiredLocaleNames << "da";          // "Danish"
    requiredLocaleNames << "da_DK";       // "Danish (Denmark)"
    requiredLocaleNames << "de";          // "German"
    requiredLocaleNames << "de_AT";       // "German (Austria)"
    requiredLocaleNames << "de_BE";       // "German (Belgium)"
    requiredLocaleNames << "de_CH";       // "German (Switzerland)"
    requiredLocaleNames << "de_DE";       // "German (Germany)"
    requiredLocaleNames << "de_LI";       // "German (Liechtenstein)"
    requiredLocaleNames << "de_LU";       // "German (Luxembourg)"
    requiredLocaleNames << "el";          // "Greek"
    requiredLocaleNames << "el_CY";       // "Greek (Cyprus)"
    requiredLocaleNames << "el_GR";       // "Greek (Greece)"
    requiredLocaleNames << "en";          // "English"
    requiredLocaleNames << "en_AU";       // "English (Australia)"
    requiredLocaleNames << "en_BE";       // "English (Belgium)"
    requiredLocaleNames << "en_BW";       // "English (Botswana)"
    requiredLocaleNames << "en_BZ";       // "English (Belize)"
    requiredLocaleNames << "en_CA";       // "English (Canada)"
    requiredLocaleNames << "en_GB";       // "English (United Kingdom)"
    requiredLocaleNames << "en_HK";       // "English (Hong Kong SAR China)"
    requiredLocaleNames << "en_IE";       // "English (Ireland)"
    requiredLocaleNames << "en_IN";       // "English (India)"
    requiredLocaleNames << "en_JM";       // "English (Jamaica)"
    requiredLocaleNames << "en_MH";       // "English (Marshall Islands)"
    requiredLocaleNames << "en_MT";       // "English (Malta)"
    requiredLocaleNames << "en_NA";       // "English (Namibia)"
    requiredLocaleNames << "en_NZ";       // "English (New Zealand)"
    requiredLocaleNames << "en_PH";       // "English (Philippines)"
    requiredLocaleNames << "en_PK";       // "English (Pakistan)"
    requiredLocaleNames << "en_SG";       // "English (Singapore)"
    requiredLocaleNames << "en_TT";       // "English (Trinidad and Tobago)"
    requiredLocaleNames << "en_US";       // "English (United States)"
    requiredLocaleNames << "en_US_POSIX"; // "English (United States, Computer)"
    requiredLocaleNames << "en_VI";       // "English (U.S. Virgin Islands)"
    requiredLocaleNames << "en_ZA";       // "English (South Africa)"
    requiredLocaleNames << "en_ZW";       // "English (Zimbabwe)"
    requiredLocaleNames << "es";          // "Spanish"
    requiredLocaleNames << "es_AR";       // "Spanish (Argentina)"
    requiredLocaleNames << "es_BO";       // "Spanish (Bolivia)"
    requiredLocaleNames << "es_CL";       // "Spanish (Chile)"
    requiredLocaleNames << "es_CO";       // "Spanish (Colombia)"
    requiredLocaleNames << "es_CR";       // "Spanish (Costa Rica)"
    requiredLocaleNames << "es_DO";       // "Spanish (Dominican Republic)"
    requiredLocaleNames << "es_EC";       // "Spanish (Ecuador)"
    requiredLocaleNames << "es_ES";       // "Spanish (Spain)"
    requiredLocaleNames << "es_GT";       // "Spanish (Guatemala)"
    requiredLocaleNames << "es_HN";       // "Spanish (Honduras)"
    requiredLocaleNames << "es_MX";       // "Spanish (Mexico)"
    requiredLocaleNames << "es_NI";       // "Spanish (Nicaragua)"
    requiredLocaleNames << "es_PA";       // "Spanish (Panama)"
    requiredLocaleNames << "es_PE";       // "Spanish (Peru)"
    requiredLocaleNames << "es_PR";       // "Spanish (Puerto Rico)"
    requiredLocaleNames << "es_PY";       // "Spanish (Paraguay)"
    requiredLocaleNames << "es_SV";       // "Spanish (El Salvador)"
    requiredLocaleNames << "es_US";       // "Spanish (United States)"
    requiredLocaleNames << "es_UY";       // "Spanish (Uruguay)"
    requiredLocaleNames << "es_VE";       // "Spanish (Venezuela)"
    requiredLocaleNames << "et";          // "Estonian"
    requiredLocaleNames << "et_EE";       // "Estonian (Estonia)"
    requiredLocaleNames << "eu";          // "Basque"
    requiredLocaleNames << "eu_ES";       // "Basque (Spain)"
    requiredLocaleNames << "fa";          // "Persian"
    requiredLocaleNames << "fa_AF";       // "Persian (Afghanistan)"
    requiredLocaleNames << "fa_IR";       // "Persian (Iran)"
    requiredLocaleNames << "fi";          // "Finnish"
    requiredLocaleNames << "fi_FI";       // "Finnish (Finland)"
    requiredLocaleNames << "fr";          // "French"
    requiredLocaleNames << "fr_BE";       // "French (Belgium)"
    requiredLocaleNames << "fr_CA";       // "French (Canada)"
    requiredLocaleNames << "fr_CH";       // "French (Switzerland)"
    requiredLocaleNames << "fr_FR";       // "French (France)"
    requiredLocaleNames << "fr_LU";       // "French (Luxembourg)"
    requiredLocaleNames << "fr_MC";       // "French (Monaco)"
    requiredLocaleNames << "fr_SN";       // "French (Senegal)"
    requiredLocaleNames << "gl";          // "Galician"
    requiredLocaleNames << "gl_ES";       // "Galician (Spain)"
    requiredLocaleNames << "he";          // "Hebrew"
    requiredLocaleNames << "he_IL";       // "Hebrew (Israel)"
    requiredLocaleNames << "hi";          // "Hindi"
    requiredLocaleNames << "hi_IN";       // "Hindi (India)"
    requiredLocaleNames << "hu";          // "Hungarian"
    requiredLocaleNames << "hu_HU";       // "Hungarian (Hungary)"
    requiredLocaleNames << "id";          // "Indonesian"
    requiredLocaleNames << "id_ID";       // "Indonesian (Indonesia)"
    requiredLocaleNames << "it";          // "Italian"
    requiredLocaleNames << "it_CH";       // "Italian (Switzerland)"
    requiredLocaleNames << "it_IT";       // "Italian (Italy)"
    requiredLocaleNames << "ja";          // "Japanese"
    requiredLocaleNames << "ja_JP";       // "Japanese (Japan)"
    requiredLocaleNames << "kk";          // "Kazakh"
    requiredLocaleNames << "lt";          // "Lithuanian"
    requiredLocaleNames << "lt_LT";       // "Lithuanian (Lithuania)"
    requiredLocaleNames << "km";          // "Khmer"
    requiredLocaleNames << "km_KH";       // "Khmer (Cambodia)"
    requiredLocaleNames << "ms";          // "Malay"
    requiredLocaleNames << "nb";          // "Norwegian Bokml"
    requiredLocaleNames << "nb_NO";       // "Norwegian Bokml (Norway)"
    requiredLocaleNames << "nl";          // "Dutch"
    requiredLocaleNames << "nl_BE";       // "Dutch (Belgium)"
    requiredLocaleNames << "nl_NL";       // "Dutch (Netherlands)"
    requiredLocaleNames << "nn";          // "Norwegian Nynorsk"
    requiredLocaleNames << "nn_NO";       // "Norwegian Nynorsk (Norway)"
    requiredLocaleNames << "pl";          // "Polish"
    requiredLocaleNames << "pl_PL";       // "Polish (Poland)"
    requiredLocaleNames << "ps";          // "Pashto"
    requiredLocaleNames << "ps_AF";       // "Pashto (Afghanistan)"
    requiredLocaleNames << "pt";          // "Portuguese"
    requiredLocaleNames << "pt_BR";       // "Portuguese (Brazil)"
    requiredLocaleNames << "pt_PT";       // "Portuguese (Portugal)"
    requiredLocaleNames << "ro";          // "Romanian"
    requiredLocaleNames << "ro_MD";       // "Romanian (Moldova)"
    requiredLocaleNames << "ro_RO";       // "Romanian (Romania)"
    requiredLocaleNames << "ru";          // "Russian"
    requiredLocaleNames << "ru_RU";       // "Russian (Russia)"
    requiredLocaleNames << "ru_UA";       // "Russian (Ukraine)"
    requiredLocaleNames << "sr";          // "Serbian"
    requiredLocaleNames << "sr_Latn_ME";  // "Serbian Latn (Montenegro)"
    requiredLocaleNames << "sr_Cyrl_ME";  // "Serbian Cyrl (Montenegro)"
    requiredLocaleNames << "sr_Latn_RS";  // "Serbian Latn (Serbia)"
    requiredLocaleNames << "sr_Cyrl_RS";  // "Serbian Cyrl (Serbia)"
    requiredLocaleNames << "sk";          // "Slovak"
    requiredLocaleNames << "sk_SK";       // "Slovak (Slovakia)"
    requiredLocaleNames << "sl";          // "Slovenian"
    requiredLocaleNames << "sl_SI";       // "Slovenian (Slovenia)"
    requiredLocaleNames << "sv";          // "Swedish"
    requiredLocaleNames << "sv_FI";       // "Swedish (Finland)"
    requiredLocaleNames << "sv_SE";       // "Swedish (Sweden)"
    requiredLocaleNames << "sw_KE";       // "Swahili (Kenya)"
    requiredLocaleNames << "th";          // "Thai"
    requiredLocaleNames << "th_TH";       // "Thai (Thailand)"
    requiredLocaleNames << "tr";          // "Turkish"
    requiredLocaleNames << "tr_TR";       // "Turkish (Turkey)"
    requiredLocaleNames << "uk";          // "Ukrainian"
    requiredLocaleNames << "uk_UA";       // "Ukrainian (Ukraine)"
    requiredLocaleNames << "ur";          // "Urdu"
    requiredLocaleNames << "ur_IN";       // "Urdu (India)"
    requiredLocaleNames << "ur_PK";       // "Urdu (Pakistan)"
    requiredLocaleNames << "vi";          // "Vietnamese"
    requiredLocaleNames << "vi_VN";       // "Vietnamese (Vietnam)"
    requiredLocaleNames << "zh";          // "Chinese"
    requiredLocaleNames << "zh_Hans" ;    // "Chinese (Simplified Han)"
    requiredLocaleNames << "zh_Hans_CN";  // "Chinese (Simplified Han, China)"
    requiredLocaleNames << "zh_Hans_HK";  // "Chinese (Simplified Han, Hong Kong SAR China)"
    requiredLocaleNames << "zh_Hans_MO";  // "Chinese (Simplified Han, Macau SAR China)"
    requiredLocaleNames << "zh_Hans_SG";  // "Chinese (Simplified Han, Singapore)"
    requiredLocaleNames << "zh_Hant";     // "Chinese (Traditional Han)"
    requiredLocaleNames << "zh_Hant_HK";  // "Chinese (Traditional Han, Hong Kong SAR China)"
    requiredLocaleNames << "zh_Hant_MO";  // "Chinese (Traditional Han, Macau SAR China)"
    requiredLocaleNames << "zh_Hant_TW";  // "Chinese (Traditional Han, Taiwan)"

    QList<QString> availableLocaleNames;
    QList<QString> availableDisplayNames;
    int numberOfAvailableLocales = uloc_countAvailable();

    for (int i = 0; i < numberOfAvailableLocales; ++i) {
        int maxresultsize = 100;
        UChar result[maxresultsize];
        UErrorCode status = U_ZERO_ERROR;
        availableLocaleNames << QString::fromUtf8(uloc_getAvailable(i));
        uloc_getDisplayName(availableLocaleNames[i].toUtf8().constData(),
                            "en_US", result, maxresultsize, &status);
        if (U_SUCCESS(status))
            availableDisplayNames << QString::fromUtf16(result);
        else
            availableDisplayNames << QString("What kind of locale is this?");
    }
    // for (int i = 0; i < numberOfAvailableLocales; ++i) {
    //     qDebug() << "available:" << availableLocaleNames[i] << availableDisplayNames[i];
    // }
    foreach(const QString &requiredLocaleName, requiredLocaleNames) {
        // if (availableLocaleNames.contains(requiredLocaleName))
        //     qDebug() << "required and available: "
        //              << requiredLocaleName
        //              << availableDisplayNames[availableLocaleNames.indexOf(requiredLocaleName)];
        // else {
        //     qDebug() << "required but missing: " << requiredLocaleName;
        // }
        QVERIFY2(availableLocaleNames.contains(requiredLocaleName),
                 QString("The following required locale is missing: "
                         + requiredLocaleName).toUtf8().constData());
    }
    // add supported aliases here to check that these work as well:
    QList<QString> supportedLocaleNames = requiredLocaleNames;
    supportedLocaleNames << "no";
    supportedLocaleNames << "no_NO";
    supportedLocaleNames << "es_419";
    // add some variants with different sort rules:
    supportedLocaleNames << "de_DE@collation=phonebook";
    supportedLocaleNames << "zh_CN";
    supportedLocaleNames << "zh_SG";
    supportedLocaleNames << "zh_HK";
    supportedLocaleNames << "zh_MO";
    supportedLocaleNames << "zh_TW";
    supportedLocaleNames << "zh_Hans_TW";
    supportedLocaleNames << "zh_TW@collation=pinyin";
    supportedLocaleNames << "zh_TW@collation=stroke";
    supportedLocaleNames << "zh_CN@collation=pinyin";
    supportedLocaleNames << "zh_CN@collation=stroke";
    supportedLocaleNames << "ko_KR";
    supportedLocaleNames << "kk_KZ";
    bool runFullTest = false;
    if (!runFullTest) {
        supportedLocaleNames.clear();
        supportedLocaleNames << "ar_SA";       // "Arabic (Saudi Arabia)"
        supportedLocaleNames << "ca_ES";       // "Catalan (Spain)"
        supportedLocaleNames << "da_DK";       // "Danish (Denmark)"
        supportedLocaleNames << "de_CH";       // "German (Switzerland)"
        supportedLocaleNames << "de_DE";       // "German (Germany)"
        supportedLocaleNames << "el_GR";       // "Greek (Greece)"
        supportedLocaleNames << "en";          // "English"
        supportedLocaleNames << "en_BE";       // "English (Belgium)"
        supportedLocaleNames << "en_GB";       // "English (United Kingdom)"
        supportedLocaleNames << "en_US";       // "English (United States)"
        supportedLocaleNames << "en_US_POSIX"; // "English (United States, Computer)"
        supportedLocaleNames << "eu_ES";       // "Basque (Spain)"
        supportedLocaleNames << "fa_IR";       // "Persian (Iran)"
        supportedLocaleNames << "fi_FI";       // "Finnish (Finland)"
        supportedLocaleNames << "fr_FR";       // "French (France)"
        supportedLocaleNames << "gl_ES";       // "Galician (Spain)"
        supportedLocaleNames << "he_IL";       // "Hebrew (Israel)"
        supportedLocaleNames << "hi_IN";       // "Hindi (India)"
        supportedLocaleNames << "id_ID";       // "Indonesian (Indonesia)"
        supportedLocaleNames << "it_IT";       // "Italian (Italy)"
        supportedLocaleNames << "ja_JP";       // "Japanese (Japan)"
        supportedLocaleNames << "ko_KR";       // "Korean (Korea)"
        supportedLocaleNames << "ms_MY";       // "Malay (Malaysia)"
        supportedLocaleNames << "nb_NO";       // "Norwegian Bokml (Norway)"
        supportedLocaleNames << "nl_NL";       // "Dutch (Netherlands)"
        supportedLocaleNames << "nn_NO";       // "Norwegian Nynorsk (Norway)"
        supportedLocaleNames << "pt_BR";       // "Portuguese (Brazil)"
        supportedLocaleNames << "pt_PT";       // "Portuguese (Portugal)"
        supportedLocaleNames << "ru_RU";       // "Russian (Russia)"
        supportedLocaleNames << "sv_SE";       // "Swedish (Sweden)"
        supportedLocaleNames << "th_TH";       // "Thai (Thailand)"
        supportedLocaleNames << "tr_TR";       // "Turkish (Turkey)"
        supportedLocaleNames << "uk_UA";       // "Ukrainian (Ukraine)"
        supportedLocaleNames << "ur_PK";       // "Urdu (Pakistan)"
        supportedLocaleNames << "zh_Hans_CN";  // "Chinese (Simplified Han, China)"
        supportedLocaleNames << "zh_Hant_TW";  // "Chinese (Traditional Han, Taiwan)"
        supportedLocaleNames << "no_NO";
        supportedLocaleNames << "es_419";
        supportedLocaleNames << "de_DE@collation=phonebook";
        supportedLocaleNames << "zh_CN@collation=pinyin";
        supportedLocaleNames << "zh_CN@collation=stroke";
    }
    QStringList numberSystemTestLocales;
    foreach(QString supportedLocaleName, supportedLocaleNames) {
        if(supportedLocaleName.startsWith("ar")) {
            numberSystemTestLocales << supportedLocaleName + QLatin1String("@numbers=arab");
            numberSystemTestLocales << supportedLocaleName + QLatin1String("@numbers=latn");
        }
        else if (supportedLocaleName.startsWith("fa")) {
            numberSystemTestLocales << supportedLocaleName + QLatin1String("@numbers=arabext");
            numberSystemTestLocales << supportedLocaleName + QLatin1String("@numbers=latn");
        }
    }
    supportedLocaleNames.append(numberSystemTestLocales);
    // sort the list for easier comparison in the output
    // (i.e. es_419 should be near es, not at the end of the list):
    std::sort(supportedLocaleNames.begin(), supportedLocaleNames.end());
    MCalendar::setSystemTimeZone("GMT+0");
    QDateTime dateTime(QDate(2008, 7, 21), QTime(14, 31, 0, 0), Qt::LocalTime);
    MCalendar gregorianCalendar(MLocale::GregorianCalendar);
    MCalendar islamicCalendar(MLocale::IslamicCalendar);
    gregorianCalendar.setDateTime(dateTime);
    islamicCalendar.setDateTime(dateTime);
    QStringList sortingTestList;
    sortingTestList
        // symbols
        <<"#" <<"/" <<"%"<<"???" <<"???" <<"???"
        // Western Numbers:
        <<"01" <<"02" <<"04" <<"05"<<"06"<<"1"<<"2"<<"4"<<"5"<<"6"<<"11"<<"12"<<"14"<<"15"<<"16"<<"21"<<"22"<<"24"<<"25"<<"26"
        // Eastern Arabic Numbers:
        <<"????" <<"????" <<"????" <<"????"<<"????"<<"??"<<"??"<<"??"<<"??"<<"??"<<"????"<<"????"<<"????"<<"????"<<"????"<<"????"<<"????"<<"????"<<"????"<<"????"
        // Eastern Arabic Numbers, Persian variants:
        <<"????" <<"????"<<"????"<<"??"<<"??"<<"??"<<"????"<<"????"<<"????"<<"????"<<"????"<<"????"
        // Devanagari numbers:
        <<"??????" <<"??????" <<"??????" <<"??????"<<"??????"<<"???"<<"???"<<"???"<<"???"<<"???"<<"??????"<<"??????"<<"??????"<<"??????"<<"??????"<<"??????"<<"??????"<<"??????"<<"??????"<<"??????"
        // Han:
        << "???" << "??????" << "????????????"
        << "???" << "??????" << "?????????"
        << "???" << "??????" << "??????" << "?????????"
        << "???" << "??????" << "??????" << "????????????"
        << "???" << "??????" << "????????????" << "???????????????"
        << "???" << "??????" << "????????????" << "????????????"
        << "???" << "??????" << "????????????" << "????????????"
        << "???" << "??????" << "?????????" << "???????????????"
        << "???" << "??????" << "????????????" << "????????????"
        << "???" << "??????" << "????????????" << "????????????"
        << "???" << "???" << "???" << "???" << "???"
        << "???" << "???" << "???" << "???" << "???" << "???"
        // Cyrillic:
        << "??" << "??" << "??" << "??" << "??" << "??" << "??" << "??" << "??" << "??" << "??"
        // Greek:
        << "??" << "??" << "??" << "??" << "??" << "??"
        // Arabic and Farsi:
        << "??" << "??" << "??" << "??" << "??" << "??" << "??" << "??" << "??"
        << "??" << "??" << "??" << "??" << "??" << "??" << "??" << "??" << "??" << "??"
        << "??" << "??" << "??" << "??" << "??" << "??" << "??" << "??" << "??" << "??" << "??"
        // Hebrew:
        << "??" << "??" << "??" << "??" << "??"
        // Latin:
        << "??" << "??" << "A" << "a" << "??" << "??" << "??" << "??" << "??" << "??"
        << "b" << "ch" << "Ch" << "cH" << "CH" << "h" << "H" << "i" << "I"
        << "c" << "ca" << "cz" << "??" << "??a" << "??c" << "??z"
        << "cote" << "c??te" << "cot??" << "c??t??"
        << "C??TE" << "C??T??" << "COT??" << "COTE"
        << "O" << "o" << "??" << "??" << "??fgh" << "oegh"
        << "d" << "da" << "dz" << "??" << "??a" << "??z"
        << "e" << "ea" << "ez" << "??" << "??a" << "??z"
        << "n" << "na" << "nz" << "??" << "??a" << "??z"
        << "r" << "ra" << "rz" << "??" << "??a" << "??z"
        << "s" << "sa" << "sz" << "??" << "??a" << "??z"
        << "t" << "ta" << "tz" << "??" << "??a" << "??z"
        << "U" << "??" << "??" << "??" << "N" << "??" << "??"
        << "z" << "za" << "zz" << "??" << "??a"<< "??z"
        ;
    QString ft_localesTestOutput = "";
    foreach(QString supportedLocaleName, supportedLocaleNames) {
        MLocale locale(supportedLocaleName);
        QCOMPARE(MLocale::dataPaths(), (QStringList() << "/usr/share/mlocale5/icu"));
        locale.setTimeFormat24h(MLocale::LocaleDefaultTimeFormat24h);
        QCOMPARE(locale.timeFormat24h(), MLocale::LocaleDefaultTimeFormat24h);
        std::sort(sortingTestList.begin(), sortingTestList.end(), locale.collator());
        QString newLinePlusSupportedLocaleName('\n' + supportedLocaleName);
        ft_localesTestOutput
            += supportedLocaleName + "\tLanguage endonym\t"
            + locale.languageEndonym()
            + newLinePlusSupportedLocaleName + "\tCountry endonym\t"
            + locale.countryEndonym()
            + newLinePlusSupportedLocaleName + "\tDecimal point\t"
            + locale.decimalPoint()
            + newLinePlusSupportedLocaleName + "\tFloating Point Number\t"
            + locale.formatNumber(-123456789.01234)
            + newLinePlusSupportedLocaleName + "\tPercent value\t"
            + locale.formatPercent(-12.3456789, 4)
            + newLinePlusSupportedLocaleName + "\tCurrency value\t"
            + locale.formatCurrency(1234.56, "")
            + newLinePlusSupportedLocaleName + "\tNegative Currency value\t"
            + locale.formatCurrency(-1234.56, "");
        for (unsigned i = 0; i < 2; ++i) {
            MLocale::CalendarType calendarType;
            Q_UNUSED(calendarType);
            MCalendar calendar;
            QString calendarTypeString;
            switch (i) {
            case 0:
                calendarType = MLocale::GregorianCalendar;
                calendarTypeString = "Gregorian calendar";
                calendar = gregorianCalendar;
                break;
            case 1:
                calendarType = MLocale::IslamicCalendar;
                calendarTypeString = "Islamic calendar";
                calendar = islamicCalendar;
                break;
            }
            for (unsigned i = 0; i < 2; ++i) {
                MLocale::DateSymbolContext dateSymbolContext;
                QString dateSymbolContextString;
                switch (i) {
                case 0:
                    dateSymbolContext = MLocale::DateSymbolStandalone;
                    dateSymbolContextString = "stand-alone";
                    break;
                case 1:
                    dateSymbolContext = MLocale::DateSymbolFormat;
                    dateSymbolContextString = "format";
                    break;
                }
                for (unsigned j = 0; j < 3; ++j) {
                    MLocale::DateSymbolLength dateSymbolLength;
                    QString dateSymbolLengthString;
                    switch (j) {
                    case 0:
                        dateSymbolLength = MLocale::DateSymbolWide;
                        dateSymbolLengthString = "wide";
                        break;
                    case 1:
                        dateSymbolLength = MLocale::DateSymbolAbbreviated;
                        dateSymbolLengthString = "abbreviated";
                        break;
                    case 2:
                        dateSymbolLength = MLocale::DateSymbolNarrow;
                        dateSymbolLengthString = "narrow";
                        break;
                    }
                    for (int monthNumber = 1; monthNumber <= 12; ++monthNumber) {
                        ft_localesTestOutput
                            += newLinePlusSupportedLocaleName
                            + "\tName of month " + QString("%1").arg(monthNumber, int(2), int(10), QLatin1Char('0'))
                            + " (" + calendarTypeString + "), "
                            + dateSymbolContextString + ", "
                            + dateSymbolLengthString + QLatin1Char('\t')
                            + locale.monthName(calendar, monthNumber, dateSymbolContext, dateSymbolLength);
                    }
                }
            }
        }
        for (unsigned i = 0; i < 2; ++i) {
            MLocale::CalendarType calendarType;
            Q_UNUSED(calendarType);
            MCalendar calendar;
            QString calendarTypeString;
            switch (i) {
            case 0:
                calendarType = MLocale::GregorianCalendar;
                calendarTypeString = "Gregorian calendar";
                calendar = gregorianCalendar;
                break;
            case 1:
                calendarType = MLocale::IslamicCalendar;
                calendarTypeString = "Islamic calendar";
                calendar = islamicCalendar;
                break;
            }
            for (unsigned i = 0; i < 2; ++i) {
                MLocale::DateSymbolContext dateSymbolContext;
                QString dateSymbolContextString;
                switch (i) {
                case 0:
                    dateSymbolContext = MLocale::DateSymbolStandalone;
                    dateSymbolContextString = "stand-alone";
                    break;
                case 1:
                    dateSymbolContext = MLocale::DateSymbolFormat;
                    dateSymbolContextString = "format";
                    break;
                }
                for (unsigned j = 0; j < 3; ++j) {
                    MLocale::DateSymbolLength dateSymbolLength;
                    QString dateSymbolLengthString;
                    switch (j) {
                    case 0:
                        dateSymbolLength = MLocale::DateSymbolWide;
                        dateSymbolLengthString = "wide";
                        break;
                    case 1:
                        dateSymbolLength = MLocale::DateSymbolAbbreviated;
                        dateSymbolLengthString = "abbreviated";
                        break;
                    case 2:
                        dateSymbolLength = MLocale::DateSymbolNarrow;
                        dateSymbolLengthString = "narrow";
                        break;
                    }
                    for (int weekDayNumber = 1; weekDayNumber <= 7; ++weekDayNumber) {
                        ft_localesTestOutput
                            += newLinePlusSupportedLocaleName
                            + "\tName of weekday " + QString("%1").arg(weekDayNumber, int(2), int(10), QLatin1Char('0'))
                            + " (" + calendarTypeString + "), "
                            + dateSymbolContextString + ", "
                            + dateSymbolLengthString + QLatin1Char('\t')
                            + locale.weekdayName(calendar, weekDayNumber, dateSymbolContext, dateSymbolLength);
                    }
                }
            }
        }
        ft_localesTestOutput
            += newLinePlusSupportedLocaleName + "\tCollation sample\t"
            + sortingTestList.join(" ")
            + newLinePlusSupportedLocaleName + "\tExemplar characters index\t"
            + '[' + locale.exemplarCharactersIndex().join(" ") + ']'
            + newLinePlusSupportedLocaleName + "\tScripts used\t"
            + locale.localeScripts().join(" ")
            + newLinePlusSupportedLocaleName + "\t%r (locale specific 12 hour ???TimeShort???)\t"
            + locale.formatDateTime(gregorianCalendar, "%r")
            + newLinePlusSupportedLocaleName + "\t%R (locale specific 24 hour ???TimeShort???)\t"
            + locale.formatDateTime(gregorianCalendar, "%R");
        for (unsigned i = 0; i < 2; ++i) {
            MLocale::CalendarType calendarType;
            MCalendar calendar;
            QString calendarTypeString;
            switch (i) {
            case 0:
                calendarType = MLocale::GregorianCalendar;
                calendarTypeString = "Gregorian calendar";
                calendar = gregorianCalendar;
                break;
            case 1:
                calendarType = MLocale::IslamicCalendar;
                calendarTypeString = "Islamic calendar";
                calendar = islamicCalendar;
                break;
            }
            for (unsigned dateType = MLocale::DateNone;
                 dateType <= MLocale::DateFull;
                 ++dateType) {
                QString dateTypeString;
                switch (dateType) {
                case 0:
                    dateTypeString = "dateNone";
                    break;
                case 1:
                    dateTypeString = "dateShort";
                    break;
                case 2:
                    dateTypeString = "dateMedium";
                    break;
                case 3:
                    dateTypeString = "dateLong";
                    break;
                case 4:
                    dateTypeString = "dateFull";
                    break;
                }
                for (unsigned timeType = MLocale::TimeNone;
                     timeType <= MLocale::TimeFull;
                     ++timeType) {
                    QString timeTypeString;
                    switch (timeType) {
                    case 0:
                        timeTypeString = "timeNone";
                        break;
                    case 1:
                        timeTypeString = "timeShort";
                        break;
                    case 2:
                        timeTypeString = "timeMedium";
                        break;
                    case 3:
                        timeTypeString = "timeLong";
                        break;
                    case 4:
                        timeTypeString = "timeFull";
                        break;
                    }
                    if (dateType == MLocale::DateNone && timeType == MLocale::TimeNone)
                        continue;
                    unsigned maxMixing;
                    if (dateType == MLocale::DateFull && timeType == MLocale::TimeFull)
                        maxMixing = 3;
                    else
                        maxMixing = 0;
                    for (unsigned mixing = 0; mixing <= maxMixing; ++mixing) {
                        QString messageLocale;
                        switch (mixing) {
                        case 0:
                        default:
                            messageLocale = "";
                            break;
                        case 1:
                            messageLocale = "fi_FI";
                            break;
                        case 2:
                            messageLocale = "de_DE";
                            break;
                        case 3:
                            messageLocale = "en_US";
                            break;
                        }
                        locale.setCategoryLocale(MLocale::MLcTime, locale.name());
                        locale.setCategoryLocale(MLocale::MLcMessages, messageLocale);
                        locale.setCalendarType(calendarType);
                        QCOMPARE(locale.calendarType(), calendarType);
                        for (int hourMode = 0; hourMode < 2; ++hourMode) {
                            MLocale::TimeFormat24h timeFormat24h = MLocale::LocaleDefaultTimeFormat24h;
                            locale.setTimeFormat24h(timeFormat24h);
                            QCOMPARE(locale.timeFormat24h(), timeFormat24h);
                            QString icuFormatStringDefault
                                = locale.icuFormatString(static_cast<MLocale::DateType>(dateType),
                                                         static_cast<MLocale::TimeType>(timeType),
                                                         calendarType);
                            QString formattedDateTimeDefault
                                = locale.formatDateTime(calendar,
                                                        static_cast<MLocale::DateType>(dateType),
                                                        static_cast<MLocale::TimeType>(timeType));
                            QString icuFormatStringForced;
                            QString formattedDateTimeForced;
                            QString timeFormat24hString;
                            switch (hourMode) {
                            case 0:
                                timeFormat24h = MLocale::TwelveHourTimeFormat24h;
                                locale.setTimeFormat24h(timeFormat24h);
                                QCOMPARE(locale.timeFormat24h(), timeFormat24h);
                                icuFormatStringForced
                                    = locale.icuFormatString(static_cast<MLocale::DateType>(dateType),
                                                             static_cast<MLocale::TimeType>(timeType),
                                                             calendarType);
                                formattedDateTimeForced
                                    = locale.formatDateTime(calendar,
                                                            static_cast<MLocale::DateType>(dateType),
                                                            static_cast<MLocale::TimeType>(timeType));
                                if (locale.defaultTimeFormat24h() == MLocale::TwelveHourTimeFormat24h) {
                                    QCOMPARE(icuFormatStringDefault, icuFormatStringForced);
                                    timeFormat24hString = "24/12 hour mode: 12 (default)";
                                }
                                else {
                                    timeFormat24hString = "24/12 hour mode: 12          ";
                                }
                                break;
                            case 1:
                                timeFormat24h = MLocale::TwentyFourHourTimeFormat24h;
                                locale.setTimeFormat24h(timeFormat24h);
                                QCOMPARE(locale.timeFormat24h(), timeFormat24h);
                                icuFormatStringForced
                                    = locale.icuFormatString(static_cast<MLocale::DateType>(dateType),
                                                             static_cast<MLocale::TimeType>(timeType),
                                                             calendarType);
                                formattedDateTimeForced
                                    = locale.formatDateTime(calendar,
                                                            static_cast<MLocale::DateType>(dateType),
                                                            static_cast<MLocale::TimeType>(timeType));
                                if (locale.defaultTimeFormat24h() == MLocale::TwentyFourHourTimeFormat24h) {
                                    QCOMPARE(icuFormatStringDefault, icuFormatStringForced);
                                    timeFormat24hString = "24/12 hour mode: 24 (default)";
                                }
                                else {
                                    timeFormat24hString = "24/12 hour mode: 24          ";
                                }
                                break;
                            }
                            ft_localesTestOutput +=
                                newLinePlusSupportedLocaleName;
                            if(mixing > 0)
                                ft_localesTestOutput += QLatin1String(" & ") + messageLocale;
                            ft_localesTestOutput +=
                                QLatin1Char('\t')
                                + dateTypeString + ", " + timeTypeString + ", "
                                + calendarTypeString + ", " + timeFormat24hString
                                + QLatin1Char('\t')
                                + icuFormatStringForced
                                + " -> "
                                + formattedDateTimeForced;
                        }
                    }
                }
            }
        }
        ft_localesTestOutput += '\n';
    }
    QString ft_localesTestOutputFileName =
        "/tmp/ft_locales-test-output.txt";
    QFile ft_localesTestOutputFile(ft_localesTestOutputFileName);
    if (!ft_localesTestOutputFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
        QFAIL(qPrintable("could not open file " + ft_localesTestOutputFileName));
    int bytesWritten = ft_localesTestOutputFile.write(ft_localesTestOutput.toUtf8().constData());
    if (bytesWritten == -1)
        QFAIL(qPrintable("could not write to file" + ft_localesTestOutputFileName));
    QCOMPARE(uint(bytesWritten), qstrlen(ft_localesTestOutput.toUtf8().constData()));
    ft_localesTestOutputFile.close();

    QString ft_localesTestInputFileName;
    if (runFullTest)
        ft_localesTestInputFileName =
            qApp->applicationDirPath() + QDir::separator() + "ft_locales-test-input-full-icu-" + U_ICU_VERSION_SHORT + ".txt";
    else
        ft_localesTestInputFileName =
            qApp->applicationDirPath() + QDir::separator() + "ft_locales-test-input-icu-" + U_ICU_VERSION_SHORT + ".txt";
    QFile ft_localesTestInputFile(ft_localesTestInputFileName);
    if (!ft_localesTestInputFile.open(QIODevice::ReadOnly))
        QSKIP(qPrintable("could not open file " + ft_localesTestInputFileName));
    QString ft_localesTestInput = QString::fromUtf8(ft_localesTestInputFile.readAll().constData());
    ft_localesTestInputFile.close();
    
    // QTextStream debugStream(stdout);
    // debugStream.setCodec("UTF-8");
    // debugStream << ft_localesTestInput;

    if (ft_localesTestOutput != ft_localesTestInput) {
        QProcess::execute("diff -u " + ft_localesTestInputFileName + ' ' + ft_localesTestOutputFileName);
//        QFAIL(qPrintable("files " + ft_localesTestOutputFileName + " and " + ft_localesTestInputFileName + " differ."));
    }
}

QTEST_MAIN(Ft_Locales);
