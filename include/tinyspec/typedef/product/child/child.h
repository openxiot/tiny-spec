/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_PRODUCT_CHILD_H
#define TINYSPEC_IOT_TYPEDEF_PRODUCT_CHILD_H

#include <tinyspec/tinyspec_base.h>
#include <tinycore/tiny_list.h>
#include <tinyspec/typedef/product/handler/action_handler.h>
#include <tinyspec/typedef/product/handler/property_handler.h>
#include <tinyspec/typedef/product/handler/identify_listener.h>
#include <tinyspec/typedef/product/observer/property_observer.h>
#include <tinyspec/typedef/product/observer/event_observer.h>
#include <tinyspec/typedef/operation/action_operation.h>
#include <tinyspec/typedef/operation/event_operation.h>
#include <tinyspec/typedef/device/device.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    char                        did[IOT_DEVICE_ID_LENGTH + 1];
    char                        type[IOT_DEVICE_TYPE_LENGTH + 1];
    bool                        online;
    Device                      device;
    PropertyOnGet               onGet;
    PropertyOnSet               onSet;
    ActionOnInvoke              onInvoke;
    IdentifyListener            identifyListener;
    PropertyObserver            onPropertyChanged;
    EventObserver               onEventOccurred;
    void                      * context;
} Child;

TINYSPEC_API
Child* Child_New(void);

TINYSPEC_API
void Child_Delete(Child *thiz);

TINYSPEC_API
TinyRet Child_Copy(Child *dst, const Child *src);


TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_PRODUCT_CHILD_H */
