/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_DEVICE_ACTION_H
#define TINYSPEC_IOT_TYPEDEF_DEVICE_ACTION_H

#include <tinycore/tiny_list.h>
#include <tinyspec/typedef/operation/action_operation.h>
#include <tinyspec/tinyspec_base.h>
#include <tinyspec/typedef/device/urn/urn.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    uint16_t            iid;
    Urn                 type;
    TinyList            in;
    TinyList            out;
    void *              service;
} Action;

TINYSPEC_API
Action* Action_New(uint16_t iid);

TINYSPEC_API
Action* Action_NewInstance(uint16_t iid, const char *ns, const char *name, uint32_t uuid, const char *vendor);

TINYSPEC_API
void Action_Delete(Action *thiz);

TINYSPEC_API
TinyRet Action_InAdd(Action *thiz, uint16_t iid);

/** 带 repeat 范围的入参（等价于 spec 的 repeat: [min, max]）。 */
TINYSPEC_API
TinyRet Action_InAddEx(Action *thiz, uint16_t iid, uint16_t min, uint16_t max);

TINYSPEC_API
TinyRet Action_OutAdd(Action *thiz, uint16_t iid);

TINYSPEC_API
TinyRet Action_OutAddEx(Action *thiz, uint16_t iid, uint16_t min, uint16_t max);

TINYSPEC_API
void Action_TryInvoke(Action *thiz, ActionOperation *o);

TINYSPEC_API
void Action_CheckResult(Action *thiz, ActionOperation *o);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_DEVICE_ACTION_H */
