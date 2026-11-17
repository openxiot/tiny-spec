#include <tinyspec/stanza/iq/device/control/invoke_actions.h>

TinyRet QueryInvokeActions_Construct(QueryInvokeActions *thiz)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    return ActionOperations_Construct(&thiz->actions);
}

void QueryInvokeActions_Dispose(QueryInvokeActions *thiz)
{
    RETURN_IF_FAIL(thiz);

    ActionOperations_Dispose(&thiz->actions);
}

TinyRet ResultInvokeActions_Construct(ResultInvokeActions *thiz)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    return ActionOperations_Construct(&thiz->actions);
}

void ResultInvokeActions_Dispose(ResultInvokeActions *thiz)
{
    RETURN_IF_FAIL(thiz);

    ActionOperations_Dispose(&thiz->actions);
}
