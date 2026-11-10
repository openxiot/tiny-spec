/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/stanza/iq/device/control/set_property.h>

TINYCORE_RET QuerySetProperty_Construct(QuerySetProperty *thiz)
{
    return PropertyOperation_Construct(&thiz->property);
}

void QuerySetProperty_Dispose(QuerySetProperty *thiz)
{
    PropertyOperation_Dispose(&thiz->property);
}

TINYCORE_RET ResultSetProperty_Construct(ResultSetProperty *thiz)
{
    return PropertyOperation_Construct(&thiz->property);
}

void ResultSetProperty_Dispose(ResultSetProperty *thiz)
{
    PropertyOperation_Dispose(&thiz->property);
}
