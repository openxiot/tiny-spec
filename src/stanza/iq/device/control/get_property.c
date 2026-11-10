/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/stanza/iq/device/control/get_property.h>

TINYCORE_RET QueryGetProperty_Construct(QueryGetProperty *thiz)
{
    return PropertyOperation_Construct(&thiz->property);
}

void QueryGetProperty_Dispose(QueryGetProperty *thiz)
{
    PropertyOperation_Dispose(&thiz->property);
}

TINYCORE_RET ResultGetProperty_Construct(ResultGetProperty *thiz)
{
    return PropertyOperation_Construct(&thiz->property);
}

void ResultGetProperty_Dispose(ResultGetProperty *thiz)
{
    PropertyOperation_Dispose(&thiz->property);
}
