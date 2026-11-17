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
#include <tinycore/tiny_snprintf.h>
#include <tinyspec/typedef/status/iot_status.h>
#include <tinycore/json_string.h>
#include <tinycore/json_object.h>
#include <tinyspec/typedef/device/property.h>

static TinyRet Property_Construct(Property *thiz, uint16_t iid);

static void Property_Dispose(Property *thiz);

Property *Property_New(uint16_t iid) {
    Property *thiz = NULL;

    do {
        thiz = (Property *) tiny_malloc(sizeof(Property));
        if (thiz == NULL) {
            TINY_LOG_DEBUG("tiny_malloc FAILED");
            break;
        }

        if (RET_FAILED(Property_Construct(thiz, iid))) {
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
                               const char *unit
) {
    Property *thiz = NULL;

    do {
        thiz = Property_New(iid);
        if (thiz == NULL) {
            TINY_LOG_DEBUG("Property_New FAILED");
            break;
        }

        if (RET_FAILED(Urn_Set(&thiz->type, ns, PROPERTY, name, uuid, vendor))) {
            Property_Delete(thiz);
            thiz = NULL;
            break;
        }

        thiz->format = format;
        thiz->access = access;
        thiz->unit = tiny_strdup(unit);
    } while (false);

    return thiz;
}

static TinyRet Property_Construct(Property *thiz, uint16_t iid) {
    TinyRet ret;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    do {
        memset(thiz, 0, sizeof(Property));
        thiz->iid = iid;

        TinyList_Construct(&thiz->members);

        ret = Urn_Construct(&thiz->type);
        if (RET_FAILED(ret)) {
            TINY_LOG_DEBUG("Urn_Construct FAILED: %s", TinyRet_GetDescription( ret));
            break;
        }
    } while (false);

    return ret;
}

static void Property_Dispose(Property *thiz) {
    RETURN_IF_FAIL(thiz);

    Urn_Dispose(&thiz->type);

    if (thiz->unit != NULL) {
        tiny_free(thiz->unit);
        thiz->unit = NULL;
    }

    TinyList_Dispose(&thiz->members);

    if (thiz->valueList != NULL) {
        ValueList_Delete(thiz->valueList);
        thiz->valueList = NULL;
    }

    if (thiz->valueRange != NULL) {
        ValueRange_Delete(thiz->valueRange);
        thiz->valueRange = NULL;
    }

    if (thiz->value != NULL) {
        JsonValue_Delete(thiz->value);
        thiz->value = NULL;
    }
}

void Property_Delete(Property *thiz) {
    RETURN_IF_FAIL(thiz);

    TINY_LOG_DEBUG("Property_Delete (iid: %d)", thiz->iid);

    Property_Dispose(thiz);
    tiny_free(thiz);
}

void Property_SetUnit(Property *thiz, const char *unit) {
    RETURN_IF_FAIL(thiz);

    if (thiz->unit != NULL) {
        tiny_free(thiz->unit);
        thiz->unit = NULL;
    }

    if (unit != NULL && unit[0] != '\0') {
        thiz->unit = tiny_strdup(unit);
    }
}

TinyRet Property_MemberAdd(Property *thiz, Property *member) {
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(member, TINY_RET_E_NULL_PTR);

    if (TinyList_AddTail(&thiz->members, member) == NULL) {
        TINY_LOG_ERROR("Property_MemberAdd FAILED");
        return TINY_RET_E_OUT_OF_MEMORY;
    }

    return TINY_RET_OK;
}

uint32_t Property_GetMemberCount(Property *thiz) {
    RETURN_VAL_IF_FAIL(thiz, 0);

    return thiz->members.size;
}

Property *Property_GetMemberAt(Property *thiz, uint32_t index) {
    RETURN_VAL_IF_FAIL(thiz, NULL);

    return (Property *) TinyList_GetAt(&thiz->members, index);
}

static bool Property_CheckValueType(Property *thiz, JsonValue *value) {
    RETURN_VAL_IF_FAIL(thiz, false);
    RETURN_VAL_IF_FAIL(value, false);

    switch (thiz->format) {
        case FORMAT_BOOL:
            if (value->type == JSON_BOOLEAN) {
                return true;
            }
            if (value->type == JSON_NUMBER) {
                if (value->data.number->type == JSON_NUMBER_INTEGER) {
                    if (value->data.number->value.intValue == 0 || value->data.number->value.intValue == 1) {
                        return true;
                    }
                }
            }
            return false;

        case FORMAT_STRING:
            return (value->type == JSON_STRING);

        case FORMAT_FLOAT:
            if (value->type == JSON_NUMBER) {
                return true;
            }
            return false;

        case FORMAT_UINT8:
            if (value->type == JSON_NUMBER) {
                if (value->data.number->type == JSON_NUMBER_INTEGER) {
                    return Format_CheckInteger(FORMAT_UINT8, value->data.number->value.intValue);
                }
            }
            return false;

        case FORMAT_UINT16:
            if (value->type == JSON_NUMBER) {
                if (value->data.number->type == JSON_NUMBER_INTEGER) {
                    return Format_CheckInteger(FORMAT_UINT16, value->data.number->value.intValue);
                }
            }
            return false;

        case FORMAT_UINT32:
            if (value->type == JSON_NUMBER) {
                if (value->data.number->type == JSON_NUMBER_INTEGER) {
                    return Format_CheckInteger(FORMAT_UINT32, value->data.number->value.intValue);
                }
            }
            return false;

        case FORMAT_INT8:
            if (value->type == JSON_NUMBER) {
                if (value->data.number->type == JSON_NUMBER_INTEGER) {
                    return Format_CheckInteger(FORMAT_INT8, value->data.number->value.intValue);
                }
            }
            return false;

        case FORMAT_INT16:
            if (value->type == JSON_NUMBER) {
                if (value->data.number->type == JSON_NUMBER_INTEGER) {
                    return Format_CheckInteger(FORMAT_INT16, value->data.number->value.intValue);
                }
            }
            return false;

        case FORMAT_INT:
        case FORMAT_INT32:
            if (value->type == JSON_NUMBER) {
                if (value->data.number->type == JSON_NUMBER_INTEGER) {
                    return Format_CheckInteger(FORMAT_INT32, value->data.number->value.intValue);
                }
            }
            return false;

        case FORMAT_INT64:
            if (value->type == JSON_NUMBER) {
                if (value->data.number->type == JSON_NUMBER_INTEGER) {
                    return Format_CheckInteger(FORMAT_INT64, value->data.number->value.intValue);
                }
            }
            return false;

        case FORMAT_HEX:
            return (value->type == JSON_STRING);

        case FORMAT_TLV8:
            return (value->type == JSON_STRING);

        case FORMAT_COMBINATION:
            return (value->type == JSON_OBJECT);

        default:
            return false;
    }
}

static bool Property_CheckMembers(Property *thiz, JsonValue *value) {
    JsonObject *object = NULL;
    uint32_t size = 0;
    uint32_t i = 0;

    RETURN_VAL_IF_FAIL(thiz, false);
    RETURN_VAL_IF_FAIL(value, false);

    object = value->data.object;
    if (object == NULL) {
        TINY_LOG_ERROR("combination value is not an object");
        return false;
    }

    size = Property_GetMemberCount(thiz);
    for (i = 0; i < size; ++i) {
        Property *member = Property_GetMemberAt(thiz, i);
        char key[16];
        JsonValue *memberValue = NULL;

        RETURN_VAL_IF_FAIL(member, false);

        tiny_snprintf(key, sizeof(key), "%u", member->iid);

        if (!JsonObject_ContainsKey(object, key)) {
            TINY_LOG_ERROR("combination missing member, iid: %u", member->iid);
            return false;
        }

        memberValue = JsonObject_GetValue(object, key);
        if (memberValue == NULL) {
            TINY_LOG_ERROR("combination member value is null, iid: %u", member->iid);
            return false;
        }

        if (!Property_CheckValue(member, memberValue)) {
            TINY_LOG_ERROR("combination member value invalid, iid: %u", member->iid);
            return false;
        }
    }

    return true;
}

bool Property_CheckValue(Property *thiz, JsonValue *value) {
    RETURN_VAL_IF_FAIL(thiz, false);

    TINY_LOG_DEBUG("Property_CheckValue, iid: %d", thiz->iid);

    if (value == NULL) {
        TINY_LOG_ERROR("value is null");
        return false;
    }

    if (!Property_CheckValueType(thiz, value)) {
        TINY_LOG_ERROR("value type invalid: %s, property format: %s", JsonValueType_ToString(value->type),
                       Format_ToString(thiz->format));
        return false;
    }

    if (thiz->format == FORMAT_COMBINATION) {
        return Property_CheckMembers(thiz, value);
    }

    if (thiz->format == FORMAT_STRING && thiz->maxLength > 0) {
        if (JsonString_GetLength(value->data.string) > thiz->maxLength) {
            TINY_LOG_ERROR("string too long, max-length: %u", thiz->maxLength);
            return false;
        }
    }

    if (thiz->valueRange != NULL) {
        return ValueRange_CheckValue(thiz->valueRange, value);
    }

    if (thiz->valueList != NULL) {
        return ValueList_CheckValue(thiz->valueList, value);
    }

    return true;
}

void Property_TryRead(Property *thiz, PropertyOperation *o) {
    RETURN_IF_FAIL(thiz);
    RETURN_IF_FAIL(o);

    TINY_LOG_DEBUG("Property_TryRead: %d", thiz->iid);

    if (!Access_IsReadable(thiz->access)) {
        o->status = IOT_STATUS_PROPERTY_CANNOT_READ;
    }
}

void Property_TryWrite(Property *thiz, PropertyOperation *o) {
    RETURN_IF_FAIL(thiz);
    RETURN_IF_FAIL(o);

    TINY_LOG_DEBUG("Property_TryWrite: %d", thiz->iid);

    do {
        if (!Access_IsWritable(thiz->access)) {
            o->status = IOT_STATUS_PROPERTY_CANNOT_WRITE;
            break;
        }

        if (!Property_CheckValue(thiz, o->value)) {
            o->status = IOT_STATUS_PROPERTY_VALUE_INVALID;
            break;
        }
    } while (false);
}

void Property_TryChange(Property *thiz, PropertyOperation *o, bool save) {
    RETURN_IF_FAIL(thiz);
    RETURN_IF_FAIL(o);

    TINY_LOG_DEBUG("Property_TryChange: %d", thiz->iid);

    do {
        if (!Access_IsNotifiable(thiz->access)) {
            o->status = IOT_STATUS_PROPERTY_CANNOT_NOTIFY;
            break;
        }

        if (!Property_CheckValue(thiz, o->value)) {
            o->status = IOT_STATUS_PROPERTY_VALUE_INVALID;
            break;
        }

        if (save) {
            if (thiz->value != NULL) {
                JsonValue_Delete(thiz->value);
                thiz->value = NULL;
            }

            thiz->value = JsonValue_NewFrom(o->value);
        }
    } while (false);
}

// void Property_TrySubscribe(Property *thiz, PropertyOperation *o) {
//     RETURN_IF_FAIL(thiz);
//     RETURN_IF_FAIL(o);
//
//     TINY_LOG_DEBUG("Property_TrySubscribe: %d", thiz->iid);
//
//     if (!Access_IsNotifiable(thiz->access)) {
//         o->status = IOT_STATUS_PROPERTY_CANNOT_NOTIFY;
//     }
// }

// void Property_TryUnsubscribe(Property *thiz, PropertyOperation *o) {
//     RETURN_IF_FAIL(thiz);
//     RETURN_IF_FAIL(o);
//
//     TINY_LOG_DEBUG("Property_TryUnsubscribe: %d", thiz->iid);
//
//     if (!Access_IsNotifiable(thiz->access)) {
//         o->status = IOT_STATUS_PROPERTY_CANNOT_NOTIFY;
//     }
// }
