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
#include <tinycore/json_array.h>
#include <tinyspec/stanza/iq/device/manager/properties_changed.h>
#include <tinyspec/stanza/message/message.h>
#include <tinyspec/typedef/operation/property_operation.h>
#include <tinyspec/typedef/notice/device/impl/device_event_occurred.h>
#include <tinyspec/typedef/product/child/child.h>
#include <tinyspec/typedef/product/operator/product_operator.h>
#include <tinyspec/typedef/product/product.h>
#include <tinyspec/typedef/status/iot_status.h>

#include "tinyspec/codec/stanza_codec.h"
#include "tinyspec/dispatch/dispatch.h"
#include "tinyspec/dispatch/send.h"
#include "tinyspec/stanza/iq/basic/factory/ping_factory.h"
#include "tinyspec/stanza/stanza.h"

static TinyRet Product_Construct(Product *thiz);

static void Product_Dispose(Product *thiz);

static void child_release_handler(void *data, void *ctx)
{
    (void)ctx;

    Child_Delete(data);
}

Product *Product_New(void)
{
    Product *thiz = NULL;

    do
    {
        thiz = (Product *)tiny_malloc(sizeof(Product));
        if (thiz == NULL)
        {
            TINY_LOG_DEBUG("tiny_malloc FAILED");
            break;
        }

        if (RET_FAILED(Product_Construct(thiz)))
        {
            Product_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (false);

    return thiz;
}

static TinyRet Product_Construct(Product *thiz)
{
    TinyRet ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    do
    {
        memset(thiz, 0, sizeof(Product));

        ret = Device_Construct(&thiz->device);
        if (RET_FAILED(ret))
        {
            TINY_LOG_DEBUG("Device_Construct FAILED: %s", TinyRet_GetDescription(ret));
            break;
        }

        TinyList_Construct(&thiz->children);
        TinyList_SetDeleteListener(&thiz->children, child_release_handler, NULL);
    } while (false);

    return ret;
}

static void Product_Dispose(Product *thiz)
{
    RETURN_IF_FAIL(thiz);

    TinyList_Dispose(&thiz->children);
    Device_Dispose(&thiz->device);
}

void Product_Delete(Product *thiz)
{
    RETURN_IF_FAIL(thiz);

    Product_Dispose(thiz);
    tiny_free(thiz);
}

bool Product_CheckHandler(Product *thiz)
{
    RETURN_VAL_IF_FAIL(thiz, false);

    TINY_LOG_DEBUG("Product_CheckHandler");

    if (thiz->onSet == NULL)
    {
        TINY_LOG_ERROR("Property Setter is null");
        return false;
    }

    if (thiz->onGet == NULL)
    {
        TINY_LOG_ERROR("Property Getter is null");
        return false;
    }

    if (thiz->onInvoke == NULL)
    {
        TINY_LOG_ERROR("Action Invocation is null");
    }

    return true;
}

void Product_OnReceive(Product *thiz, JsonObject *data)
{
    Stanza *stanza = NULL;

    RETURN_IF_FAIL(thiz);
    RETURN_IF_FAIL(data);

    do
    {
        stanza = StanzaCodec_Decode(data);
        if (stanza == NULL)
        {
            break;
        }

        if (stanza->type == STANZA_IQ)
        {
            dispatch_iq(thiz, &stanza->data.iq);
        }
    } while (false);

    if (stanza != NULL)
    {
        Stanza_Delete(stanza);
    }
}

void Product_GetNextStanzaId(Product *thiz, char *buf, size_t size)
{
    RETURN_IF_FAIL(thiz);
    snprintf(buf, size, "%u", ++thiz->stanzaId);
}

void Product_SendPing(Product *thiz)
{
    Stanza *stanza;

    RETURN_IF_FAIL(thiz);

    do
    {
        char id[32];

        Product_GetNextStanzaId(thiz, id, sizeof(id));

        stanza = Ping_New(id);
        if (stanza == NULL)
        {
            break;
        }

        const TinyRet ret = send_stanza(thiz, stanza);
        if (RET_FAILED(ret))
        {
            TINY_LOG_WARN("send_stanza: %s", TinyRet_GetDescription(ret));
            break;
        }

    } while (false);

    if (stanza != NULL)
    {
        Stanza_Delete(stanza);
    }
}

void Product_ChangePropertyValue(Product *thiz, int siid, int piid, JsonValue *value)
{
    Stanza *stanza = NULL;
    PropertyOperation *op = NULL;
    char id[32];

    RETURN_IF_FAIL(thiz);
    RETURN_IF_FAIL(value);

    do
    {
        /* 构造属性操作：pid = {did.siid.piid}，value 深拷一份 */
        op = PropertyOperation_NewValue(thiz->config.did, (uint16_t)siid, (uint16_t)piid, value);
        if (op == NULL)
        {
            break;
        }

        /* 校验属性可通知、取值合法（save=false 不持久化，值由应用自持） */
        TinyRet ret = Product_TryChangePropertyValue(thiz, op, false);
        if (RET_FAILED(ret) || op->status != IOT_STATUS_COMPLETED)
        {
            TINY_LOG_WARN("skip properties-changed (ret=%d, status=%d)", ret, op->status);
            break;
        }

        /* 构造 properties-changed 通知并发送 */
        Product_GetNextStanzaId(thiz, id, sizeof(id));

        stanza = Stanza_New(STANZA_IQ);
        if (stanza == NULL)
        {
            break;
        }

        ret = IQ_InitializeQuery(&stanza->data.iq, id, IQ_METHOD_PROPERTIES_CHANGED);
        if (RET_FAILED(ret))
        {
            break;
        }

        QueryPropertiesChanged *changed = &stanza->data.iq.content.query.content.propertiesChanged;
        if (TinyList_AddTail(&changed->properties.properties, op) == NULL)
        {
            break;
        }
        op = NULL; /* 所有权移交 stanza，由 Stanza_Delete 统一释放 */

        send_stanza(thiz, stanza);
    } while (false);

    if (op != NULL)
    {
        PropertyOperation_Delete(op);
    }
    if (stanza != NULL)
    {
        Stanza_Delete(stanza);
    }
}

/**
 * 发送事件通知（message / event-occurred）。
 *
 * 构造一条 STANZA_MESSAGE：
 *   topic  = MESSAGE_TOPIC_DEVICE（urn:xiot:device）
 *   type   = DEVICE_NOTICE_EVENT_OCCURRED（event-occurred）
 *   payload = { eid: "<did>.<siid>.<eiid>", arguments: <深拷贝> }
 *
 * arguments 按引用接收后会深拷贝一份，调用方可继续持有/释放原数组。
 */
void Product_NotifyEventOccurred(Product *thiz, int siid, int eiid, JsonArray *arguments)
{
    Stanza *stanza = NULL;
    char id[32];

    RETURN_IF_FAIL(thiz);

    do
    {
        DeviceEventOccurred *ev = NULL;

        stanza = Stanza_New(STANZA_MESSAGE);
        if (stanza == NULL)
        {
            break;
        }

        Message *msg = &stanza->data.message;
        msg->topic = MESSAGE_TOPIC_DEVICE;
        msg->type = DEVICE_NOTICE_EVENT_OCCURRED;

        Product_GetNextStanzaId(thiz, id, sizeof(id));
        snprintf(msg->id, sizeof(msg->id), "%s", id);

        ev = &msg->payload.data.device.payload.eventOccurred;
        if (RET_FAILED(DeviceEventOccurred_Construct(ev)))
        {
            break;
        }

        /* 填充事件 ID：did.siid.eiid */
        snprintf(ev->eid.did, sizeof(ev->eid.did), "%s", thiz->config.did);
        ev->eid.siid = (uint16_t)siid;
        ev->eid.iid = (uint16_t)eiid;

        /* 深拷贝参数数组；arguments 可为 NULL（无参数事件） */
        if (arguments != NULL)
        {
            ev->argument = JsonArray_NewFrom(arguments);
            if (ev->argument == NULL)
            {
                TINY_LOG_WARN("Product_NotifyEventOccurred: 参数数组拷贝失败");
                break;
            }
        }

        const TinyRet ret = send_stanza(thiz, stanza);
        if (RET_FAILED(ret))
        {
            TINY_LOG_WARN("send_stanza: %s", TinyRet_GetDescription(ret));
            break;
        }
    } while (false);

    if (stanza != NULL)
    {
        Stanza_Delete(stanza);
    }
}