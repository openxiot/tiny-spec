/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/codec/message/message_codec.h>
#include <tinycore/tiny_log.h>
#include <tinyspec/typedef/notice/device/device_notice_codec.h>

TinyRet MessageCodec_Encode(JsonObject *root, Message *message)
{
    TinyRet ret = TINY_RET_OK;
    JsonObject *object = NULL;

    RETURN_VAL_IF_FAIL(root, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(message, TINY_RET_E_NULL_PTR);

    do
    {
        JsonObject *payload = NULL;
        const char *type = NULL;

        object = JsonObject_New();
        if (object == NULL)
        {
            TINY_LOG_ERROR("JsonObject_New");
            ret = TINY_RET_E_OUT_OF_MEMORY;
            break;
        }

        ret = JsonObject_PutString(object, "id", message->id);
        if (RET_FAILED(ret))
        {
            TINY_LOG_ERROR("JsonObject_PutString: %s", TinyRet_GetDescription(ret));
            ret = TINY_RET_E_OUT_OF_MEMORY;
            break;
        }

        ret = JsonObject_PutString(object, "topic", message_topic_to_string(message->topic));
        if (RET_FAILED(ret))
        {
            TINY_LOG_ERROR("JsonObject_PutString: %s", TinyRet_GetDescription(ret));
            ret = TINY_RET_E_OUT_OF_MEMORY;
            break;
        }

        payload = JsonObject_New();
        if (payload == NULL)
        {
            TINY_LOG_ERROR("JsonObject_New");
            ret = TINY_RET_E_OUT_OF_MEMORY;
            break;
        }

        ret = JsonObject_PutObject(object, "payload", payload);
        if (RET_FAILED(ret))
        {
            TINY_LOG_ERROR("JsonObject_PutObject: %s", TinyRet_GetDescription(ret));
            JsonObject_Delete(payload);
            ret = TINY_RET_E_OUT_OF_MEMORY;
            break;
        }

        switch (message->topic)
        {
            case MESSAGE_TOPIC_DEVICE:
                type = device_notice_type_to_string((DeviceNoticeType) (message->type));
                ret = JsonObject_PutString(object, "type", type);
                if (RET_SUCCEEDED(ret))
                {
                    ret = DeviceNoticeCodec_Encode(payload, message->type, &message->payload.data.device);
                }
                else
                {
                    TINY_LOG_ERROR("JsonObject_PutString: %s", TinyRet_GetDescription(ret));
                }
                break;

            default:
                TINY_LOG_ERROR("message.topic: %s", message_topic_to_string(message->topic));
                ret = TINY_RET_E_PARAM_INVALID;
                break;
        }

        if (RET_FAILED(ret))
        {
            break;
        }

        ret = JsonObject_PutObject(root, "message", object);
        if (RET_FAILED(ret))
        {
            TINY_LOG_ERROR("JsonObject_PutObject: %s", TinyRet_GetDescription(ret));
            break;
        }
    } while (false);

    if (RET_FAILED(ret))
    {
        if (object != NULL)
        {
            JsonObject_Delete(object);
        }
    }

    return ret;
}

TinyRet MessageCodec_Decode(Message* message, JsonObject *object)
{
    TinyRet ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(message, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(object, TINY_RET_E_NULL_PTR);

    do
    {
        JsonString *id = JsonObject_GetString(object, "id");
        if (id == NULL)
        {
            TINY_LOG_ERROR("#id not found");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        strncpy(message->id, id->value, IQ_ID_LENGTH);

        JsonString *topic = JsonObject_GetString(object, "topic");
        if (topic == NULL)
        {
            TINY_LOG_ERROR("#topic not found");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        JsonString *type = JsonObject_GetString(object, "type");
        if (type == NULL)
        {
            TINY_LOG_ERROR("#type not found");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        JsonObject *payload = JsonObject_GetObject(object, "payload");
        if (payload == NULL)
        {
            TINY_LOG_ERROR("#payload not found");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        message->topic = message_topic_from_string(topic->value);

        switch (message->topic)
        {
            case MESSAGE_TOPIC_DEVICE:
                message->type = (int) device_notice_type_from_string(type->value);
                ret = DeviceNoticeCodec_Decode(&message->payload.data.device, message->type, payload);
                break;

            default:
                TINY_LOG_ERROR("message topic not support: %s", topic->value);
                ret = TINY_RET_E_PARAM_INVALID;
                break;
        }
    } while (false);

    return ret;
}
