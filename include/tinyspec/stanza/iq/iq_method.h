/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_STANZA_IQ_IQ_METHOD_H
#define TINYSPEC_STANZA_IQ_IQ_METHOD_H

#include <tinyspec/tinyspec_base.h>

TINYSPEC_BEGIN_DECLS

typedef enum
{
    IQ_METHOD_UNDEFINED             = 0x00,

    /*
     * basic
     */
    IQ_METHOD_HELLO                 = 0x01,
    IQ_METHOD_BYE                   = 0x02,
    IQ_METHOD_PING                  = 0x03,

    /*
     * device/control
     */
    IQ_METHOD_GET_PROPERTIES        = 0x11,
    IQ_METHOD_SET_PROPERTIES        = 0x12,
    IQ_METHOD_GET_PROPERTY          = 0x13,
    IQ_METHOD_SET_PROPERTY          = 0x14,
    IQ_METHOD_INVOKE_ACTION         = 0x15,
    IQ_METHOD_INVOKE_ACTIONS        = 0x16,
    IQ_METHOD_PROPERTIES_CHANGED    = 0x17,

    /*
     * device/manager
     */
    IQ_METHOD_GET_CHILDREN          = 0x18,

    /*
     * device/key
     */
    IQ_METHOD_GET_ACCESS_KEY        = 0x21,
    IQ_METHOD_SET_ACCESS_KEY        = 0x22,

    /*
     * device/verify
     */
    IQ_METHOD_INITIALIZE            = 0x31,
    IQ_METHOD_VERIFY_FINISH         = 0x32,
    IQ_METHOD_VERIFY_START          = 0x33,
} IQMethod;

TINYSPEC_API
const char * IQMethod_ToString(IQMethod method);

TINYSPEC_API
IQMethod IQMethod_FromString(const char *string);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_STANZA_IQ_IQ_METHOD_H */
