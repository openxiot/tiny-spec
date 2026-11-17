/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_DEVICE_URN_URN_H
#define TINYSPEC_IOT_TYPEDEF_DEVICE_URN_URN_H

#include <tinyspec/tinyspec_base.h>
#include <tinyspec/typedef/device/urn/urn_type.h>

TINYSPEC_BEGIN_DECLS

/**
 * <URN> ::= "urn:"<namespace>":"<type>":"<name>":"<value>[":"<organization>":"<model>":"<version>]
 *
 * urn:homekit-spec:device:light:00000001
 * urn:homekit-spec:service:switch:00000014
 * urn:homekit-spec:property:on:00000030
 *
 * urn:homekit-spec:device:light:00000001:a:b:1
 * urn:homekit-spec:device:light:00000001:x:y:1
 *
 */

typedef struct
{
    char        * ns;
    UrnType       type;
    char        * name;
    uint32_t      value;
    char        * vendor;
} Urn;

TINYSPEC_API
TinyRet Urn_Construct(Urn *thiz);

TINYSPEC_API
void Urn_Dispose(Urn *thiz);

TINYSPEC_API
TinyRet Urn_Set(Urn *thiz, const char *ns, UrnType type, const char *name, uint32_t value, const char *vendor);

TINYSPEC_API
TinyRet Urn_SetString(Urn *thiz, const char *string);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_DEVICE_URN_URN_H */
