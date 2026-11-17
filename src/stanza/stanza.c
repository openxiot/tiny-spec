/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinycore/tiny_alloc.h>
#include <tinyspec/stanza/stanza.h>

static TinyRet Stanza_Construct(Stanza *thiz, StanzaType type)
{
    memset(thiz, 0, sizeof(Stanza));

    thiz->type = type;

    return TINY_RET_OK;
}

static void Message_Dispose(Message *thiz)
{
    switch (thiz->topic)
    {
        case MESSAGE_TOPIC_DEVICE:
            switch ((DeviceNoticeType) thiz->type)
            {
                case DEVICE_NOTICE_PROPERTY_CHANGED:
                    DevicePropertyChanged_Dispose(&thiz->payload.data.device.payload.propertyChanged);
                    break;

                case DEVICE_NOTICE_EVENT_OCCURRED:
                    DeviceEventOccurred_Dispose(&thiz->payload.data.device.payload.eventOccurred);
                    break;

                default:
                    break;
            }
            break;

        default:
            break;
    }
}

static void Stanza_Dispose(Stanza *thiz)
{
    RETURN_IF_FAIL(thiz);

    switch (thiz->type)
    {
        case STANZA_IQ:
            IQ_Dispose(&thiz->data.iq);
            break;

        case STANZA_MESSAGE:
            Message_Dispose(&thiz->data.message);
            break;

        default:
            break;
    }
}

Stanza * Stanza_New(StanzaType type)
{
    Stanza *thiz = NULL;

    do
    {
        thiz = (Stanza *)tiny_malloc(sizeof(Stanza));
        if (thiz == NULL)
        {
            break;
        }

        if (RET_FAILED(Stanza_Construct(thiz, type)))
        {
            Stanza_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (0);

    return thiz;
}

void Stanza_Delete(Stanza *thiz)
{
    Stanza_Dispose(thiz);
    tiny_free(thiz);
}
