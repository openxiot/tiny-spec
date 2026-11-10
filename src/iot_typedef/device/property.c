/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinycore/tiny_alloc.h>
#include <tinycore/tiny_log.h>
#include <tinyspec/iot_typedef/status/iot_status.h>
#include <tinycore/json_number.h>
#include <tinyspec/iot_typedef/device/property.h>

static TINYCORE_RET Property_Construct(Property *thiz, uint16_t iid);

static void Property_Dispose(Property *thiz);

Property* Property_New(uint16_t iid)
{
    Property *thiz = NULL;

    do
    {
        thiz = (Property *)tiny_malloc(sizeof(Property));
        if (thiz == NULL)
        {
            TINY_LOG_DEBUG("tiny_malloc FAILED");
            break;
        }

        if (RET_FAILED(Property_Construct(thiz, iid)))
        {
            Property_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (false);

    return thiz;
}

Property *Property_NewInstance(uint16_t iid,
                               const char *ns,
                               const char *name,
                               uint32_t uuid,
                               const char *vendor,
                               Format format,
                               Access access,
                               Unit unit)
{
    Property * thiz = NULL;

    do
    {
        thiz = Property_New(iid);
        if (thiz == NULL)
        {
            TINY_LOG_DEBUG("Property_New FAILED");
            break;
        }

        if (RET_FAILED(Urn_Set(&thiz->type, ns, PROPERTY, name, uuid, vendor)))
        {
            Property_Delete(thiz);
            thiz = NULL;
            break;
        }

        thiz->format = format;
        thiz->access = access;
        thiz->unit = unit;
    } while (false);

    return thiz;
}

static TINYCORE_RET Property_Construct(Property *thiz, uint16_t iid)
{
    TINYCORE_RET ret;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    do
    {
        memset(thiz, 0, sizeof(Property));
        thiz->iid = iid;

        ret = Urn_Construct(&thiz->type);
        if (RET_FAILED(ret))
        {
            TINY_LOG_DEBUG("Urn_Construct FAILED: %s", TinyRet_GetDescription( ret));
            break;
        }
    } while (false);

    return ret;
}

static void Property_Dispose(Property *thiz)
{
    RETURN_IF_FAIL(thiz);

    Urn_Dispose(&thiz->type);

    if (thiz->valueList != NULL)
    {
        ValueList_Delete(thiz->valueList);
        thiz->valueList = NULL;
    }

    if (thiz->valueRange != NULL)
    {
        ValueRange_Delete(thiz->valueRange);
        thiz->valueRange = NULL;
    }

    if (thiz->value != NULL)
    {
        JsonValue_Delete(thiz->value);
        thiz->value = NULL;
    }
}

void Property_Delete(Property *thiz)
{
    RETURN_IF_FAIL(thiz);

    TINY_LOG_DEBUG("Property_Delete (iid: %d)", thiz->iid);

    Property_Dispose(thiz);
    tiny_free(thiz);
}

static bool Property_CheckValueType(Property *thiz, JsonValue* value)
{
    RETURN_VAL_IF_FAIL(thiz, false);
    RETURN_VAL_IF_FAIL(value, false);

    switch (thiz->format)
    {
        case FORMAT_BOOL:
            if (value->type == JSON_BOOLEAN)
            {
                return true;
            }
            if (value->type == JSON_NUMBER)
            {
                if (value->data.number->type == JSON_NUMBER_INTEGER)
                {
                    if (value->data.number->value.intValue == 0 || value->data.number->value.intValue == 1)
                    {
                        return true;
                    }
                }
            }
            return false;

        case FORMAT_STRING:
            return (value->type == JSON_STRING);

        case FORMAT_FLOAT:
            if (value->type == JSON_NUMBER)
            {
                return true;
            }
            return false;

        case FORMAT_UINT8:
            if (value->type == JSON_NUMBER)
            {
                if (value->data.number->type == JSON_NUMBER_INTEGER)
                {
                    return Format_CheckInteger(FORMAT_UINT8, value->data.number->value.intValue);
                }
            }
            return false;

        case FORMAT_UINT16:
            if (value->type == JSON_NUMBER)
            {
                if (value->data.number->type == JSON_NUMBER_INTEGER)
                {
                    return Format_CheckInteger(FORMAT_UINT16, value->data.number->value.intValue);
                }
            }
            return false;

        case FORMAT_UINT32:
            if (value->type == JSON_NUMBER)
            {
                if (value->data.number->type == JSON_NUMBER_INTEGER)
                {
                    return Format_CheckInteger(FORMAT_UINT32, value->data.number->value.intValue);
                }
            }
            return false;

        case FORMAT_INT8:
            if (value->type == JSON_NUMBER)
            {
                if (value->data.number->type == JSON_NUMBER_INTEGER)
                {
                    return Format_CheckInteger(FORMAT_INT8, value->data.number->value.intValue);
                }
            }
            return false;

        case FORMAT_INT16:
            if (value->type == JSON_NUMBER)
            {
                if (value->data.number->type == JSON_NUMBER_INTEGER)
                {
                    return Format_CheckInteger(FORMAT_INT16, value->data.number->value.intValue);
                }
            }
            return false;

        case FORMAT_INT:
        case FORMAT_INT32:
            if (value->type == JSON_NUMBER)
            {
                if (value->data.number->type == JSON_NUMBER_INTEGER)
                {
                    return Format_CheckInteger(FORMAT_INT32, value->data.number->value.intValue);
                }
            }
            return false;

        case FORMAT_INT64:
            if (value->type == JSON_NUMBER)
            {
                if (value->data.number->type == JSON_NUMBER_INTEGER)
                {
                    return Format_CheckInteger(FORMAT_INT64, value->data.number->value.intValue);
                }
            }
            return false;

        case FORMAT_HEX:
            return (value->type == JSON_STRING);

        case FORMAT_TLV8:
            return (value->type == JSON_STRING);

        default:
            return false;
    }
}

bool Property_CheckValue(Property *thiz, JsonValue* value)
{
    RETURN_VAL_IF_FAIL(thiz, false);

    TINY_LOG_DEBUG("Property_CheckValue, iid: %d", thiz->iid);

    if (value == NULL)
    {
        TINY_LOG_ERROR("value is null");
        return false;
    }

    if (! Property_CheckValueType(thiz, value))
    {
        TINY_LOG_ERROR("value type invalid: %s, property format: %s", JsonValueType_ToString(value->type), Format_ToString(thiz->format));
        return false;
    }

    if (thiz->valueRange != NULL)
    {
        return ValueRange_CheckValue(thiz->valueRange, value);
    }

    if (thiz->valueList != NULL)
    {
        return ValueList_CheckValue(thiz->valueList, value);
    }

    return true;
}

void Property_TryRead(Property *thiz, PropertyOperation *o)
{
    RETURN_IF_FAIL(thiz);
    RETURN_IF_FAIL(o);

    TINY_LOG_DEBUG("Property_TryRead: %d", thiz->iid);

    if (! Access_IsReadable(thiz->access))
    {
        o->status = IOT_STATUS_PROPERTY_CANNOT_READ;
    }
}

void Property_TryWrite(Property *thiz, PropertyOperation *o)
{
    RETURN_IF_FAIL(thiz);
    RETURN_IF_FAIL(o);

    TINY_LOG_DEBUG("Property_TryWrite: %d", thiz->iid);

    do
    {
        if (! Access_IsWritable(thiz->access))
        {
            o->status = IOT_STATUS_PROPERTY_CANNOT_WRITE;
            break;
        }

        if (! Property_CheckValue(thiz, o->value))
        {
            o->status = IOT_STATUS_PROPERTY_VALUE_INVALID;
            break;
        }
    } while (false);
}

void Property_TryChange(Property *thiz, PropertyOperation *o, bool save)
{
    RETURN_IF_FAIL(thiz);
    RETURN_IF_FAIL(o);

    TINY_LOG_DEBUG("Property_TryChange: %d", thiz->iid);

    do
    {
        if (! Access_IsNotifiable(thiz->access))
        {
            o->status = IOT_STATUS_PROPERTY_CANNOT_NOTIFY;
            break;
        }

        if (! Property_CheckValue(thiz, o->value))
        {
            o->status = IOT_STATUS_PROPERTY_VALUE_INVALID;
            break;
        }

        if (save)
        {
            if (thiz->value != NULL)
            {
                JsonValue_Delete(thiz->value);
                thiz->value = NULL;
            }

            thiz->value = JsonValue_NewFrom(o->value);
        }
    } while (false);
}

void Property_TrySubscribe(Property *thiz, PropertyOperation *o)
{
    RETURN_IF_FAIL(thiz);
    RETURN_IF_FAIL(o);

    TINY_LOG_DEBUG("Property_TrySubscribe: %d", thiz->iid);

    if (! Access_IsNotifiable(thiz->access))
    {
        o->status = IOT_STATUS_PROPERTY_CANNOT_NOTIFY;
    }
}

void Property_TryUnsubscribe(Property *thiz, PropertyOperation *o)
{
    RETURN_IF_FAIL(thiz);
    RETURN_IF_FAIL(o);

    TINY_LOG_DEBUG("Property_TryUnsubscribe: %d", thiz->iid);

    if (! Access_IsNotifiable(thiz->access))
    {
        o->status = IOT_STATUS_PROPERTY_CANNOT_NOTIFY;
    }
}
