/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_STATUS_IOT_STATUS_H
#define TINYSPEC_IOT_TYPEDEF_STATUS_IOT_STATUS_H

#include <tinyspec/tinyspec_base.h>

/**
 * FOR GENERIC
 */
#define IOT_STATUS_COMPLETED                        (0)
#define IOT_STATUS_TO_BE_EXECUTE                    (1)
#define IOT_STATUS_INTERNAL_ERROR                   (-500)
#define IOT_STATUS_ACCESS_KEY_INVALID               (-501)
#define IOT_STATUS_DEVICE_ID_NOT_EXIST              (-502)
#define IOT_STATUS_QUERY_NOT_SUPPORTED              (-503)
#define IOT_STATUS_TIMEOUT                          (-504)

/**
 * FOR XCP DEVICE ENDPOINT VERIFY
 */
#define IOT_STATUS_DEVICE_ID_INVALID                (-100)
#define IOT_STATUS_DEVICE_TYPE_INVALID              (-101)
#define IOT_STATUS_PUBLIC_KEY_NOT_FOUND             (-102)
#define IOT_STATUS_SIGNATURE_NOT_FOUND              (-103)
#define IOT_STATUS_VERSION_NOT_FOUND                (-104)
#define IOT_STATUS_AUTHENTICATION_NOT_FOUND         (-105)
#define IOT_STATUS_AUTHENTICATION_NOT_SUPPORTED     (-106)
#define IOT_STATUS_STATUS_INVALID                   (-107)
#define IOT_STATUS_SIGNATURE_INVALID                (-108)
#define IOT_STATUS_CODEC_NOT_FOUND                  (-109)
#define IOT_STATUS_CODEC_NOT_IMPLEMENTED            (-110)
#define IOT_STATUS_DEVICE_ALREADY_ONLINE            (-111)

/**
 * FOR OPERATION
 */
#define IOT_STATUS_DEVICE_OFFLINE                   (-400)
#define IOT_STATUS_AUTHENTICATION_FAILED            (-401)
#define IOT_STATUS_CHILD_NOT_FOUND                  (-402)
#define IOT_STATUS_SERVICE_NOT_FOUND                (-410)

#define IOT_STATUS_PID_INVALID                      (-420)
#define IOT_STATUS_PROPERTY_NOT_FOUND               (-421)
#define IOT_STATUS_PROPERTY_CANNOT_READ             (-422)
#define IOT_STATUS_PROPERTY_CANNOT_WRITE            (-423)
#define IOT_STATUS_PROPERTY_CANNOT_NOTIFY           (-424)
#define IOT_STATUS_PROPERTY_VALUE_INVALID           (-425)
#define IOT_STATUS_PROPERTY_VALUE_ERROR             (-426)

#define IOT_STATUS_AID_INVALID                      (-430)
#define IOT_STATUS_ACTION_NOT_FOUND                 (-431)
#define IOT_STATUS_ACTION_IN_ERROR                  (-432)
#define IOT_STATUS_ACTION_IN_VALUE_INVALID          (-433)
#define IOT_STATUS_ACTION_OUT_ERROR                 (-434)
#define IOT_STATUS_ACTION_OUT_VALUE_INVALID         (-435)

#define IOT_STATUS_EID_INVALID                      (-440)
#define IOT_STATUS_EVENT_NOT_FOUND                  (-441)
#define IOT_STATUS_EVENT_ARGUMENTS_ERROR            (-442)
#define IOT_STATUS_EVENT_ARGUMENTS_VALUE_ERROR      (-443)

#endif /* TINYSPEC_IOT_TYPEDEF_STATUS_IOT_STATUS_H */
