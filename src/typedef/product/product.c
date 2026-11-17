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
#include <tinyspec/typedef/product/child/child.h>
#include <tinyspec/typedef/product/product.h>

#include "tinyspec/codec/stanza_codec.h"
#include "tinyspec/dispatch/dispatch.h"
#include "tinyspec/stanza/stanza.h"

static TINYCORE_RET Product_Construct(Product *thiz);

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

static TINYCORE_RET Product_Construct(Product *thiz)
{
    TINYCORE_RET ret = TINY_RET_OK;

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