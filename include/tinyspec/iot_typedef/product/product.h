/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_PRODUCT_PRODUCT_H
#define TINYSPEC_IOT_TYPEDEF_PRODUCT_PRODUCT_H

#include <tinyspec/tinyspec_base.h>
#include <tinycore/tiny_list.h>
#include <tinyspec/iot_typedef/product/config/product_config.h>
#include <tinyspec/iot_typedef/product/handler/action_handler.h>
#include <tinyspec/iot_typedef/product/handler/property_handler.h>
#include <tinyspec/iot_typedef/product/handler/identify_listener.h>
#include <tinyspec/iot_typedef/product/observer/property_observer.h>
#include <tinyspec/iot_typedef/product/observer/event_observer.h>
#include <tinyspec/iot_typedef/operation/action_operation.h>
#include <tinyspec/iot_typedef/operation/event_operation.h>
#include <tinyspec/iot_typedef/device/device.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    ProductConfig               config;
    Device                      device;
    TinyList                    children;
    PropertyOnGet               onGet;
    PropertyOnSet               onSet;
    ActionOnInvoke              onInvoke;
    IdentifyListener            identifyListener;
    PropertyObserver            onPropertyChanged;
    EventObserver               onEventOccurred;
    void                      * context;
} Product;

TINYSPEC_API
Product* Product_New(void);

TINYSPEC_API
void Product_Delete(Product *thiz);

TINYSPEC_API
bool Product_CheckHandler(Product *thiz);

//TINYSPEC_API
//void Product_Lock(Product *thiz);
//
//TINYSPEC_API
//void Product_Unlock(Product *thiz);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_PRODUCT_PRODUCT_H */
