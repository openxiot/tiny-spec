/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinycore/tiny_log.h>
#include <tinyspec/stanza/iq/device/manager/factory/get_children_factory.h>

Stanza * ResultGetChildren_New(const char *id, ResultGetChildren *children)
{
    Stanza *thiz = NULL;

    RETURN_VAL_IF_FAIL(children, NULL);

    do
    {
        thiz = Stanza_New(STANZA_IQ);
        if (thiz == NULL)
        {
            break;
        }

        if (RET_FAILED(IQ_InitializeResult(&thiz->data.iq, id, IQ_METHOD_GET_CHILDREN)))
        {
            Stanza_Delete(thiz);
            thiz = NULL;
            break;
        }

        ResultGetChildren *dst = &thiz->data.iq.content.result.content.getChildren;
        for (uint32_t i = 0; i < TinyList_GetSize(&children->children); ++i)
        {
            Child *src = (Child *)TinyList_GetAt(&children->children, i);
            Child *copy = Child_New();
            if (copy == NULL)
            {
                TINY_LOG_ERROR("Child_New failed");
                continue;
            }

            if (RET_FAILED(Child_Copy(copy, src)))
            {
                TINY_LOG_ERROR("Child_Copy failed");
                Child_Delete(copy);
                continue;
            }

            if (TinyList_AddTail(&dst->children, copy) == NULL)
            {
                TINY_LOG_ERROR("TinyList_AddTail failed");
                Child_Delete(copy);
            }
        }
    } while (false);

    return thiz;
}