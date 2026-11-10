/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_DEVICE_SERVICE_H
#define TINYSPEC_IOT_TYPEDEF_DEVICE_SERVICE_H

#include <tinycore/tiny_list.h>
#include <tinyspec/iot_typedef/operation/property_operation.h>
#include <tinyspec/iot_typedef/operation/action_operation.h>
#include <tinyspec/iot_typedef/operation/event_operation.h>
#include <tinyspec/iot_typedef/device/urn/urn.h>
#include <tinyspec/iot_typedef/device/property.h>
#include <tinyspec/iot_typedef/device/action.h>
#include <tinyspec/iot_typedef/device/event.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    uint16_t            iid;
    Urn                 type;
    TinyList            properties;
    TinyList            actions;
    TinyList            events;
} Service;

TINYSPEC_API
Service* Service_New(uint16_t iid);

TINYSPEC_API
Service* Service_NewInstance(uint16_t iid, const char *ns, const char *name, uint32_t uuid, const char *vendor);

TINYSPEC_API
void Service_Delete(Service *thiz);

TINYSPEC_API
Property *Service_GetProperty(Service *thiz, uint16_t iid);

TINYSPEC_API
Action *Service_GetAction(Service *thiz, uint16_t iid);

TINYSPEC_API
Event *Service_GetEvent(Service *thiz, uint16_t iid);

TINYSPEC_API
bool Service_CheckValue(Service *thiz, PropertyOperation * o);

TINYSPEC_API
bool Service_CheckResult(Service *thiz, ActionOperation * o);

TINYSPEC_API
void Service_TryRead(Service *thiz, PropertyOperation * o);

TINYSPEC_API
void Service_TryWrite(Service *thiz, PropertyOperation * o);

TINYSPEC_API
void Service_TryInvoke(Service *thiz, ActionOperation * o);

TINYSPEC_API
void Service_TryChange(Service *thiz, PropertyOperation *o, bool save);

TINYSPEC_API
void Service_TryProduce(Service *thiz, EventOperation *o);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_DEVICE_SERVICE_H */
