/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/stanza/iq/device/control/get_properties.h>

TinyRet QueryGetProperties_Construct(QueryGetProperties *thiz)
{
    return PropertyOperations_Construct(&thiz->properties);
}

void QueryGetProperties_Dispose(QueryGetProperties *thiz)
{
    PropertyOperations_Dispose(&thiz->properties);
}

TinyRet ResultGetProperties_Construct(ResultGetProperties *thiz)
{
    return PropertyOperations_Construct(&thiz->properties);
}

void ResultGetProperties_Dispose(ResultGetProperties *thiz)
{
    PropertyOperations_Dispose(&thiz->properties);
}
