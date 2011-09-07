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

#include "mlocalenullconfigitem.h"

namespace ML10N {

class MLocaleNullConfigItemPrivate {
public:
    bool valid;
    QString key;
    QString value;
};

MLocaleNullConfigItem::MLocaleNullConfigItem( const QString& key ) :
    MLocaleAbstractConfigItem( key ), d_ptr( new MLocaleNullConfigItemPrivate )
{
    d_ptr->valid = false;
    d_ptr->key = key;
}

MLocaleNullConfigItem::~MLocaleNullConfigItem()
{
    delete d_ptr;
}

bool MLocaleNullConfigItem::isValid() const
{
  return d_ptr->valid;
}

QString MLocaleNullConfigItem::value() const
{
  return d_ptr->value;
}

void MLocaleNullConfigItem::setValue( const QString& value )
{
  d_ptr->value = value;
  d_ptr->valid = true;
  emit valueChanged( d_ptr->value );
}

}
