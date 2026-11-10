/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinycore/json_array.h>
#include <tinycore/json_number.h>
#include <tinycore/tiny_log.h>
#include <tinyspec/iot_typedef/notice/device/impl/device_property_changed_codec.h>
#include <tinyspec/iot_typedef/notice/device/impl/device_properties_changed_codec.h>

TINYCORE_RET DevicePropertiesChangedCodec_Encode(JsonObject *payload,
                                                 DevicePropertiesChanged *propertiesChanged)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(payload, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(propertiesChanged, TINY_RET_E_NULL_PTR);

    do
    {
        JsonArray *array = JsonArray_New();
        if (array == NULL)
        {
            TINY_LOG_ERROR("JsonArray_New");
            ret = TINY_RET_E_OUT_OF_MEMORY;
            break;
        }

        for (uint32_t i = 0; i < propertiesChanged->properties.size; ++i)
        {
            DevicePropertyChanged *item =
                    (DevicePropertyChanged *) TinyList_GetAt(&propertiesChanged->properties, i);
            JsonObject *object = JsonObject_New();

            if (object == NULL)
            {
                TINY_LOG_ERROR("JsonObject_New");
                ret = TINY_RET_E_OUT_OF_MEMORY;
                break;
            }

            ret = DevicePropertyChangedCodec_Encode(object, item);
            if (RET_FAILED(ret))
            {
                JsonObject_Delete(object);
                break;
            }

            ret = JsonArray_AddObject(array, object);
            if (RET_FAILED(ret))
            {
                /* AddObject failed: it did not take ownership. */
                JsonObject_Delete(object);
                break;
            }
        }

        if (RET_FAILED(ret))
        {
            JsonArray_Delete(array);
            break;
        }

        ret = JsonObject_PutArray(payload, "properties", array);
        if (RET_FAILED(ret))
        {
            JsonArray_Delete(array);
            break;
        }

        if (propertiesChanged->timestamp != 0)
        {
            ret = JsonObject_PutInteger(payload, "timestamp",
                                        (int32_t) propertiesChanged->timestamp);
        }
    } while (false);

    return ret;
}

TINYCORE_RET DevicePropertiesChangedCodec_Decode(DevicePropertiesChanged *propertiesChanged,
                                                 JsonObject *payload)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(propertiesChanged, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(payload, TINY_RET_E_NULL_PTR);

    do
    {
        /* Decoders receive zeroed storage, so install the ownership
         * listener before items are appended. */
        ret = DevicePropertiesChanged_Construct(propertiesChanged);
        if (RET_FAILED(ret))
        {
            break;
        }

        JsonArray *array = JsonObject_GetArray(payload, "properties");
        if (array == NULL)
        {
            TINY_LOG_ERROR("properties not found");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        uint32_t size = JsonArray_GetSize(array);
        if (size == 0)
        {
            TINY_LOG_ERROR("properties is empty");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        for (uint32_t i = 0; i < size; ++i)
        {
            JsonObject *object = JsonArray_GetObject(array, i);
            if (object == NULL)
            {
                TINY_LOG_ERROR("properties[%u] is not an object", i);
                ret = TINY_RET_E_PARAM_INVALID;
                break;
            }

            DevicePropertyChanged *item = DevicePropertyChanged_New();
            if (item == NULL)
            {
                ret = TINY_RET_E_OUT_OF_MEMORY;
                break;
            }

            ret = DevicePropertyChangedCodec_Decode(item, object);
            if (RET_FAILED(ret))
            {
                DevicePropertyChanged_Delete(item);
                break;
            }

            ret = DevicePropertiesChanged_Add(propertiesChanged, item);
            if (RET_FAILED(ret))
            {
                DevicePropertyChanged_Delete(item);
                break;
            }
        }

        if (RET_FAILED(ret))
        {
            break;
        }

        JsonNumber *timestamp = JsonObject_GetNumber(payload, "timestamp");
        if (timestamp != NULL && JsonNumber_IsInteger(timestamp))
        {
            propertiesChanged->timestamp = (int64_t) JsonNumber_GetInteger(timestamp);
        }
    } while (false);

    return ret;
}
