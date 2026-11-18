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

#include <tinycore/tiny_list.h>
#include <tinyspec/tinyspec_base.h>
#include <tinyspec/typedef/device/device.h>
#include <tinyspec/typedef/operation/action_operation.h>
#include <tinyspec/typedef/operation/event_operation.h>
#include <tinyspec/typedef/product/config/product_config.h>
#include <tinyspec/typedef/product/handler/action_handler.h>
#include <tinyspec/typedef/product/handler/identify_listener.h>
#include <tinyspec/typedef/product/handler/property_handler.h>
#include <tinyspec/typedef/product/observer/event_observer.h>
#include <tinyspec/typedef/product/observer/property_observer.h>
#include <tinyspec/typedef/product/send/send.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    ProductConfig config;
    Device device;
    TinyList children;
    PropertyOnGet onGet;
    PropertyOnSet onSet;
    ActionOnInvoke onInvoke;
    IdentifyListener identifyListener;
    PropertyObserver onPropertyChanged;
    EventObserver onEventOccurred;
    uint32_t stanzaId;
    void *context;
    DataSend send;
    void *sendCtx;
} Product;

TINYSPEC_API
Product *Product_New(void);

TINYSPEC_API
void Product_Delete(Product *thiz);

TINYSPEC_API
bool Product_CheckHandler(Product *thiz);

TINYSPEC_API
void Product_OnReceive(Product *thiz, JsonObject *data);

TINYSPEC_API
void Product_GetNextStanzaId(Product *thiz, char *buf, size_t size);

TINYSPEC_API
void Product_SendPing(Product *thiz);

/*
 * 把某属性(siid.piid)的新值 value 作为 properties-changed 通知发送到对端。
 * 校验属性可通知、取值合法后才发送；否则跳过并告警。
 *
 * 注意：不接管 value 的所有权——内部会深拷一份，调用方负责在返回后释放传入的 value。
 */
TINYSPEC_API
void Product_ChangePropertyValue(Product *thiz, int siid, int piid, JsonValue *value);

TINYSPEC_API
void Product_NotifyEventOccurred(Product *thiz, int siid, int eiid, JsonArray * arguments);


TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_PRODUCT_PRODUCT_H */
