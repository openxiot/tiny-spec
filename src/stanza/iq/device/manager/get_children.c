#include <tinycore/json_object.h>
#include <tinyspec/stanza/iq/device/manager/get_children.h>
#include <tinyspec/typedef/product/child/child.h>

static void OnChildDelete(void *data, void *context)
{
    (void) context;
    if (data != NULL)
    {
        Child_Delete((Child *)data);
    }
}

TinyRet QueryGetChildren_Construct(QueryGetChildren *thiz)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);
    thiz->did = NULL;
    return TINY_RET_OK;
}

void QueryGetChildren_Dispose(QueryGetChildren *thiz)
{
    RETURN_IF_FAIL(thiz);
    if (thiz->did != NULL)
    {
        JsonString_Delete(thiz->did);
        thiz->did = NULL;
    }
}

TinyRet ResultGetChildren_Construct(ResultGetChildren *thiz)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);
    TinyList_Construct(&thiz->children);
    TinyList_SetDeleteListener(&thiz->children, OnChildDelete, NULL);
    return TINY_RET_OK;
}

void ResultGetChildren_Dispose(ResultGetChildren *thiz)
{
    RETURN_IF_FAIL(thiz);
    TinyList_Dispose(&thiz->children);
}
