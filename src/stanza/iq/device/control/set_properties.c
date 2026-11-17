/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/stanza/iq/device/control/set_properties.h>

TinyRet QuerySetProperties_Construct(QuerySetProperties *thiz)
{
    return PropertyOperations_Construct(&thiz->properties);
}

void QuerySetProperties_Dispose(QuerySetProperties *thiz)
{
    PropertyOperations_Dispose(&thiz->properties);
}

TinyRet ResultSetProperties_Construct(ResultSetProperties *thiz)
{
    return PropertyOperations_Construct(&thiz->properties);
}

void ResultSetProperties_Dispose(ResultSetProperties *thiz)
{
    PropertyOperations_Dispose(&thiz->properties);
}
