/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/codec/stanza_codec.h>
#include <tinyspec/dispatch/send.h>

/*
 * 把一个 stanza 编码成 JSON，经应用注入的 DataSend 发往传输层。
 * 不接管 stanza 的所有权：调用方负责在返回后释放。
 */
TinyRet send_stanza(Product *thiz, Stanza *stanza)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(stanza, TINY_RET_E_NULL_PTR);

    if (thiz->send == NULL)
    {
        return TINY_RET_E_NULL_PTR;
    }

    JsonObject *obj = StanzaCodec_Encode(stanza);
    if (obj == NULL)
    {
        return TINY_RET_E_PARAM_INVALID;
    }

    TinyRet ret = thiz->send(obj, thiz, thiz->sendCtx);

    JsonObject_Delete(obj);

    return ret;
}