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

#ifndef ML10N_MLOCALEABSTRACTCONFIGITEM_H
#define ML10N_MLOCALEABSTRACTCONFIGITEM_H

#include <QString>
#include <QObject>

#include "mlocaleexport.h"

namespace ML10N {

class MLOCALE_EXPORT MLocaleAbstractConfigItem : public QObject
{
Q_OBJECT
  public:
    MLocaleAbstractConfigItem( const QString& key );

    // returns true, if this value is valid,
    // false otherwise. e.g. a non-existing gconf key
    // should retuen false here.
    virtual bool isValid() const = 0;

    // returns the value of this item
    virtual QString value() const = 0;

    // sets the value of this item
    virtual void setValue( const QString& value ) = 0;

  signals:
    void valueChanged( const QString& newValue );
};

}

#endif
