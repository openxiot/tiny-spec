/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/stanza_codec/stanza_codec.h>
#include <tinycore/tiny_log.h>
#include <tinycore/json_number.h>
#include <tinyspec/stanza_codec/iq/iq_codec.h>
#include <tinyspec/stanza_codec/message/message_codec.h>

JsonObject* StanzaCodec_Encode(Stanza *stanza)
{
    TINYCORE_RET ret = TINY_RET_OK;
    JsonObject *root = NULL;

    RETURN_VAL_IF_FAIL(stanza, NULL);

    do
    {
        root = JsonObject_New();
        if (root == NULL)
        {
            break;
        }

        if (stanza->type == STANZA_IQ)
        {
            ret = IQCodec_Encode(root, & (stanza->data.iq));
            if (RET_FAILED(ret))
            {
                TINY_LOG_ERROR("IQCodec_Encode: %s", TinyRet_GetDescription(ret));
                JsonObject_Delete(root);
                root = NULL;
                break;
            }
            break;
        }

        if (stanza->type == STANZA_MESSAGE)
        {
            ret = MessageCodec_Encode(root, & (stanza->data.message));
            if (RET_FAILED(ret))
            {
                TINY_LOG_ERROR("MessageCodec_Encode: %s", TinyRet_GetDescription(ret));
                JsonObject_Delete(root);
                root = NULL;
                break;
            }
            break;
        }
    } while (false);

    return root;
}

Stanza* StanzaCodec_Decode(JsonObject *object)
{
    TINYCORE_RET ret = TINY_RET_OK;
    Stanza * stanza = NULL;

    RETURN_VAL_IF_FAIL(object, NULL);

    do
    {
        if (JsonObject_ContainsKey(object, "iq"))
        {
            JsonObject *iq = NULL;

            stanza = Stanza_New(STANZA_IQ);
            if (stanza == NULL)
            {
                break;
            }

            iq = JsonObject_GetObject(object, "iq");
            if (iq == NULL)
            {
                TINY_LOG_DEBUG("invalid stanza, iq not found!");
                Stanza_Delete(stanza);
                stanza = NULL;
                break;
            }

            ret = IQCodec_Decode(&stanza->data.iq, iq);
            if (RET_FAILED(ret))
            {
                TINY_LOG_ERROR("IQCodec_Decode FAILED: %s", TinyRet_GetDescription(ret));
                Stanza_Delete(stanza);
                stanza = NULL;
                break;
            }
        }

        if (JsonObject_ContainsKey(object, "message"))
        {
            JsonObject *message = NULL;

            stanza = Stanza_New(STANZA_MESSAGE);
            if (stanza == NULL)
            {
                break;
            }

            message = JsonObject_GetObject(object, "message");
            if (message == NULL)
            {
                TINY_LOG_DEBUG("invalid stanza, message not found!");
                Stanza_Delete(stanza);
                stanza = NULL;
                break;
            }

            ret = MessageCodec_Decode(&stanza->data.message, message);
            if (RET_FAILED(ret))
            {
                TINY_LOG_ERROR("MessageCodec_Decode: %s", TinyRet_GetDescription(ret));
                Stanza_Delete(stanza);
                stanza = NULL;
                break;
            }
        }
    } while (false);

    return stanza;
}
