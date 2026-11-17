#include <tinyspec/stanza/iq/device/control/invoke_actions.h>

static void OnActionOperationDelete(void *data, void *context)
{
    (void) context;
    if (data != NULL)
    {
        ActionOperation_Delete((ActionOperation *)data);
    }
}

TINYCORE_RET QueryInvokeActions_Construct(QueryInvokeActions *thiz)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    TinyList_Construct(&thiz->actions);
    TinyList_SetDeleteListener(&thiz->actions, OnActionOperationDelete, NULL);
    return TINY_RET_OK;
}

void QueryInvokeActions_Dispose(QueryInvokeActions *thiz)
{
    RETURN_IF_FAIL(thiz);
    TinyList_Dispose(&thiz->actions);
}

TINYCORE_RET ResultInvokeActions_Construct(ResultInvokeActions *thiz)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    TinyList_Construct(&thiz->actions);
    TinyList_SetDeleteListener(&thiz->actions, OnActionOperationDelete, NULL);
    return TINY_RET_OK;
}

void ResultInvokeActions_Dispose(ResultInvokeActions *thiz)
{
    RETURN_IF_FAIL(thiz);
    TinyList_Dispose(&thiz->actions);
}
