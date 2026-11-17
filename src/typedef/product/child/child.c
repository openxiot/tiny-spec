#include <stdio.h>
#include <tinyspec/typedef/product/child/child.h>
#include <tinycore/tiny_alloc.h>

Child * Child_New(void)
{
    return (Child *)tiny_calloc(1, sizeof(Child));
}

void Child_Delete(Child *thiz)
{
    if (thiz == NULL)
    {
        return;
    }
    tiny_free(thiz);
}

/*
 * get-children 应答只需 did/type/online 元数据，
 * 不深拷贝 Child.device（含 services 树）。
 */
TinyRet Child_Copy(Child *dst, const Child *src)
{
    RETURN_VAL_IF_FAIL(dst, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(src, TINY_RET_E_NULL_PTR);
    if (dst == src)
    {
        return TINY_RET_E_PARAM_INVALID;
    }

    snprintf(dst->did, sizeof(dst->did), "%s", src->did);
    snprintf(dst->type, sizeof(dst->type), "%s", src->type);
    dst->online = src->online;

    return TINY_RET_OK;
}
